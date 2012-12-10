#include "SmartPlayer.h"

#include "proto.h"
#include "prob.h"		/* for globals */
#include "prob_params.h"

#include "assert.h"
#define ASSERT assert

extern int gSensorX;
extern int gSensorY;
extern int gNewPiece;
extern int gNumPops;

SmartPlayer::SmartPlayer(void)
{
        piece_ratio = ((float)(gParams[param_val_max] - 1)) / 
	              ((float)(kPieces - 1));
        phase_ratio = ((float)(gParams[param_val_max] - 1)) /
	              ((float)(kPhases - 1));

	pop = (pop_struct *)nil;
	pop_index = 0;
	memory = new MemoryClass(gParams[param_mem_size]);
	fClearMemoryOnInit = TRUE;		/* I don't know why we'd want to change this,
			 but if we do, it should be a parameter	(add to gParams) */
	     /* we don't want to change it either */


	/*
		val * ratio = move
	*/
	if ((gParams[param_inc_drop] || gParams[param_rover_on_leash]) &&
	    gParams[param_halt_allowed])  // both allowed, all moves allowed
	    ratio = ((float)moveMax)/(float)gParams[param_val_max];
	else if (!gParams[param_inc_drop] && !gParams[param_rover_on_leash] &&
		 !gParams[param_halt_allowed])  // BOTH disallowed
	    ratio = ((float)(moveMax-2))/(float)gParams[param_val_max];
	else // no incremental drop, so there is no moveDown, so one less move
	    ratio = ((float)(moveMax-1))/(float)gParams[param_val_max];
	       // (or no halt allowed, so there is no moveHalt, so one less mv)

}

SmartPlayer::~SmartPlayer(void)
{
        int level;

	if (memory)
		delete memory;

        // Get rid of tree copies -- see note in SetBrain()
	for(level=0; level < gNumPops; level++)
	  free_tree(ptree[level]);
}

void SmartPlayer::Init()
{
	if (memory && fClearMemoryOnInit)
		memory->Clear();
}

void SmartPlayer::SetBrain( pop_struct *the_pop, int i)
{
        int level;

        pop = the_pop;
	pop_index = i;

	ASSERT(MAXLEVELS >= gNumPops);

	// Alex: We make a *new copy* of each brain-tree (HA HA! isn't
        // there a town in Massachusetts named "Braintree"?!) -- can you
        //guess why?  Well, after many crossovers, a tree's nodes
        //are distributed throughout vitual mem, eh?  Therefore each "eval"
        //of a tree requires many page swaps, and think of how many
        //evals there are per fitness measure: numgames*numpiecepergame*
        //numevalsperpiece.  Pointed out by Astro Teller, but he didn't
        //think of it
	for(level=0; level < gNumPops; level++)
	  ptree[level] = copy_tree(pop[level].population[pop_index], pop);
}

MoveType SmartPlayer::NextMove( Board * board , Piece * piece )
{
	MoveType move;
	int num_terms = 0;               /* counter of number of terminals */
	int adf_level;
	GENERIC val;
	GENERIC terminals[30];    /* BAD HARDWIRED STUFF!!!! writeme*/

	
	ASSERT( pop );

	/*
		Set the globals so eval can find the memory etc.
	*/
	gBoard = board;
	gMemory = memory;


/* Note: this section depends on setup.c's "order": 1 pos 2 base 3 phase */ 
/*                                                    4 roving_sensor */
/* ... 5 on up: ADFs */
/* That is, they have to have to deal with the terminals in the same order. */

#ifdef TERM_PIECE_POS
	terminals[num_terms] = board->PiecePosX();
	num_terms++;
	terminals[num_terms] = board->PiecePosY();
	num_terms++;
#endif

#ifdef TERM_PIECE_BASE
	terminals[num_terms] = (int)(piece->GetBase() * piece_ratio);
	num_terms++;
#endif

#ifdef TERM_PIECE_PHASE
	terminals[num_terms] = (int)(piece->GetPhase() * phase_ratio);
	num_terms++;
#endif

#ifdef TERM_ROVING_SENSOR
	terminals[num_terms] = board->GetBit(gSensorX, gSensorY - 1); //NW
	num_terms++;
	terminals[num_terms] = board->GetBit(gSensorX + 1, gSensorY - 1);//NE
	num_terms++;
	terminals[num_terms] = board->GetBit(gSensorX + 1, gSensorY); //SE
	num_terms++;
	terminals[num_terms] = board->GetBit(gSensorX, gSensorY);     //SW
	num_terms++;
#endif

#ifdef TERM_NEW_PIECE
	terminals[num_terms] = gNewPiece;
	num_terms++;
#endif

	if (gParams[param_term_uniquad]) {
	  terminals[num_terms] = 
	    board->GetBit(gSensorX + 1, gSensorY) +
	      2 * board->GetBit(gSensorX, gSensorY) +
		4 * board->GetBit(gSensorX + 1, gSensorY - 1) +
		  8 * board->GetBit(gSensorX, gSensorY - 1);
	  num_terms++;
	}

	// do left/right/top/bottom edge detectors, in that order:
	if (gParams[param_term_edges]) {
	  terminals[num_terms] = (gSensorX == 1);
	  num_terms++;
	  terminals[num_terms] = (gSensorX == (gParams[param_board_x] - 3));
	  num_terms++;
	  terminals[num_terms] = (gSensorY == 2);
	  num_terms++;
	  terminals[num_terms] = (gSensorY == (gParams[param_board_y] - 2));
	  num_terms++;
	}


	// Eval the adf_levels "bottom up" (which actually means
	// from the highest popnum on down, since pop #0
	// is the "main function"				     
	for(adf_level = gNumPops-1; adf_level >= 0; adf_level--) {
	  // ASSERT(pop[adf_level].terminal_table_size == num_terms);
	  // If specialized, then level 0 has only ADFs and rand-const
	  // writeme:  currently only works with 2 adf levels
	  if (gParams[param_specialize_adf] && (adf_level == 0))
	    terminals[1] = val;
	  load_terminal_set_values(pop, adf_level, terminals);
	  ASSERT(POP == pop);
	  val = eval(ptree[adf_level]);
          ASSERT (val <= gParams[param_val_max]);
	  terminals[num_terms] = val;
	  num_terms++;
	}

	move = (MoveType)(((float)val)*ratio);	// the compiler should cast val automatically, but I don't trust it

        // if no inc drop, then there is no moveDown, so there is one
        // less kind of move, so shift up a move-type to "skip" moveDown.
        // But only if rover is not on a leash -- if he is, we need moveDown.
        if (!gParams[param_inc_drop] && (move >= moveDown) &&
            !gParams[param_rover_on_leash])
            move = (MoveType)((int)move + 1);
	
	// if no moveHalt allowed, then "skip over it".
	if ((!gParams[param_halt_allowed]) && (move >= moveHalt))
	    move = (MoveType)((int)move + 1);

        //  The preceding depends on moveDown being the first of the enum??
	ASSERT (move >= moveDown);
	ASSERT (move < moveMax);
	
	return move;
}

/* EOF */

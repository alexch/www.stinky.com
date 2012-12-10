/*

	Params



parameters for tetris



	note that we should save these per population, but we use

	a single global instead
 */

#include "TetrisParams.h"
#ifdef MACINTOSH
#include "macintosh.h"
#else
typedef int Boolean;
#define TRUE 1
#define FALSE 0
#endif

// it's "C" for historical reasons
// (and it's in src/tetrisprob.cc)
extern "C" {
extern void EnableFunction( char * func, Boolean fEnable );
};

int gParams[paramMax];

static char * szParams[] = {

	"board_x", "board_y", "mem_size", "mem_max",
	"val_min", "val_max", "num_fc", "num_validate",
	"mod_readboard", "drop_time",
	"score_piece", "score_line", "score_pixel", "score_hole",
	"piece_on_board", "auto_init_player",
	"show_mem", 
	"game_time", 
	"inc_drop", 
	"rover_on_leash", "ping_pong", 
	"smallest_fitness", "diff_fitness",
	"rand_start_loc",
	"efd_weight", "efd_threshold", "min_num_fc",
	"binary_tournament", "center_piece", "specialize_adf",
	"halt_allowed",
	"term_roving_sensor", 
	"term_uniquad", "term_edges",
	"sensor_sees_edges",
	// these should move someday
	"fitness_queue",
	"catchup"
};

int LookupParamName( char * sz );

void DefaultParams( void )
{
	gParams[param_board_x] = 16;	/*	x dimension of board */
	gParams[param_board_y] = 16;	/*	y dimension of board */
	gParams[param_mem_size] = 32;	/*	size of memory */
	gParams[param_mem_max] = 32;	/*	contents of memory */
	gParams[param_val_min] = 0;     /*      minimum value returned by */
					/*      eval */
	gParams[param_val_max] = 32;	/*	maximum value returned by eval */
	gParams[param_num_fc] = 5;	/*	number of fitness cases */
	gParams[param_num_validate] = 0;  /*	number of validation trials	 */
	                                  // 0 => use fitness score as validation score
		 
	gParams[param_mod_readboard] = 1;  //	1 => readboard uses MOD math
	                                   //	0 => readboard uses bounded math */
	gParams[param_drop_time] = 10;	/*	number of time steps before dropping a piece one step */
	gParams[param_score_piece] = 1;	/*	points per piece dropped	 */
	gParams[param_score_line] = 10;	/*	points per line erased	 */
	gParams[param_piece_on_board] = 1;  //	0 => piece floats above board
	                                    //	1 => piece bits are returned by board->GetBit()

	gParams[param_auto_init_player] = 0; //	0 => memory erased each game
					     // 1 => memory erased each new piece 

	gParams[param_show_mem] = 1;		/*	Should ShowTetris show memory */
	gParams[param_game_time] = 2000;	/*	Clicks before game timeout */
	gParams[param_inc_drop] = 1;		/*	Does piece fall incrementally? */
						/*	(or all at once)			   */
	gParams[param_rover_on_leash] = 0;	/*	Does rover move with piece? */
						/*	DO NOT TURN ON IF inc_drop ON! */
		/*	because I said so */
		/*	Also, if you turn it on, disable
			functions moveup, moveleft,
			etc. 		*/
		/*	Also, if you use BOARD, turn */
		/*	off! */

	gParams[param_ping_pong] = 0;		/*	Instead of right/left moves, */
						/*	have move/reverse-direction. */
						/*	Also, bounce off walls */

	gParams[param_smallest_fitness] = 0;	/*	Threshold under which a gen0 */
						/*	individual will be regenerated */

	gParams[param_diff_fitness] = 0;	/*	Should a different game be used */
						/*	for each individual fitness */
						/*	case? */

	gParams[param_rand_start_loc] = 0;	/*	Should init horiz piece pos be */
						/*	random (or center (= 0))? */

	gParams[param_efd_weight] = 50; 	/*	How much weight (%) to give curr */
						/*	expected fitness delta for */
						/*	updating overall efd  */

	gParams[param_efd_threshold] = 0;	/*	The efd under which fitness */
						/*	measurement halts. Default is */
						/*	never halt (until max num_fc) */

	gParams[param_min_num_fc] = 0;		/*	Min num fitness cases per indiv */

	gParams[param_binary_tournament] = 0;	/*	If non-zero, then efd disabled. */
						/*	This is the number of games */
						/*	played before half the */
						/*	population is "given up on", */
						/*	i.e. stops getting fitness cases.*/

	gParams[param_center_piece] = 1;	/*	Piece starts in center, else */
						/*	left */


	gParams[param_specialize_adf] = 0; //   If on, level 0 has only
					   //   random-constant and adfN
					   //   for constants 


	gParams[param_halt_allowed] = 1; // Do we give the program the
					 // option to drop the piece
					 // (i.e. explicit halt.)  If not,
					 // it must depend on time-out 

	gParams[param_term_roving_sensor] = 1;	/*	Have 4 terminals for roving */
	                                        /*	sensor */

	gParams[param_term_uniquad] = 0; /*  Have one terminal treating 4 */
	                               	/*	sensors of rover as a binary */
	                                /*	number */
	                               	/* WARNING: INTERACT WITH MAX VAL!! */
		                        /* CODE DOESN'T CHECK... */

	gParams[param_term_edges] = 0;	/* 4 terminals to flag when roving */
	                                /*	 sensor is at the */
                                 	/*	 top/bottom/left/right */

	gParams[param_score_pixel] = 1;
	gParams[param_score_hole] = -3;
	
	gParams[param_sensor_sees_edges] = 0; // 0 -> sensor only travels
					      // inside board
	                                      // 1 -> sensor can travel to
					      // board edges

	gParams[param_fitness_queue] = 0;  // if >0, make a fitness queue
					   // of that size. NOTE: should
					   // only use with pAlwaysEval
					   // ALSO: interacts
					   // with param_catchup

	gParams[param_catchup] = 0;  // Fill fitness queue when new player
				     // is created

}

int LookupParamName( char * sz )
{

	int i;

	for (i=0; i<paramMax; ++i)
		if (!strcasecmp(sz, szParams[i]))
			return i;

	return -1;
}


int ReadParam( char * name, char * val )
{

	int i;

	void * v;

	/*
	Look for a disable_function command
	 */

	if (!strcasecmp(name, "disable_function"))
		EnableFunction( val, FALSE );

	else if (!strcasecmp(name, "enable_function"))
		EnableFunction( val, TRUE );

	else
	{
		i = LookupParamName(name);

		if (i==-1)
			return 1;

		sscanf(val,"%d", &gParams[i]);
		if(i==param_term_uniquad)
			printf("SET %d %s to %d\n",i,name,gParams[param_term_uniquad]);

	}

	return 0;
}

/*
 	Write parameters to stdio file
*/
int WriteParams( FILE * f )
{
	int i;

	for (i=0; i<paramMax; ++i)
	{
		fprintf(f, "%s = %d\n", szParams[i], gParams[i]);
	}
	return 0;
	
}

/*
	Write parameters to stream
*/
int WriteTetrisParams(ostream& str)
{
	int i;
	for (i=0; i<paramMax; ++i)
	{
	  str << szParams[i] << " = " << gParams[i] << endl;
	}
	return 0;
	
}


/* EOF */

/*
	SmartPlayer.h

	A player with a tree for a brain.
	
	11/14 adc	Replaced size/range instance variables with references
				to the global parameters (gParams)
*/
//#pragma once
#ifndef _H_SMARTPLAYER
#define _H_SMARTPLAYER

#include "Player.h"
#include "Board.h"
#include "MemoryClass.h"
#include "Piece.h"

#ifndef GPQ
#ifdef UNIX
#include "../../lib/gpc.h"	/* for tree, GENERIC */
#else
#include "gpc.h"		/* for tree, GENERIC */
#endif
#endif

class SmartPlayer : public Player {
public:

	SmartPlayer(void);				// player with memory
	~SmartPlayer(void);
	
	virtual void Init(void);	     // prepare for new piece to fall
	virtual MoveType NextMove( Board * board , Piece * piece );
	
	void SetBrain( pop_struct *pop, int i ); 
	                        // brain a *bunch* of trees -- ADFs
	
	MemoryClass * memory;   // unprotected so Game can give it to Display
	
	
protected:

#define MAXLEVELS 20

	tree * ptree[MAXLEVELS];  // This is the player's brain!  Yuch!
                              // It should be kept in a ptree dish! Get it?
                              // Wudda binary tree have neurons and neuroffs?

                              // An array of trees, one per ADF level.

        pop_struct *pop;      // These point to the player's brain parts.
        int pop_index;
     
	float ratio;	/*	val * ratio = move	*/

        float piece_ratio;
        float phase_ratio;
	
	Boolean	fClearMemoryOnInit;
	
};


#endif

/* EOF */

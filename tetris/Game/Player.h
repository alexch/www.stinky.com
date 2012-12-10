/*
	Player
*/
//#pragma once
#ifndef _H_PLAYER
#define _H_PLAYER

#include "Board.h"
#include "MemoryClass.h"

typedef enum MoveType {
	moveDown,	 
	moveLeft, 
	moveRight, 
	moveUp,
	moveRotate, 
	moveHalt,
	moveNone, 

	moveMax
} MoveType;

class Player {
public:
	virtual void Init(void);		// prepare for new piece to fall -- (e.g., reset PC)
	virtual MoveType NextMove( Board * board , Piece * piece );

    MemoryClass * memory; 

};

/* ------- */

class RandomPlayer : public Player {
public:
	virtual void Init(void);		// prepare for new piece to fall -- (e.g., reset PC)
	virtual MoveType NextMove( Board * board , Piece * piece );
};

/* ------- */
#if 0
class KeyPlayer : public Player {
public:
	virtual void Init(void);		// prepare for new piece to fall -- (e.g., reset PC)
	virtual MoveType NextMove( Board * board , Piece * piece );
};
#endif


#endif

/* EOF */

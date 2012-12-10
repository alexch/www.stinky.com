/*
	Player.cp

	This module implements individual strategies for tetris
	One strategy will be a GA
*/

#include "Player.h"
#include "assert.h"

#include <stdlib.h>

/*
	Player
	
	Abstract class
*/

void Player::Init(void) {
}

MoveType Player::NextMove( Board * board , Piece * piece )
{
	return moveNone;
}

/*
	RandomPlayer
	
	Picks a random move
*/
void RandomPlayer::Init(void)
{
}

MoveType RandomPlayer::NextMove( Board * board , Piece * piece )
{
	int r = rand();
	MoveType move = (enum MoveType)(r % moveMax);
	assert(move >= moveNone && move < moveMax);
	return move;
}

#ifdef MACINTOSH
/*
	KeyPlayer
	
	Checks for keyboard events
	
*/

#define	keyLeft		0x7B
#define keyRight	0x7C
#define keyDown		0x7D
#define keyUp		0x7E
#define keyEnter	0x4C
#define keyReturn	0x24
#define keySpace	0x31

short KeyIsPressed(unsigned short k );

short KeyIsPressed(unsigned short k )
// k =  any keyboard scan code, 0-127
{
	unsigned char km[16];

	GetKeys( (long *) km);
	return ( ( km[k>>3] >> (k & 7) ) & 1);
}


void KeyPlayer::Init(void)
{
}

MoveType KeyPlayer::NextMove( Board * board , Piece * piece )
{
#define MAP( key, move )	\
		if (KeyIsPressed( key )) return move;
	
	MAP( keyLeft, moveLeft );
	MAP( keyRight, moveRight );
	MAP( keyDown, moveDown );
	MAP( keySpace, moveRotate );
	MAP( keyEnter, moveHalt );
	MAP( keyReturn, moveHalt );
	
	return moveNone;
}

#endif

/* EOF */

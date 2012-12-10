/*
	Piece.h
	
	A piece returns a 4x4 bit grid in the form of an unsigned short.
*/
//#pragma once
#ifndef _H_PIECE
#define _H_PIECE

#include <iostream.h>

#ifndef MACINTOSH
#include "Unix/config.h"
#endif

#include "RandomClass.h"

#ifdef UNIS
#define kPieces 1
#else
#ifdef DITRIS
#define kPieces 2
#else
#define kPieces	7
#endif
#endif

#define kPhases	4

class Piece {

public:

	Piece(int seed);
	~Piece(void);       // writeme?
	
	void SetRandom(void);
	
	unsigned short GetBits(void);
	Boolean GetBit(int x, int y);

	int GetBase(void);
	int GetPhase(void);
	
	void Rotate(void);
	void Unrotate(void);
		
	RandomClass *r; // evs unprotected so Board could use it
	                // to produce random init x pos of piece

  void Read(istream& str);
  void Write(ostream& str);
   
protected:

	int base;
	int phase;
	
};


void InitPieces(void);


#endif

/* EOF */

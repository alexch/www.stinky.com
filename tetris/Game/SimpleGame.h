//#pragma once
#ifndef _H_SIMPLEGAME
#define _H_SIMPLEGAME

#include <iostream.h>
#include "Player.h"  // for Move

typedef enum {STEP_NONE, STEP_ILLEGAL, STEP_LINE, STEP_NEW, STEP_FULL} STEP_VAL;

class SimpleGame  {
	
public:
  
  SimpleGame(int x = 12, int y = 24);
  ~SimpleGame(void);
  
  void Init(int seed);
  STEP_VAL Step(MoveType move);

  void Print(ostream& str);
  void Print(ostream& str, char * szWall, char * szPiece, char * szBoard,
	     char * szBlank, char *szEOL);
  void Read(istream& str);
  void Write(ostream& str);

  long score;
  unsigned long time;
  
 protected:
  Board * board;
  Piece * piece;
  unsigned long timeNext;
  long	timeStep;	//	number of ticks between drops of the piece
  unsigned long	timeMax;  //	the maximum number of ticks a game can take

  int lines;
  int pieces;

  int scorePiece;	// number of points per new piece
  int scoreLine;        // number of points per completed line
  int scorePixel;
  int scoreHole;

  int	xBoard;		//	board dimensions
  int 	yBoard;


    inline float CalculateScore( int nPieces, int nLinesCleared, int nPixels, int nHoles )
	{
		/* 
		    Calculate Score
			formula: A*pieces + B*lines + C*pixels + D*holes + ???
		*/
		
		return nPieces * scorePiece +
			nLinesCleared * scoreLine +
			nPixels * scorePixel +
			nHoles * scoreHole;
	}
	
	float CalculateScore(Board * board, int pieces, int lines)
	{
			int pixels, holes;
			board->CountPixels(&pixels, &holes);
			return CalculateScore( pieces, lines, pixels, holes );
	}
};

#endif

/* EOF */

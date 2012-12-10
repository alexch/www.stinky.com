//#pragma once
#ifndef _H_GAME
#define _H_GAME

#include "Player.h"
//	#include "SmartPlayer.h"
#include "Display.h"

class GameStats {
public:
  float pieces;
  float lines;
  float pixels;
  float holes;
  // more stuff

  GameStats() { pieces = lines = pixels = holes = 0; };
  void operator+=(const GameStats& stats) {
    pieces += stats.pieces;
    lines += stats.lines;
    pixels += stats.pixels;
    holes += stats.holes;
  }
  void operator/=(const float div) {
    pieces /= div;
    lines /= div;
    pixels /= div;
    holes /= div;
  }
  void print(ostream& str)  {
  	str << pieces << " Pieces\t" << lines <<" Lines\t" << pixels << " Pixels\t" << holes << " Holes\n";
  }
  
};

class Game  {
	
public:

	Game(int x, int y);		// board size
	
	long Run( Player * player, Display * display, int seed,
		  Boolean fValidating = FALSE, GameStats * stats = NULL );

protected:

	Boolean	fAutoInit;	//	does the PC reset each time a new piece drops
	
	int		scorePiece;	// number of points per new piece
	int		scoreLine;		// number of points per completed line
	int		scorePixel;
	int		scoreHole;

	int		xBoard;		//	board dimensions
	int 	yBoard;

	long	timeStep;	//	number of ticks between drops of the piece
	unsigned long	timeMax;	//	the maximum number of ticks a game can take

	inline int CalculateScore( int nPieces, int nLinesCleared, int nPixels, int nHoles )
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
	
	int CalculateScore(Board * board, int pieces, int lines)
	{
			int pixels, holes;
			board->CountPixels(&pixels, &holes);
			return CalculateScore( pieces, lines, pixels, holes );
	}
};

#endif



/* EOF */

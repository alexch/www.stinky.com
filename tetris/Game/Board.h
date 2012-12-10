/*
	Board.h
*/
#ifndef _H_BOARD
#define _H_BOARD

#include <iostream.h>

#include "Piece.h"


#define MaxRows 64


/*
	Here's a board:
	01234567...  31		<< which bit, counting from low to high
	#.....#-------
	#.....#-------
	#..#..#-------
	#..##.#-------   My name is Prince, and I am funky.
	#..##.#-------   My name is Prince, the one and only.
	#####.#-------
	#######-------
*/

class Board {
	friend class Display;	//	a Display can access a board's private parts
	friend class WindowDisplay;	//	a Display can access a board's private parts
	friend class TextDisplay;//	a Display can access a board's private parts
	
public:
	
	Board(int x, int y);
	~Board(void);
	
	Board * Copy(void);
	
	Boolean	IsFull(void);
	
	int	SetPiece( Piece * piece ); // returns init Y loc of piece
	
	Boolean	MoveLeft(void);
	Boolean MoveRight(void);
	Boolean MoveUp(void);
	Boolean MoveDown(void);
	
	Boolean RotatePiece(void);
	
	int	DropPiece(void);
	
	Boolean	GetBit( int x, int y );
        int     Window( int x1, int y1, int x2, int y2);

        int     PiecePosX();
        int     PiecePosY();
	
	void CountPixels( int * ppixels, int * pholes = NULL );

        // EVS made these two things unprotected so Display can be better:
	Piece *	piece;
	int xPiece, yPiece;

  void Print(ostream& str, char * szWall = "#", char * szPiece = "X", char
    * szBoard = "O", char * szBlank = " ", char * szEOL = "\n");
  void Read(istream& str);
  void Write(ostream& str);

protected:

	int	xMax, yMax;		// dimensions

	unsigned long 	* rows;	// data for all rows
						// pointer to array of longs

	
	Boolean	fPieceIsOnBoard;		//	GetBit returns piece->GetBit too

	unsigned short GetBits( int x, int y );
   	Boolean Move_Any( int x, int y );
	
};

#endif

/* EOF */


















































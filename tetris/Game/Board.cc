#ifndef MACINTOSH
#include "Unix/config.h"
#endif

#include <stdio.h>
#include <stdlib.h>
#include <math.h>

#ifndef MACINTOSH
//#include "random.h"
#endif

#include "Board.h"
#include "Assert.h"
#define ASSERT assert

#ifdef GPQ
#include "TetrisParams.h"
#else
// #include "prob_params.h"
#endif


extern float pow(float,float);

// #define RANGE_CHECK		/*	Check the range of GetBit every time	*/

#define	BIT(x)		((unsigned long)(1L << (x)))

Board * Board::Copy(void)
{
	Board * boardNew = new Board(xMax,yMax);
	memcpy(boardNew->rows, this->rows, yMax * sizeof(unsigned long));
	return boardNew;
}

Board::Board(int x, int y)
{
	unsigned long	row;
	int counterY, counterX, i;
	
	ASSERT( x < sizeof(long) * 8 );	/* max width */
	ASSERT( x >= 6 );		/* width of piece + borders */
	ASSERT( y < MaxRows);

	//	set instance variables
	fPieceIsOnBoard = gParams[param_piece_on_board];
	xMax = x; 
	yMax = y;
	piece = (Piece *)nil;
	xPiece = yPiece = 0;
	
	//	allocate array
	//	just use a single long for the x dimension -- max. 32 columns
	rows = (unsigned long *) malloc(yMax * sizeof(unsigned long));
	
	//	clear it out
	row = BIT(0) | BIT(x - 1);
	for (i=0; i<yMax; ++i)
	{
		rows[i] = row;
	}
	rows[yMax-1] = ~(unsigned long)0L;

// eric -- you're using the wrong random function here, I think...
//	anyway, we don't use it, so fuck it
#if 0
	//      put on them rows o' RANDOM JUNK
        for(counterY = yMax-2; counterY > yMax-2-gParams[param_junk_height]; 
             counterY--) 
            for(counterX = 1; counterX < xMax-1; counterX++)
	    {
                if (random_int(2)) {
	            rows[counterY] = rows[counterY] | BIT(counterX);
                }
	    }
#endif

}

Board::~Board()
{
     ASSERT(rows);
     free(rows);
}

/*
	A board is full if there's any pixels (other than borders) above row 4
*/
Boolean Board::IsFull(void)
{
	unsigned long row;
	int i;
	
	row = ~(BIT(0) | BIT(xMax - 1));
	
	for (i=0; i<4; ++i)
		if (rows[i] & row)
			return TRUE;
	return FALSE;
}

Board::SetPiece(Piece * piece)
{
	this->piece = piece;
	yPiece = 0;
	if (gParams[param_rand_start_loc])
	  return(xPiece = (piece->r->next(xMax-4) + 1));
	else if (gParams[param_center_piece])
	  return(xPiece = (xMax - 4) / 2); // put it in the middle
	else
          return(xPiece = 1);
}

/*
	Try to move the piece. 
	Return 	TRUE if it could move (updating its position)
			FALSE if it could not (keeping its position the same)
	
	Implementation:
		Exploit the fact that a piece is 4x4 bits, as follows:
		Treat the piece and the target location as 16-bit integers.
		If there is any overlap, it will show up during a logical AND.
*/

/*
	Build a 16-bit integer from the bits at point x,y
*/
unsigned short Board::GetBits( int x, int y )
{
	unsigned long bits = 0;
	unsigned long mask;
	int i;
	
	mask = 0x0FL << x;	// 4 bits, shifted over
	
	for ( i = y + 3; i >= y; --i)
	{
		bits <<= 4;
		if (i < yMax)
			bits |=  (rows[i] & mask) >> x;
	}
	return (unsigned short)bits;
}

Boolean Board::Move_Any( int x, int y )
{
	unsigned short bitsMine;
	unsigned short bitsPiece;
	Boolean	answer = FALSE;
	
	if (x < 0 || x >= xMax || y < 0 || y > yMax)
		return FALSE;
	
	// Try it in the new position
	bitsMine = GetBits( x, y );
	bitsPiece = piece->GetBits();
	
	if (bitsMine & bitsPiece)
		return FALSE;
	else
	{
		xPiece = x;
		yPiece = y;
		return TRUE;
	}	
}

Boolean	Board::MoveLeft(void)
{
	return( Move_Any( xPiece - 1, yPiece ) );
}
	
Boolean	Board::MoveRight(void)
{
	return Move_Any( xPiece + 1, yPiece );
}

Boolean	Board::MoveUp(void)
{
	return Move_Any( xPiece, yPiece - 1 );
}

Boolean	Board::MoveDown(void)
{
	return Move_Any( xPiece, yPiece + 1 );
}

Boolean Board::RotatePiece(void)
{
	unsigned short bitsMine;
	unsigned short bitsPiece;
	
	piece->Rotate();
	
	bitsMine = GetBits( xPiece, yPiece );
	bitsPiece = piece->GetBits();
	
	if (bitsMine & bitsPiece)
	{
		piece->Unrotate();
		return FALSE;		
	}
	else
		return TRUE;
}

/*
	DropPiece
	
	This puts the current piece on the board, and
	checks for completed lines.
	
	Note that the piece is not deleted. It's still
	owned by the caller.
*/
int	Board::DropPiece(void)
{
	int i,j;
	unsigned short bits;
	int lines = 0;
	
	bits = piece->GetBits();
	
	for (i=0; i<4; ++i)
	{
		if (i+yPiece < yMax)
		{
			rows[i+yPiece] |= ( (bits & (0x0FL << (i*4))) // get proper 4-bit sequence
								>> (i*4) ) 				// shift it down to 0-bit
								<< xPiece;				// and shift it out to proper x location
		}
	}
	
	//	Now check for completed lines
	
	for (i = yMax-2; i >= 0; --i)
	{
		if (rows[i] == ~(~0L << xMax))
		{
			lines++;
			// shift all rows down
			for (j=i-1; j>=0; --j)
				rows[j+1] = rows[j];
			// this used to say "rows[j]" with j==-1 -- that's a bug, hm?
			rows[0] = BIT(0) | BIT(xMax - 1);
			i++;       // EVS 1/29/94
			           // "prevents i from decrementing"
				   // so EVERY row is checked.
		}
	}
	
	return lines;

}

/*
	This function is probably the biggest bottleneck.
	It's unnecessary to do range checking once
	you've debugged it, so I've set a compile flag to
	remove it.
*/
Boolean Board::GetBit(int x, int y)
{
#ifdef	RANGE_CHECK
	ASSERT( x >=0 && x < xMax );
	ASSERT( y >=0 && y < yMax );
#endif
	
	if (rows[y] & BIT(x))
		return TRUE;
	if (fPieceIsOnBoard && piece)
		return (piece->GetBit(x - xPiece, y - yPiece));
	else return FALSE;
}

/*  Given a rectangle, how many bits are on in that area? */
/*  PieceIsOnBoard NOT IMPLEMENTED FOR THIS currently. */
int Board::Window(int x1, int y1, int x2, int y2)
{
    int i, y, smallerX, sum=0;
    unsigned long mask, on_bits;

#ifdef	RANGE_CHECK
	ASSERT( x1 >=0 && x1 < xMax );
	ASSERT( y1 >=0 && y1 < yMax );
	ASSERT( x2 >=0 && x2 < xMax );
	ASSERT( y2 >=0 && y2 < yMax );
#endif

    mask = (unsigned long) (1 << (abs(x1-x2)+1) ) - 1L;
    if (x1 > x2) {
        mask = mask << x2;
	smallerX = x2;
      }
    else {
        mask = mask << x1;
	smallerX = x1;
      }

    if (y1 > y2) {
        for(y=y2; y <= y1; y++) {
	   on_bits = (rows[y] & mask) >> smallerX;
           for(i=smallerX; i <= abs(x1-x2); i++) {
	       if ((on_bits % 2) == 1)
	           sum++;
	       on_bits = on_bits >> 1;
	   }
	}
    }
    else {
        for(y=y1; y <= y2; y++) {
	   on_bits = (rows[y] & mask) >> smallerX;
           for(i=smallerX; i <= abs(x1-x2); i++) {
	       if ((on_bits % 2) == 1)
	           sum++;
	       on_bits = on_bits >> 1;
	   }
	}
    }

    return(sum);
}


int Board::PiecePosX()
{
    return(xPiece);
}


int Board::PiecePosY()
{
    return(yPiece);
}

/*
	Returns the number of pixels filled in the board as well as the
	number of "holes." 
	A hole is an empty pixel with a full pixel somewhere above it
*/
void Board::CountPixels(int * ppixels, int * pholes )
{
  int x,y,flag;
  int pixels = 0, holes = 0;
  
  Boolean fSave = fPieceIsOnBoard;
  fPieceIsOnBoard = FALSE;
  
  // don't include the walls
  for (x=1; x < xMax-1; x++) 
    {
      flag = 0;
      for (y=0; y < yMax-1; y++) 	    // don't include the "floor"
	{
	  if (GetBit(x,y)) 
	    {
	      pixels++;
	      flag = 1;	// have seen first on-pixel
	    }
	  else if (flag)
	    holes++; 
	}
    }
  
  fPieceIsOnBoard = fSave;
  
  if (ppixels)
    *ppixels = pixels;
  if (pholes)
    *pholes = holes;
}

void Board::Print(ostream& str, char *szWall, char *szPiece, char *szBoard, char * szBlank, char * szEOL)
{
  for (int y=0; y < yMax; ++y)
    {
      for (int x=0; x<xMax; ++x)
	{
	  if (x==0||x==xMax-1||y==yMax-1)
	      str << szWall;
    	  else if (piece && piece->GetBit(x - xPiece, y - yPiece))
	      str << szPiece;
	  else if (this->GetBit( x,  y ))
	      str << szBoard;
	  else
	      str << szBlank;
	}
      str << szEOL;
    }
}

void Board::Write(ostream& str)
{
  str
    << ' ' <<xMax << ' ' <<yMax
    << ' ' <<xPiece << ' ' <<yPiece << endl;
  for (int y=0;y<yMax;++y)
    str << rows[y] << ' ';
  str << endl;
}

void Board::Read(istream& str)
{
  char ch;
  str >> ws;
  str >> xMax >> yMax
    >> xPiece >> yPiece;

//  cout << "Board: xMax " << xMax << "  yMax " << yMax << " xPiece " <<
//    xPiece << " yPiece " << yPiece << endl;

  free(rows);
  rows = (unsigned long *) malloc(yMax * sizeof(unsigned long));
  for (int y=0;y<yMax;++y)
    {
      str >> rows[y];
//      cout << '\t' << rows[y];
    }
  piece= nil;
}

/* EOF */

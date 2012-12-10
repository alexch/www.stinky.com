/*
	Game.cc
	
	Plays a game of Tetris
	
*/

#ifdef MACINTOSH
#include "MacUtilities.h"
#else
#include "config.h"
#endif

#include <stdio.h>

#include "SimpleGame.h"

#include "Globals.h"

#include "Assert.h"
#define ASSERT assert
	
SimpleGame::SimpleGame(int x, int y)
{
	scorePiece = 1;
	scoreLine =  10;
	scorePixel = 0;
	scoreHole =  0;
	
	xBoard = x;		// the caller gets these from gParams normally
	yBoard = y;
	
	timeStep =	1;
	timeMax =	10000; 

	board = nil;
}

void SimpleGame::Init(int seed)
{
  board = new Board(xBoard, yBoard);
  piece = new Piece(seed);
  piece->SetRandom();
  board->SetPiece(piece);

  score = 0;
  time = 0;
  timeNext = time + timeStep;

  lines = pieces = 0;
}

STEP_VAL SimpleGame::Step(MoveType move)
{
  Boolean fNewPiece = FALSE;
  Boolean fMoveHappened = TRUE;   // Was an attempted move valid?
  int retval = STEP_ILLEGAL;
  switch (move) {
  case moveLeft:
    fMoveHappened = board->MoveLeft();
    break;
  case moveRight:
    fMoveHappened = board->MoveRight();
    break;
  case moveRotate:
    fMoveHappened = board->RotatePiece();
    break;
  case moveHalt:
    while (board->MoveDown() == TRUE);
    fMoveHappened = TRUE;
    break;
  case moveDown:
  default:
    retval = STEP_NONE;
    // ignore for now
    break;
  }
  if (fMoveHappened)
    retval = STEP_NONE;

  // increment time
  time += 1;

  // Try to move it down
  if (time >= timeNext || move == moveDown) {
    timeNext = time + timeStep;
    if (board->MoveDown() == FALSE) {
      int xlines = board->DropPiece();
      if (xlines) retval = STEP_LINE;
      lines += xlines;
      fNewPiece = TRUE;
      if (board->IsFull()) {
	retval = STEP_FULL;
	fNewPiece = FALSE;
      }
    }
  }

  if (fNewPiece) {
    piece->SetRandom();
    board->SetPiece(piece);
    pieces++;
  }

  score = CalculateScore( board, pieces, lines );
	
  return retval;

}

void SimpleGame::Print(ostream& str)
{
  // prints the board and the score
   board->Print(str);
  str << "Score: " << score << "<P>" << endl;
}

void SimpleGame::Print(ostream& str, char * szWall, char *szPiece, char
		       *szBoard, char *szBlank, char *szEOL)
{
  board->Print(str, szWall, szPiece, szBoard, szBlank, szEOL);
  str << "Score: " << score << "<P>" << endl;
}

void SimpleGame::Write(ostream& str)
{
  str <<  ' ' << xBoard <<  ' ' << yBoard;
  str << ' '
    << time << ' '<< timeNext <<  ' '<< timeStep <<  ' '<< timeMax
    <<  ' '<< lines <<  ' '<< pieces
    <<  ' '<< scorePiece <<  ' '<< scoreLine <<  ' '<< scorePixel <<  ' '<< scoreHole
    << endl;
  board->Write(str);
  piece->Write(str);
}

void SimpleGame::Read(istream& str)
{
  str >> ws
    >> xBoard >> yBoard
    >> time >> timeNext >> timeStep >> timeMax
    >> lines >> pieces
    >> scorePiece >> scoreLine >> scorePixel >> scoreHole;

//  cout << "Time: " << time << " xBoard: " << xBoard << endl;

  board = new Board(xBoard, yBoard);
  board->Read(str);
  piece = new  Piece(0);
  piece->Read(str);
  board->piece = piece;
}

/* EOF */

/*
	Game.cp
	
	Plays a game of Tetris
	
*/

#ifdef MACINTOSH
#include "MacUtilities.h"
#else
#include "config.h"
#endif

#include <stdio.h>

#include "Game.h"
#include "Player.h"
#include "Display.h"

#include "Globals.h"

#ifdef GPQ
#include "TetrisParams.h"
#else
#include "prob.h"
#include "prob_params.h"
#endif

#ifdef PROFILE
#include <profile.h>
#endif
#include "Assert.h"
#define ASSERT assert
	
Game::Game(int x, int y)
{
	fAutoInit =  gParams[param_auto_init_player];
	scorePiece = gParams[param_score_piece];
	scoreLine =  gParams[param_score_line];
	scorePixel = gParams[param_score_pixel];
	scoreHole =  gParams[param_score_hole];
	
	xBoard = x;		// the caller gets these from gParams normally
	yBoard = y;
	
	timeStep =	gParams[param_drop_time];
//	timeMax =	100000L;    // EVS
	timeMax =	gParams[param_game_time];
	
}

/*
	In:
		player
		display
			
	returns number of points accumulated during this game

	sets gEfficiency because we're too lazy to change the header to add another parameter

	if stats parameter non-NULL, fills it at end of game

*/
long Game::Run( Player * player, Display * display, int seed, 
	        Boolean fValidating,     // If validating, then no game_time
	       GameStats * stats)  // fills with info
{
	Board * board;
	long	score = 0;
	unsigned long	time, timeNext;
	Piece *	piece = (Piece *)nil;
	MoveType	move;
	long	lines;
	int     direction = 0;   // starts going left
	Boolean fNewPiece = TRUE;
	Boolean fMoveHappened = TRUE;   // Was an attempted move valid?
	Boolean fSensorMoved;
	int     piece_zero_time; //What time was it when last piece was dropped

	long    nLinesCleared=0;
	//, nPiecesDropped=0;
#define nPieces (gNumPieces + 1)

#ifdef PROFILE
	_profile = 1;
#endif

	gDisplay = display;
	gNewPiece = FALSE;

	board = new Board(xBoard, yBoard);
	player->Init();    // This clears the dudes memory. Amnesia, but
	                   // he's still smart.
	
	if (display)
		display->Init( board, player->memory );	// initialize display
	
	piece = new Piece(seed);
        piece->SetRandom();  

        gNumPieces = -1;

	for (time = 0; fValidating || (time < timeMax) ; ++time)
	{
		#ifdef MACINTOSH
		if (Button())	
			Event();
		#endif
		if (fNewPiece == TRUE)
		{
			// why do we have a global here?
                        gNumPieces++;
                        gThinkTime[gNumPieces % MAXTHINKTIMEMEM] = 0;
                        piece_zero_time = time;
                        
			fNewPiece = FALSE;
                        gNewPiece = TRUE;  // tell player

	                piece->SetRandom();
	                gSensorX = board->SetPiece(piece);// sets it to the top
                        // gSensorY = 2;  // set sensor to upper left
                        gSensorY = yBoard - 2; // set sensor to lower left
                        
                        if (display)
                        {	
				score = CalculateScore( board, nPieces, nLinesCleared );
	                       	display->Update(time - (timeNext - timeStep), score);
	                        display->UpdateSensor(gSensorX, gSensorY);
			}
			
			timeNext = time + timeStep;
			
			if (fAutoInit)
				player->Init();
				
		}

	
		fMoveHappened = FALSE;

		move = player->NextMove( board, piece );
                gNewPiece = FALSE;

#if 0		// this is a better way to do ping pong
		// if we reimplement it, do it this way
		if (gParams[param_ping_pong]) {
			if (direction) {
				if (move == moveLeft) move = moveRight;
				else if (move == moveRight) move = moveLeft;
			}
			fBounced = FALSE;
		}
#endif

		switch( move )
		{
		
			case moveLeft:
                        	if (gParams[param_rover_on_leash])
                        	{
                        		// first move sensor
                        		fSensorMoved = !MoveGlobalSensor(-1, 0);
                        		
                        		// if he moved, then try to move the piece too
                        		// logic: keep moving it to the left until
                        		// you can't move it no more, or until it's 
                        		// aligned
                        		if (fSensorMoved) 
                        		{
                        		  do {
                        			fMoveHappened = board->MoveLeft();
                        		  } while (fMoveHappened && board->xPiece > gSensorX);
                        		}
                        		// if he didn't move, then try to move the piece
                        		// anyway -- maybe the sensor is blocked by the edge
                        		// of the board
                        		else
					{
                        			fMoveHappened = board->MoveLeft();
                        		}
				}
				else
				{
					board->MoveLeft();
				}
				break;
				
			case moveRight:
				if (gParams[param_rover_on_leash])
				{
					fSensorMoved = !MoveGlobalSensor(1, 0);
					if (fSensorMoved) {
					  do {
						fMoveHappened = board->MoveRight();
					  } while (fMoveHappened && board->xPiece < gSensorX);
					}
					else
					{
						fMoveHappened = board->MoveRight();
					}
				}
				else
				{
					board->MoveRight();
				}
				break;
				
			case moveRotate:
				fMoveHappened = board->RotatePiece();
				break;
				
			case moveHalt:
				//	Halt = move down all at once
                                gThinkTime[gNumPieces % MAXTHINKTIMEMEM] = 
                                  (time - piece_zero_time) + 1;
		                do
		                    if (display)  {
					score = CalculateScore( board, nPieces, nLinesCleared );
			                display->Update( time - (timeNext - timeStep), score );
		                    }
		              	while (board->MoveDown() == TRUE);
				break;
				
			case moveDown:
                                if (gParams[param_rover_on_leash])
                                    fMoveHappened = !MoveGlobalSensor(0,1);
				break;

			case moveNone:
				break;

			case moveUp:
                                if (gParams[param_rover_on_leash])
                                    fMoveHappened = !MoveGlobalSensor(0,-1);
				break;
							
			default:
				ASSERT(FALSE);
				break;
				
		}
		
                
                // If rover on leash we know there is no inc drop, so
                // moveDown really only refers to Rover.  But if there is
                // inc drop then we want to enter this part.
		if ( time == timeNext || move == moveHalt ||
                    (move == moveDown && !gParams[param_rover_on_leash]))
		{
			fMoveHappened = TRUE;

                        // This only activated and matters if noincdrop and
                        // rover not on leash and moveDown still exists,
                        // which would never happen as of 8/12/94
                        // Or if time out!
                        if (!gParams[param_inc_drop]) //no incremental drop
				while (board->MoveDown() == TRUE){
		                    if (display)  {
		                    	score = CalculateScore(board, nPieces, nLinesCleared);
			                display->Update( time - (timeNext - timeStep), score );
			                // don't need to update memory
		                    }
		                    
                                }

			if (board->MoveDown() == FALSE)	 // couldn't move down
			{
				lines = board->DropPiece();
				nLinesCleared += lines;
				fNewPiece = TRUE;
				
				if (board->IsFull())		// uh-oh
					break;
			}
			timeNext = time + timeStep;
		}
		
		if (display) {
			score = CalculateScore( board, nPieces, nLinesCleared );
		  	display->Update( time - (timeNext - timeStep), score);
		  	display->UpdateMemory( );
		  	
		}

	}	// for time
	
	gDisplay = (Display *)nil;

	int pixels, holes;
	board->CountPixels(&pixels, &holes);
	score = CalculateScore( nPieces, nLinesCleared, pixels, holes );
	
        ASSERT(board);
        delete board;
        board = nil;
        ASSERT(piece);
        delete piece;
	piece = nil;
	
	gEfficiency = ((float)score) / ((float)time);
	gLinesCleared = nLinesCleared;
	// gNumPieces already tallied

	if (stats) 
	  {
	    stats->pieces = nPieces;
	    stats->lines = nLinesCleared;
	    stats->pixels = pixels;
	    stats->holes = holes;
	    /* ... */
	  }
	

	return score;

}

/* EOF */

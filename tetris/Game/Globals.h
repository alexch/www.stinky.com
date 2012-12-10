#include "Board.h"
#include "MemoryClass.h"
#include "Display.h"

#ifndef GLOBAL
	#define GLOBAL extern
#endif

GLOBAL class Board * gBoard;
GLOBAL class MemoryClass * gMemory;
GLOBAL class Display * gDisplay;
GLOBAL class Piece * gPiece;

GLOBAL int gSensorX;
GLOBAL int gSensorY;

GLOBAL int gNewPiece;

GLOBAL int gNumPops;

#define MAXTHINKTIMEMEM		2000
GLOBAL int *gThinkTime;  // tree evals per piece

GLOBAL int gNumPieces;    // num pieces

GLOBAL int gLinesCleared;

GLOBAL float gEfficiency;
GLOBAL int gUniQuadSensor;

void InitGlobals(void);
void DisposeGlobals(void);

int ReadGlobalBoard(int x, int y);
int ReadGlobalWindow(int x1, int y1, int x2, int y2);
int PeekGlobalMemory(int i);
int PokeGlobalMemory(int i, int val);
int MoveGlobalSensor( int delta_x, int delta_y );

/* EOF */

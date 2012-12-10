/*

These functions are compiled in C++ (so they know about

the globals) but declared extern "C" (so they can be called

from setup.c
*/

#ifdef MACINTOSH
#include "macintosh.h"
#include <stdlib.h>
#endif

#include "assert.h"
#include "Display.h"

#ifdef GPQ
#include "TetrisParams.h"
#endif

/*
	This little trick makes the globals be defined here in this
	source file, while for everyone else, they're just declared.
	(Or maybe that's the other way around :-)
*/
#define GLOBAL
#include "Globals.h"

#define ASSERT assert

void InitGlobals(void)
{
	gBoard = (Board *)nil;
	gMemory = (MemoryClass *)nil;
	gDisplay = (Display *)nil;
	gThinkTime = (int *) malloc(sizeof(int) * MAXTHINKTIMEMEM);
}

void DisposeGlobals(void)
{
	if(gBoard)
		delete gBoard;
	if(gMemory)
		delete gMemory;
	if(gDisplay)
		delete gDisplay;
	if (gThinkTime)
		free(gThinkTime);

}

int ReadGlobalBoard( int x, int y )
{

	Boolean bit;
	
	ASSERT (gBoard);
	
	bit = gBoard->GetBit(x, y);
	
	if (gDisplay)
		gDisplay->Flash(x,y);
	
	return bit;
}

int ReadGlobalWindow(int x1, int y1, int x2, int y2)
{
	ASSERT (gBoard);

	return gBoard->Window(x1, y1, x2, y2);
}

int PeekGlobalMemory( int i )
{
	
	ASSERT(gMemory);
	ASSERT(i < gMemory->GetSize());
  	if (gDisplay)
  		gDisplay->FlashMemory(i);
	return gMemory->Peek(i);
}

int PokeGlobalMemory( int i, int val )
{
	
	ASSERT(gMemory);
	ASSERT(i < gMemory->GetSize());
	
	int ans = gMemory->Poke( i, val );

	if (gDisplay)
	{
		gDisplay->UpdateMemory();	// Assumes gMemory == gDisplay->memory
		gDisplay->FlashMemory(i);
	}
	return ans;
}

/*
	If it bumps a wall, stop it and return 1
	else return 0
	
	The sensor coordinates are at the lower-left of
	the sensor
*/
int MoveGlobalSensor( int delta_x, int delta_y )
{
	int answer = 0;
	int xMin, xMac, yMin, yMac;
	
	if (gParams[param_sensor_sees_edges])
	{
		xMin = 0; xMac = gParams[param_board_x] -2;
		yMin = 1; yMac = gParams[param_board_y] -1;
	}
	else
	{
		xMin = 1; xMac = gParams[param_board_x] -3;
		yMin = 2; yMac = gParams[param_board_y] -2;
	}
	
	gSensorX += delta_x;
	if (gSensorX < xMin)
		gSensorX = xMin, answer = 1;

	if (gSensorX > xMac)
		gSensorX = xMac, answer = 1;
		
	gSensorY -= delta_y;
	if (gSensorY < yMin)
		gSensorY = yMin, answer = 1;
	if (gSensorY > yMac)
		gSensorY = yMac, answer = 1;
	
	if (gDisplay)
		gDisplay->UpdateSensor(gSensorX, gSensorY);
	
	return answer;
}

/* EOF */

#include <stdio.h>
#include "Display.h"
#include "Globals.h"

#ifdef UNIX

// iostream.h and curses.h each define a macro called "clear"
//    I have to #undef it to avoid a conflict, and hope this source file
//    never uses the iostream sense
#undef clear

#include <curses.h>
#define USE_CURSES
#include "Unix/config.h"
  #ifndef GPQ
  #include <prob.h>
  #endif
#else
#include <console.h>
#include <LoMem.h>		// for HiliteMode

#endif  // UNIX

#ifdef MACINTOSH
#include "MacUtilities.h"
#endif

#include "Assert.h"
#define ASSERT assert

#define	kLeft	60
#define kTop	10
#define kWidth	8
#define kHeight	8

#define kDelay	2	/* bit flash delay */

Display::Display()
{
	Init(nil, nil);
}

void Display::Init( Board * board, MemoryClass * memory )
{
	sensorX = sensorY = -1;
	this->board = board;
	this->memory = memory;
}

void Display::Update( unsigned long time, long score)
{
}

void Display::Flash( int x, int y )
{
}

void Display::FlashMemory(int mem)
{
}

void Display::UpdateMemory(void)
{
}

void Display::Pause(void)
{
}

void Display::UpdateSensor( int x, int y, int fForce )
{
}

#ifdef MACINTOSH

static void FlashRect(Rect * pr);

WindowDisplay::WindowDisplay( WindowPtr wind )
{
	this->wind = wind;
	this->memoryOld = nil;
	this->boardOld = nil;
	this->xPieceOld = this->yPieceOld = -1;
}

WindowDisplay::~WindowDisplay()
{
	if (memoryOld)
		delete(memoryOld);
	memoryOld = nil;
	
	if (boardOld)
		delete(boardOld);
	boardOld = nil;

}

void WindowDisplay::Init(Board * board, MemoryClass * memory)
{
	Display::Init(board, memory);
	WindowDisplay::~WindowDisplay();	// hope this works
	this->xPieceOld = this->yPieceOld = -1;
	Update(0, 0);
	UpdateMemory();
}

void WindowDisplay::Pause(void)
{
	long delay;
//	Delay(&delay, kDelay);
	// option means pause
	while (KeyIsDown(0x3A))  {
	}
	;
		
}

#define FIND_X(x)	kLeft + ((x) * kWidth)
#define FIND_Y(y)	kTop + ((y) * kHeight)

void WindowDisplay::Update( unsigned long time, long score)
{
	Str32 st;
	Rect	rect;
	short	i, j;
	short	incX, incY;
	GrafPtr	port;
	Boolean	fDrawing;
	
	BringToFront(wind);
	
	GetPort(&port);
	SetPort(wind);
	
	// deal with update events
	if (!EmptyRgn(((WindowPeek)wind)->updateRgn))
	{
		if (boardOld) delete(boardOld);
		boardOld = nil;
		ValidRgn(((WindowPeek)wind)->updateRgn);
	}
	
	//	print time
	SetRect( &rect, 10, 0, kLeft, 60 );
	EraseRect( &rect );
	
	MoveTo( 10, 20 );
	NumToString( time, st );
	DrawString( st );

	MoveTo( 10, 40 );
	NumToString( score, st );
	DrawString( st );
	
#if 0
	/*
		Instead of looping through sequentially and drawing
		every box, I build a rectangle of same-color squares
		and then draw it all at once.
	*/
	for (i= 0; i < board->xMax; ++i)
	{
		SetRect(&rect, FIND_X(i), FIND_Y(0),
			FIND_X(i+1), FIND_Y(0) );
		fDrawing = TRUE;
		
		for (j = 0; j <= board->yMax; ++j)	// draw the last row
		{
			if ((j == board->yMax) || (board->GetBit(i, j) != fDrawing))
			{
				// draw what we've got
				if (fDrawing)
					FillRect(&rect, gray);
				else
					EraseRect(&rect);
					
				fDrawing = !fDrawing;
				
				rect.top = FIND_Y(j);
				
			}
			rect.bottom += kHeight;
		}
	}
#else
//	New method: only draw squares that have changed since the last update
	for (int y=0; y<board->yMax; ++y)
		for(int x=0; x<board->xMax; ++x)
		{
			if ((boardOld == nil) || (board->GetBit(x,y) != boardOld->GetBit(x,y)))
			{
				DrawPixel( x, y );
			}
		}

	if (boardOld) delete(boardOld);
	boardOld = board->Copy();
	
#endif	

	//	Erase old piece
	if (xPieceOld >=0 || yPieceOld >= 0)
	{
		int left = FIND_X(xPieceOld);
		int top =  FIND_Y(yPieceOld);
		SetRect(&rect, left, top, left + 4*kWidth, top + 4*kHeight);
		EraseRect(&rect);
		for (int x=xPieceOld; x<xPieceOld+4; ++x)		// put back proper squares
			for (int y=yPieceOld; y<yPieceOld+4; ++y)
				DrawPixel(x, y);
	}
	
	// Draw new piece
	if (board->piece != nil)
	{
		short color = thePort->fgColor;
	 	ForeColor( redColor );
		
		for (i=0; i<4; ++i)
			for (j=0; j<4; ++j)
				if (board->piece->GetBit(i,j))
				{
					int left = FIND_X(board->xPiece + i);
					int top = FIND_Y(board->yPiece + j);
					SetRect( &rect, left, top, left + kWidth, top + kHeight );
					FillRect( &rect, gray );
				}	
		ForeColor( color );	  

		xPieceOld = board->xPiece;
		yPieceOld = board->yPiece;
	}
	else
		xPieceOld = yPieceOld = -1;

//	SetPort(port);

	UpdateSensor(sensorX, sensorY, TRUE);	// since it gets overdrawn, force-draw it now

	Pause();
}

void WindowDisplay::DrawPixel(int x, int y)
{
	Rect rect;
	SetRect(&rect, FIND_X(x) + 1, FIND_Y(y) + 1,
		FIND_X(x+1), FIND_Y(y+1) );
	static PatPtr rgPat[] =  {
		&white, &gray 
	};
	PatPtr ppat;
	if (x==0 || x==board->xMax-1 || y==board->yMax-1)	// border is special
		ppat = &black;
	else
		ppat = rgPat[board->GetBit(x,y)];
	FillRect(&rect, *ppat);
}


void WindowDisplay::Flash( int x, int y )
{
	Rect rect;
	long ticks;
	
	SetRect(&rect, FIND_X(x), FIND_Y(y),
			FIND_X(x+1), FIND_Y(y+1) );

	FlashRect(&rect);
}

void WindowDisplay::FlashRect(Rect * pr)
{
	long ticks;
	
	BitClr((void*)0x938, 0);	// set hilite mode
	InvertRect( pr );
	Delay(kDelay, &ticks);
	
	Pause();
	
#if 1		/* flash or leave on screen	*/
	HiliteMode &= ~hiliteBit;
	InvertRect( pr );
#endif
}

void WindowDisplay::UpdateSensor( int x, int y, int fForce )
{
	GrafPtr port;
	Rect rect;
	
	#define SetSensorRect(pr, X, Y)  \
		SetRect(pr, FIND_X(X), FIND_Y(Y-1), FIND_X(X+2)+1, FIND_Y(Y+1)+1)
	
	if (sensorX != x || sensorY != y || fForce)
	{
		GetPort(&port);
		SetPort(wind);

		if (sensorX >= 0 && sensorY >= 0 && !(sensorX == x && sensorY == y))
		{
			PenPat(white);
			SetSensorRect(&rect, sensorX, sensorY);
			FrameRect(&rect);
		}

		sensorX = x;
		sensorY = y;

		if (sensorX >= 0 && sensorY >= 0)
		{
			PenPat(black);
			SetSensorRect(&rect, sensorX, sensorY);
			FrameRect(&rect);
		}
		
		Pause();
	}
}


#define MEM_PER_ROW		8
#define kMemHeight	16
#define kMemWidth	22

#define FIND_MEM_X(mem)	FIND_X(board->xMax) + kMemWidth*(mem % MEM_PER_ROW + 1)
#define FIND_MEM_Y(mem) kTop + kMemHeight * (mem / MEM_PER_ROW)

void WindowDisplay::UpdateMemory(void)
{
	
	if (gParams[param_show_mem] == 0)
		return;

	int mem;
	Str32 st;
	int val;
	Point pt;
	Rect rect;
		
	SetPort(wind);
	
	TextFont(9);
	
	#define SetMemoryRect( pr, mem )	\
		SetRect(pr, FIND_MEM_X(mem), FIND_MEM_Y(mem), FIND_MEM_X(mem)+kMemWidth, FIND_MEM_Y(mem)+kMemHeight )

	for (mem=0; mem<gParams[param_mem_size]; ++mem)
	{
		val = memory->Peek(mem);
		
		if (memoryOld == nil || memoryOld->Peek(mem) != val)
		{
			NumToString(val, st);
			SetPt(&pt, FIND_MEM_X(mem), FIND_MEM_Y(mem));
			SetMemoryRect(&rect,mem); 
			EraseRect(&rect);
			MyDrawString( st, pt, justLeft );
		}		
	}
	if (memoryOld)
		delete(memoryOld);
	memoryOld = memory->Copy();
}

void WindowDisplay::FlashMemory(int mem)
{
	if (gParams[param_show_mem] == 0)
		return;
	Rect rect;
	SetMemoryRect(&rect, mem);
	FlashRect(&rect);
}

#endif

TextDisplay::TextDisplay(void)
{
#ifdef USE_CURSES

#endif
}

void TextDisplay::Init( Board * board, MemoryClass * memory )
{
  Display::Init(board, memory);
  clear();  // clear screen
}

void TextDisplay::Pause(void)
{
  getch();
}

void TextDisplay::Update( unsigned long time, long score)
{
    int xPos, yPos, x;

#ifdef MACINTOSH
	cgotoxy(1,1,stdout);
	ccleos(stdout);
#else
//  printf("\033[H\033[2J"); // Clear screen, but not if curses...
#endif

#ifdef USE_CURSES		// Eric broke textdisplay via curses
//    printf("test\n");

//    clear();
//    erase();

    erase();

    move(0,0);

    printw("     ** TETRIS **\n");
    printw("By Eric Siegel and Alex Chaffee\n");

    printw("Time: %3d  Score: %3d\n\n",time, score);
    printw("Sensor at: (%2d,%2d)\n",gSensorX,gSensorY);

    //  Sensor : "X" (its lower-left position)
    //  Falling Piece:  "O"
    //  Settled Piece:  "O"
    for(yPos=0; yPos < board->yMax; yPos++) {
        for(xPos=0; xPos < board->xMax; xPos++) {
	    move(yPos + 5, xPos+4);

	    if (((xPos == gSensorX+1) || (xPos == gSensorX)) && 
		((yPos == gSensorY-1) || (yPos == gSensorY)))
	        addch('X');
	    else 
	    if (board->piece->GetBit(xPos - board->xPiece,
					  yPos - board->yPiece))
	        addch('O');
	    else if (board->GetBit( xPos,  yPos ))
	        addch('O');
	    else
	        addch(' ');
	}

    }

    move(30, 0);
    addch('\n');
    
    refresh();

#endif // MACINTOSH
}


const int mem_per_line = 8;
void TextDisplay::UpdateMemory(void)
{
#ifdef USE_CURSES
  move(4,30);
    
  ASSERT(memory);

  printw("Memory array:\n");
  if(gParams[param_show_mem]) {
    
    int mem_size = gParams[param_mem_size];
    
    for(int x=0; x < mem_size  ; x++) {       // Show the dude's memory
      move(5 + (x / mem_per_line), 30 + (x % mem_per_line)*4);
      printw("%4d ",memory->Peek(x));
      
    }
  }

  move(30, 0);
  addch('\n');
  
  refresh();   // update that screen NOW!!!

#endif	// MACINTOSH
}

void TextDisplay::Flash(int x, int y)
{
#ifdef USE_CURSES
  move(y + 5, x + 4);
  addch('*');
  refresh();
#endif
}

/* EOF */

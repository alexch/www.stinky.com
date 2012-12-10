#ifndef _H_DISPLAY
#define _H_DISPLAY

#include "Board.h"
#include "MemoryClass.h"

#include "TetrisParams.h"

class Display {

public:

	Display();
	
	virtual void Init( Board * board, MemoryClass * memory );
	
	virtual void Update( unsigned long time, long score );
	virtual void UpdateMemory( void );
	virtual void UpdateSensor( int x, int y, int fForce = FALSE );

	virtual void Flash( int x, int y );
	virtual void FlashMemory(int mem);
	
protected:
	int sensorX, sensorY;
	Board * board;
	MemoryClass * memory;

	virtual void Pause(void);
	
};

class TextDisplay : public Display {

 public:

    TextDisplay(void);

    virtual void Init( Board * board, MemoryClass * memory );

    virtual void Update( unsigned long time, long score );
    virtual void UpdateMemory( void );

    virtual void Flash( int x, int y );
    //	virtual void FlashMemory(int mem);

  protected:
	virtual void Pause(void);
};


#ifdef MACINTOSH

class WindowDisplay : public Display {

public:

	WindowDisplay( WindowPtr wind );
	~WindowDisplay();
	
	virtual void Init( Board * board, MemoryClass * memory );
	
	virtual void Update( unsigned long time, long score );
	virtual void UpdateMemory();
	virtual void UpdateSensor( int x, int y, int fForce = FALSE );
	virtual void Flash( int x, int y );
	virtual void FlashMemory(int mem);

protected:

	WindowPtr	wind;
	
	Board * boardOld;
	MemoryClass *memoryOld;
	int xPieceOld, yPieceOld;

	virtual void Pause(void);
	void FlashRect(Rect *pr);
	void DrawPixel(int x, int y);
};

#endif //macintosh

#endif

/* EOF */

/*
	MacUtilities
	
	Based on TBUtilities (c) Symantec
*/
 
#include "MacUtilities.h"
#include <Script.h>

#include <stdlib.h>
#include <assert.h>
#define GetResourceCanFail	GetResource
#define FailNILRes(x)		 { if (x == nil) assert(false); }

#define APPLE_MENU	1

/******************************************************************************
 KeyIsDown
 
		Determine whether or not the specified key is being pressed. Keys
		are specified by hardware-specific key code (NOT the character).
 ******************************************************************************/

Boolean KeyIsDown( short theKeyCode)
{
	KeyMap			theKeys;
	
	GetKeys( theKeys);					/* Get state of each key			*/
										
		/* Ordering of bits in a KeyMap is truly bizarre. A KeyMap is a	*/
		/* 16-byte (128 bits) array where each bit specifies the start	*/
		/* of a key (0 = up, 1 = down). We isolate the bit for the		*/
		/* specified key code by first determining the byte position in	*/
		/* the KeyMap and then the bit position within that byte.		*/
		/* Key codes 0-7 are in the first byte (offset 0 from the		*/
		/* start), codes 8-15 are in the second, etc. The BitTst() trap	*/
		/* counts bits starting from the high-order bit of the byte.	*/
		/* For example, for key code 58 (the option key), we look at	*/
		/* the 8th byte (7 offset from the first byte) and the 5th bit	*/
		/* within that byte.											*/
		
	return( BitTst( ((char*) &theKeys) + theKeyCode / 8,
					(long) 7 - (theKeyCode % 8) ) );
}

/******************************************************************************
 AbortInQueue			TCL 1.1.1 DLP 9/18/91
 
 	Walk the event queue, return TRUE if a Command-'.' is there. The
 	event is removed from the queue.
 	
 ******************************************************************************/

Boolean	AbortInQueue( void)
{
	EvQElPtr	qEvt = (EvQElPtr) GetEvQHdr()->qHead;
	Boolean		foundAbort = FALSE;

	while (qEvt)
	{
			// Starting at the evtQWhat field, and EvQEl is identical
			// to an EventRecord
			
		if ( IsCancelEvent( (EventRecord*) &qEvt->evtQWhat))
		{
			// Flush this and all other key-downs from the queue
			FlushEvents( keyDownMask, 0);
			foundAbort = TRUE;
			break;
		}
		qEvt = (EvQElPtr) qEvt->qLink;
	}
	return foundAbort;
}

/******************************************************************************
 IsCancelEvent
 
 	Returns TRUE if event is a command-period keydown event. Uses 
 	the technique in TN #263 to handle international keyboards.
 	
 ******************************************************************************/


#define kMaskModifiers  0xFE00     // we need the modifiers without the
                                   // command key for KeyTrans
                        
#define kUpKeyMask      0x0080
#define kMaskASCII1     0x00FF0000 // get the key out of the ASCII1 byte
#define kMaskASCII2     0x000000FF // get the key out of the ASCII2 byte

Boolean IsCancelEvent( EventRecord *theEvent )
{
	Boolean  isCancel = FALSE;
	short    keyCode;
	long     virtualKey, keyInfo, lowChar, highChar, state, keyCId;
	Handle   hKCHR = NULL;
	Ptr 	 KCHRPtr;

	if ((theEvent->what == keyDown) || (theEvent->what == autoKey))
	{
  
		// see if the command key is down.  If it is, find out the ASCII
  		// equivalent for the accompanying key.

		if (theEvent->modifiers & cmdKey) 
		{
				// get virtual key and keycode for KeyTrans
				
			virtualKey = (theEvent->message & keyCodeMask) >> 8;
	    
		    // and out the command key and or in the virtualKey
		    
		    keyCode = (theEvent->modifiers & kMaskModifiers)  |  virtualKey;
		   
		   	// make it look like a keyup event, to prevent dead key processing
		   	
		   	keyCode |= kUpKeyMask;
		    
		    KCHRPtr = (Ptr) GetEnvirons( smKCHRCache);
		
		    if (!KCHRPtr) 
		    {
				keyCId = GetScript( GetEnvirons(smKeyScript), smScriptKeys );
				hKCHR = GetResourceCanFail( 'KCHR', keyCId );	// TCL 1.1.3 11/30/92 BF
				FailNILRes(hKCHR);
				KCHRPtr = *hKCHR;
			}
			
			if (KCHRPtr) 
			{
				state = 0;
				keyInfo = KeyTrans( KCHRPtr, keyCode, &state);
				if (hKCHR)
					ReleaseResource(hKCHR);
			}
			else
				keyInfo = theEvent->message;
		
		    lowChar = keyInfo &  kMaskASCII2;
		    highChar = (keyInfo & kMaskASCII1) >> 16;
		    if (lowChar == '.' || highChar == '.')
				isCancel = true;
		}
	}
	return isCancel;
}

/*
	Macintosh Toolbox function
	returns 0 if everything's ok
*/
int Event(void)
{
	EventRecord event;
	WindowPtr	whichWindow;
	Str255	daName;
	long	mr;	/* MenuSelect() return code */
	GrafPtr	savePort;
	short	mask = everyEvent - (updateMask|activMask|diskMask|keyUpMask);

#if 0
/*	
	right now, just checks for an abort
*/
	if (AbortInQueue() || Button())
		exit(1);
	else
		return;
#else
		
//	if (WNEIsImplemented())
		WaitNextEvent( mask, &event, 10, nil );
//	else
//	{
//		SystemTask();
//		GetNextEvent( mask, &event);
//	}
	
	if (SystemEvent(&event))
		return;
		
	if ( event.what == mouseDown ) {
		switch ( FindWindow( event.where,  &whichWindow ) ) {

			/* check for mouseDown events in other areas */

		case inSysWindow:
			SystemClick( &event, whichWindow );
			break;

		case inMenuBar:
			mr = MenuSelect( event.where );	/* user interaction */
			if ( HiWord( mr ) == APPLE_MENU ) {	/* in apple menu? */

				GetItem( GetMHandle(APPLE_MENU),  LoWord(mr), daName );
									/* get DA name */
				GetPort( &savePort );	/* a precaution */

				OpenDeskAcc( daName );	/* activate the DA */

				SetPort( savePort );	/* some insurance */

			}
			break;

		case inContent:
			// pause for click in content region
			while (Button());
			
		default:
			break;
			
		}
	}
#endif
	
//	if (IsCancelEvent(&event))
//		exit(0);
}

/*
	MyDrawString
	
	Default: draws string with pt at the top-left of a box surrounding the string
	
	But if you want, set just and it'll offset the string accordingly
*/
void MyDrawString( unsigned char * st, Point pt, Just just )
{
	CalcDrawString( st, just, &pt, nil, nil );
	MoveTo( pt.h, pt.v );
	DrawString( st );
}

void CalcDrawString( unsigned char * st, Just just, Point * ppt, short * pwidth, short * pheight )
{
	Point pt = *ppt;
	FontInfo info;
	int height, width;
	
	GetFontInfo( &info );
	height = info.ascent + info.descent;
	width = StringWidth( st );
	
	if (just & justRight)
		pt.h -= width;
	else if (just & justHorizCenter)
		pt.h -= width / 2;

	// default: justLeft
	
	pt.v += info.ascent;
	if (just & justBottom)
		pt.v -= height;
	else if (just & justVertCenter)
		pt.v -= height/2;
	
	// default: justTop
	
	*ppt = pt;
	if (*pheight) *pheight = height;
	if (*pwidth) *pwidth = width;
}


/* EOF */

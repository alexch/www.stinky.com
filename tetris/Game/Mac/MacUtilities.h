/*
	Types and Constants
*/
typedef enum {
	justRight = 	0x01,
	justLeft =		0x02,
	justHorizCenter =	0x04,
	justTop =		0x08,
	justBottom =	0x10,
	justVertCenter =0x20
} Just;

#ifdef __cplusplus
extern "C"  {
#endif
Boolean KeyIsDown( short theKeyCode);
Boolean	AbortInQueue( void);
Boolean IsCancelEvent( EventRecord *theEvent );
int Event(void);
void MyDrawString( unsigned char * st, Point pt, Just just );
void CalcDrawString( unsigned char * st, Just just, Point * ppt, short * pwidth, short * pheight );
#ifdef __cplusplus
};
#endif

#undef ASSERT
/* EOF */

/*
	macintosh.h
	
	header for macintosh port of SGPC
*/
#include <stdio.h>
#include <limits.h>
#include <float.h>

/*
	If you don't want to change this file,
	define these in the prefix; the prefix will
	take precedence over these.
*/
#ifndef PROBLEM
	#define PROBLEM	TETRIS
#endif
#ifndef TYPE
	#define TYPE int
#endif
#ifndef DEBUG
	#define DEBUG	0
#endif


#define MACINTOSH
#define ANSI_FUNC
#define COMPRESS	""
#define STD_QSORT
#define REP_ON_GEN	1

/*	port of values.h names for limits.h and float.h	*/
#define MININT	INT_MIN
#define MAXINT	INT_MAX
#define MAXUINT	UINT_MAX
#define MAXFLOAT	FLT_MAX
#define MINFLOAT	FLT_MIN
#define MAXLDBL	LDBL_MAX

#ifndef TRUE
#define TRUE (1)
#endif
#ifndef FALSE
#define FALSE (0)
#endif
#define PI 3.14159265358979323846
#define M_PI	PI		/* is this right? */

#include <stdlib.h>		/* define malloc	*/
#define malloc(x)	safe_malloc((size_t)(x))	/* redefine malloc	*/

/*---*/

#ifdef __cplusplus
extern "C"  {
#endif

int strcasecmp( const char * s1, const char * s2 );
int gethostid(void);
int getpid(void);
int isinf(TYPE num);
int isnan(TYPE num);
void setbuffer(FILE *stream, char *buf, int size);
void * safe_malloc(size_t size);

#ifdef __cplusplus
};
#endif

/* EOF */

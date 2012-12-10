
/*
 *  assert.h
 *
 *  Copyright (c) 1992 Symantec Corporation.  All rights reserved.
 *
 */

#ifdef __cplusplus
extern "C" {
#endif

#undef assert

#ifdef NDEBUG

	#define assert(x)	((void) 0)

#else

	int __assert(char *, char *, int);
//	#define assert(x)	((void) ((x) || __assert(#x, __FILE__, __LINE__)))
	#define assert(x)	((void) ((x) || (Debugger(), __assert(#x, __FILE__, __LINE__))))
//	#define assert(x) { if (!(x)) { Debugger(); __assert(#x, __FILE__, __LINE__); } }

#endif

#ifdef __cplusplus
}
#endif

/* EOF */

/*
 *  assert.h
 *
 */


#undef assert

#ifdef NDEBUG

#define assert(x)       ((void) 0)

#else

#ifdef __cplusplus
extern "C" {
#endif

int XXassert(char *, char *, int);
#define assert(x) ((x) || XXassert(NULL, __FILE__, __LINE__))

#ifdef __cplusplus
}
#endif

#endif




/* EOF */

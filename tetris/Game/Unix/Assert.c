/*
 *  assert.c
 *
 *  Copyright (c) 1991 Symantec Corporation.  All rights reserved.
 *
 */

#include "Assert.h"
#include "stdio.h"
/*
#include "stdlib.h"
*/

int 
XXassert(char *expr, char *file, int line)
{
        fprintf(stderr, "Assertion failed: %s, file %s, line %d\n", expr, file, line);
        abort();
	return(1);
}


/* EOF */

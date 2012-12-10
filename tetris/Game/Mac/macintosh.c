/*
	Mac versions of UNIX library routines
*/

#include <stdio.h>
#include <ctype.h>		/*	for tolower	*/
#include <console.h>
#include <string.h>

#include "macintosh.h"

/*
	library functions
*/		
strcasecmp(const char *s1, const char *s2)
{
	int ch;
	while (*s1)
	{
		ch = tolower(*s1) - tolower(*s2);
		if (ch)
			return ch;
		++s1, ++s2;
	}
	return 0;
}

int gethostid(void)
{
	return 1;
}

static int pid = 0;

int getpid(void)
{
	if (pid == 0)
		// assign a random PID
		pid = TickCount();
	return pid;
}

int isinf(TYPE num)
{
	char s[256];
	if (sizeof(TYPE) == sizeof(long double))
		sprintf( s, "%Lf", (long double)num );
	else
		sprintf( s, "%f", (float)num  );
	return !strcmp(s, "INF");
}

int isnan(TYPE num)
{
	char s[256];
	if (sizeof(TYPE) == sizeof(long double))
		sprintf( s, "%Lf", (long double)num );
	else
		sprintf( s, "%f", (float)num );
	return !strcmp(s, "NAN");
}

void setbuffer(FILE *stream, char *buf, int size)
{
	if ( size == BUFSIZ 
		&& buf != NULL)
		setbuf(stream, buf);
}

#undef malloc
void * safe_malloc(size_t size)
{
	void * ptr = malloc(size);
	if (ptr == NULL)
	{
		fprintf(stderr, "Error: couldn't allocate %ld bytes\n", (long)size);
		exit(1);
	}
	return ptr;
}

/* EOF */

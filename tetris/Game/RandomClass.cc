#include "RandomClass.h"

const long m = 	100000000;
const long m1 = 10000;
const long b = 31415821;

RandomClass::RandomClass(long seed)
{
	// 19-Jan-95	Only use positive seeds
	a = seed >= 0 ? seed : 1L - seed;
}

RandomClass::~RandomClass(void)
{
}

/*
	multiplication w/o overflow
*/
long RandomClass::mult(long p, long q)
{
	long p1, p0, q1, q0;
	p1 = p/m1; p0 = p%m1;	 
	q1 = q/m1; q0 = q%m1;
	return (((p0*q1 + p1*q0) % m1) * m1 + p0*q0) % m;
}

long RandomClass::next(void)
{
	a = (mult(a,b) + 1) % m;
	return a;
}

long RandomClass::next(long max)
{
	a = next();
	return ((a/m1)*max)/m1;
}

/* EOF */

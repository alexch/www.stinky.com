/*
	RandomClass.h
	
	Maintains a random number generator.
	Based on Sedgewick's _Algorithms in C++_
	
	Usage:	
		RandomClass r(seed);
		long x = r.next();
*/

class RandomClass {

 public:
//private:
	long a;
 private:
	long mult(long p, long q);

public:
	RandomClass(long seed);
	~RandomClass(void);
	long next(void);
	long next(long max);
};


/* EOF */

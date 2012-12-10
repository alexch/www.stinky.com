//#pragma once
#ifndef _H_MEMORYCLASS
#define _H_MEMORYCLASS

#define MaxMemSize 1024

class MemoryClass {
public:
	MemoryClass(int size);
	~MemoryClass(void);

	int GetSize(void);
	
	int Peek(int i);
	int Poke(int i, int val);
	
	void Clear(void);
	// operator[] ??
	
	MemoryClass * Copy(void);

private:
	int size;
	int * data;
};


#endif

/* EOF */

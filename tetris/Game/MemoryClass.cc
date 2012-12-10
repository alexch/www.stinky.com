#include "MemoryClass.h"
#ifdef MACINTOSH
#include <stdlib.h>
#else
#include <stdlib.h>
#endif
#include "assert.h"
#define ASSERT assert

MemoryClass::MemoryClass(int size)
{
	ASSERT(size >= 0 && size < MaxMemSize);
	this->size = size;
	data = (int*) malloc(sizeof(int) * size);
}

MemoryClass::~MemoryClass(void)
{
	free(data);
}

MemoryClass * MemoryClass::Copy(void)
{
	MemoryClass * mem = new MemoryClass(size);
	memcpy(mem->data, this->data, size * sizeof(int));
	return mem;
}

int MemoryClass::Peek(int i)
{
	ASSERT( i >= 0 && i < size );
	return data[i];
}

int MemoryClass::Poke(int i, int val)
{
	int valOld = Peek(i);
	data[i] = val;
	return valOld;
}

void MemoryClass::Clear(void)
{
	int i;
	for (i=0; i<size; ++i)
		data[i] = 0;
}

int MemoryClass::GetSize(void)
{
	return size;
}

/* EOF */

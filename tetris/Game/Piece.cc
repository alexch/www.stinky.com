/*
	Piece.cp
*/

#ifndef MACINTOSH
#include "Unix/config.h"
#endif

#include "Piece.h"	
#include "Assert.h"
#define ASSERT assert

/*
	A piece is just a lookup into this global data.
*/


unsigned short pieces[kPieces][kPhases];


#ifdef UNIS
char piecesRaw[kPieces][kPhases][4][5] = 
{
	{
		// UNIS
		{
			"----",
			"----",
			"----",
			"*---"
		},
		{
			"----",
			"----",
			"----",
			"*---"
		},
		{
			"----",
			"----",
			"----",
			"*---"
		},
		{
			"----",
			"----",
			"----",
			"*---"
		}
	}
};

#else
#ifdef DITRIS

char piecesRaw[kPieces][kPhases][4][5] = 
{
	{
		// DITRIS  (tritrus -- writeme)
		{
			"----",
			"----",
			"*---",
			"**--"
		},
		{
			"----",
			"----",
			"-*--",
			"**--"
		},
		{
			"----",
			"----",
			"**--",
			"-*--"
		},
		{
			"----",
			"----",
			"**--",
			"*---"
		}
	},
	{       // The diff between the two pieces is their init. config.
		{
			"----",
			"----",
			"**--",
			"-*--"
		},
		{
			"----",
			"----",
			"**--",
			"*---"
		},
		{
			"----",
			"----",
			"*---",
			"**--"
		},
		{
			"----",
			"----",
			"-*--",
			"**--"
		}
        }
};


#else

char piecesRaw[kPieces][kPhases][4][5] = 
{
	{
		//	Brick
		{
			"----",
			"----",
			"**--",
			"**--"
		},
		{
			"----",
			"----",
			"**--",
			"**--"
		},
		{
			"----",
			"----",
			"**--",
			"**--"
		},
		{
			"----",
			"----",
			"**--",
			"**--"
		}
	},
	{
		//	I need a long one, I need a long one!
		{
			"-*--",
			"-*--",
			"-*--",
			"-*--"
		},
		{
			"----",
			"----",
			"****",
			"----"
		},
		{
			"-*--",
			"-*--",
			"-*--",
			"-*--"
		},
		{
			"----",
			"----",
			"****",
			"----"
		}
	},
	{
		//	Left-handed squiggly
		{
			"----",
			"*---",
			"**--",
			"-*--"
		},
		{
			"----",
			"----",
			"-**-",
			"**--"
		},
		{
			"----",
			"*---",
			"**--",
			"-*--"
		},
		{
			"----",
			"----",
			"-**-",
			"**--"
		}
	},
	{
		//	Right-handed squiggly
		{
			"----",
			"-*--",
			"**--",
			"*---"
		},
		{
			"----",
			"----",
			"**--",
			"-**-"
		},
		{
			"----",
			"-*--",
			"**--",
			"*---"
		},
		{
			"----",
			"----",
			"**--",
			"-**-"
		}
	},
	{
		//	Mister T
		{
			"----",
			"----",
			"***-",
			"-*--"
		},
		{
			"----",
			"-*--",
			"-**-",
			"-*--"
		}
		,
		{
			"----",
			"-*--",
			"***-",
			"----"
		}
		,
		{
			"----",
			"-*--",
			"**--",
			"-*--"
		}
	},
	{
		//	Mister L
		{
			"----",
			"-*--",
			"-*--",
			"-**-"
		}
		,
		{
			"----",
			"--*-",
			"***-",
			"----"
		}
		, {
			"----",
			"**--",
			"-*--",
			"-*--"
		}
		, {
			"----",
			"----",
			"***-",
			"*---"
		}
	},
	{
		//	Mister J
		{
			"----",
			"-*--",
			"-*--",
			"**--"
		}
		,
		{
			"----",
			"----",
			"***-",
			"--*-"
		}
		, {
			"----",
			"-**-",
			"-*--",
			"-*--"
		}
		, {
			"----",
			"*---",
			"***-",
			"----"
		}
	}
};

#endif
#endif

static Boolean fPiecesInited = FALSE;

static unsigned char StringToBits( char *pch );

void InitPieces(void)
{
	int i, j, k;
	unsigned short bits;
	
#if 0
#ifdef UNIS
	printf("\nThis is Unis (not Tetris)\n\n");
#else
#ifdef DITRIS
	printf("\nThis is Ditris (not Tetris)\n\n");
#else
	printf("\nThis is Tetris (not Unis)\n\n");
#endif
#endif
#endif
	for (i=0; i<kPieces; ++i)
	{
		for (j = 0; j < kPhases; ++j)
		{
			bits = 0;
			for (k=3 ; k >= 0 ; --k )

			{
				bits <<= 4;
				bits |= StringToBits( piecesRaw[i][j][k] ) ;
			}
			pieces[i][j] = bits;
		}
	}
	fPiecesInited = TRUE;
}

static unsigned char StringToBits( char *pch )
{
	int i;
	unsigned char ch = 0;
	
	for(i = 3; i >= 0; --i)
	{
		ch <<= 1;
		if (pch[i] == '*')
			ch |= 1;
	}

	return ch;
}

Piece::Piece(int seed)
{
	r = new RandomClass(seed);
}


Piece::~Piece(void)
{
        ASSERT(r);
	delete r;
}

void Piece::SetRandom(void)
{
	base = r->next(kPieces);
	phase = 0;
	if (!fPiecesInited)
		InitPieces();
}

unsigned short Piece::GetBits(void)
{
	return pieces[base][phase];
}

Boolean Piece::GetBit(int x, int y)
{
	unsigned short bits;

	if (x >= 4 || y >= 4 || x < 0 || y < 0)
		return FALSE;
	bits = GetBits();
	return ( (bits >> (y * 4)) & (1 << x) ) ? TRUE : FALSE;
}


int Piece::GetBase(void)
{
        return ( base );
}


int Piece::GetPhase(void)
{
        return ( phase );
}


void Piece::Rotate(void)
{
	phase += 1;
	if (phase == kPhases)
		phase = 0;
}

void Piece::Unrotate(void)
{
	phase -= 1;
	if (phase < 0)
		phase = kPhases-1;
}

void Piece::Write(ostream& str)
{
  str << ' ' << base << ' ' << phase;
  str << ' ' << r->a;
}

void Piece::Read(istream& str)
{
  str >> ws >> base >> phase;
  str >> r->a;
}

/* EOF */

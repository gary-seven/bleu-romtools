//
// ROM decoding and encoding routines
//
//
#include "drivers.h"
#include "age.h"


int GetBit(long startbit)
{
   long nbyte = startbit/8;
   int nbit = startbit%8;
   int mask = 0x80 >> nbit;

   if (!ROMSet)
       return 0;
   return ((ROMSet[nbyte] & mask) == mask);
}


void Decode(struct GFXINFO *gi)
{
	int x, y, cx, cy, plane;
	BYTE result = 0;
	int Count = 0;
	struct GFXLAYOUT * gl = gi->GfxLayout;
	long StartBit = gi->StartAddress * 8;

	int NumColumns = (32 / (gl->height / 8));

	for (cy = 0; cy < gl->total / NumColumns; cy++)
	{
		for (cx = 0; cx < (256/gl->width); cx++)
		{
			for (y = gl->height - 1; y >= 0; y--)
			{
				for (x = 0; x < gl->width; x++)
				{
					result = 0;
					for (plane = 0; plane < gl->planes; plane++)
					{
						if (GetBit(StartBit +
									  gl->xoffset[x] +
									  gl->yoffset[gl->height - 1 - y] +
									  gl->charincrement * (cx + NumColumns * cy) +
									  gl->planeoffset[plane]))
							result ^= (1 << (gl->planes - 1 - plane));
					}
					DecodedSet[Count] = result;
					Count++;
				}
			}
		}
	}
}





void PutBit(long startbit, int value)
{
	long nbyte = startbit/8;
	int nbit = startbit%8;
	int mask = 0x80 >> nbit;

	if (!ROMSet)
	    return;

	ROMSet[nbyte] &= ~mask;

	if (value != 0)
		ROMSet[nbyte] |= mask;
}



void Encode(struct GFXINFO *gi)
{
	int x, y, cx, cy, plane;
	int Count = 0;
	struct GFXLAYOUT * gl = gi->GfxLayout;
	long StartBit = gi->StartAddress * 8;

	int NumColumns = (32 / (gl->height / 8));

	for (cy = 0; cy < gl->total / NumColumns; cy++)
	{
		for (cx = 0; cx < (256/gl->width); cx++)
		{
			for (y = gl->height - 1; y >= 0; y--)
			{
				for (x = 0; x < gl->width; x++)
				{
					for (plane = 0; plane < gl->planes; plane++)
					{
						PutBit(StartBit +
								 gl->xoffset[x] +
								 gl->yoffset[gl->height - 1 - y] +
								 gl->charincrement * (cx + NumColumns * cy) +
								 gl->planeoffset[plane],
								 DecodedSet[Count] & (1 << (gl->planes - 1 - plane)));
					}
					Count++;
				}
			}
		}
	}
}

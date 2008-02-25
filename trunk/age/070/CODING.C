//
// ROM decoding and encoding routines
//
//
#include <stdio.h>
#include "drivers.h"
#include "age.h"
#include "tools.h"
#include "palette.h"

int GetBit(long startbit)
{
   long nbyte = startbit/8;
   int nbit = startbit%8;
   int mask = 0x80 >> nbit;

   if (!ROMSet)
       return 0;
   return ((ROMSet[nbyte] & mask) == mask);
}


void Decode(int CurrentBank)
{
   struct GFXINFO * gi = &Driver.GfxInfo[CurrentBank];
	int x, y, cx, cy, plane;
	BYTE result = 0;
	int Count = 0;
	struct GFXLAYOUT * gl = gi->GfxLayout;
	long StartBit = gi->StartAddress * 8;
	size_t DecodedSetSize = gl->total * gl->width * gl->height;
	static size_t MaxDecodedSetSize;
   BITMAP * tbmp = create_bitmap(gl->width,gl->height);
   int NumColumns = (32 / (gl->height / 8));

	/* if we didn't allocate space for decoded graphics already,
	 * or the space we have allocated isn't big enough then
	 * allocate space now */
	if (DecodedSet == NULL || DecodedSetSize > MaxDecodedSetSize)
	{
	    /* allocate memory for decoded graphics set */
	    DecodedSet = (BYTE *)realloc(DecodedSet, DecodedSetSize);

	    if (DecodedSet == NULL)
	    {
		printf("Memory Allocation failed\n");
		exit(1);
	    }

	    MaxDecodedSetSize = DecodedSetSize;
	}

	for (cy = 0; cy < gl->total / NumColumns; cy++)
	{
		for (cx = 0; cx < (256/gl->width); cx++)
		{
         // copy the current character into a bitmap
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
               putpixel(tbmp, y, x, result);
            }
			}

         // now change its orientation
         switch(Driver.Orientation)
         {
            case ORIENTATION_FLIP_X:
               tools_flip(tbmp, TOOLS_HORIZ_FLIP);
               break;
            case ORIENTATION_FLIP_Y:
               tools_flip(tbmp, TOOLS_VERT_FLIP);
               break;
            case ORIENTATION_ROTATE_180:
            case ORIENTATION_SWAP_XY:
               tools_cw_rotate(tbmp);
               tools_cw_rotate(tbmp);
               break;
            case ORIENTATION_ROTATE_90:
               tools_cw_rotate(tbmp);
               break;
            case ORIENTATION_ROTATE_270:
               tools_cw_rotate(tbmp);
               tools_cw_rotate(tbmp);
               tools_cw_rotate(tbmp);
               break;
            default:
               break;
         }

         // now copy it to the decoded rom space
         for (y = gl->height - 1; y >= 0; y--)
			{
				for (x = 0; x < gl->width; x++)
            {
					DecodedSet[Count] = getpixel(tbmp, y, x);
					Count++;
            }
         }
		}
	}

   destroy_bitmap(tbmp);
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



void Encode(int CurrentBank)
{
   struct GFXINFO * gi = &Driver.GfxInfo[CurrentBank];
	int x, y, cx, cy, plane;
	int Count = 0;
	struct GFXLAYOUT * gl = gi->GfxLayout;
	long StartBit = gi->StartAddress * 8;
   BITMAP * tbmp = create_bitmap(gl->width,gl->height);
	int NumColumns = (32 / (gl->height / 8));

	for (cy = 0; cy < gl->total / NumColumns; cy++)
	{
		for (cx = 0; cx < (256/gl->width); cx++)
		{
         // copy the decoded rom set to a bitmap so they can be rotated
         // back to their original orientation
         for (y = gl->height - 1; y >= 0; y--)
			{
				for (x = 0; x < gl->width; x++)
            {
               putpixel(tbmp, y, x, DecodedSet[Count]);
					Count++;
            }
         }

         // switch the orientation back to the correct one
         // note we must do the opposite to decode here (this only affects
         // rotate 90 and 270 though)
         switch(Driver.Orientation)
         {
            case ORIENTATION_FLIP_X:
               tools_flip(tbmp, TOOLS_HORIZ_FLIP);
               break;
            case ORIENTATION_FLIP_Y:
               tools_flip(tbmp, TOOLS_VERT_FLIP);
               break;
            case ORIENTATION_ROTATE_180:
            case ORIENTATION_SWAP_XY:
               tools_cw_rotate(tbmp);
               tools_cw_rotate(tbmp);
               break;
            case ORIENTATION_ROTATE_90:
               // rotate 270 to get back to original orientation
               tools_cw_rotate(tbmp);
               tools_cw_rotate(tbmp);
               tools_cw_rotate(tbmp);
               break;
            case ORIENTATION_ROTATE_270:
               // rotate 90 to get back to original orientation
               tools_cw_rotate(tbmp);
               break;
            default:
               break;
         }
         
         // convert the decoded set back to its original rom format
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
                         getpixel(tbmp, y, x) & (1 << (gl->planes - 1 - plane)));
					}
				}
			}
		}
	}
   destroy_bitmap(tbmp);
}

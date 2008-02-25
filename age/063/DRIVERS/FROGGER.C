//
// 
// Driver for frogger
//
//
// See drivers.h for description of fields
//

#include "allegro.h"
#include "..\drivers.h"


struct GFXROMINFO Frogger_RomInfo[] =
{
   { "frogger\\frogger.606", 0x0000, 0x800 },
   { "frogger\\frogger.607", 0x0800, 0x800 }
};


RGB Frogger_Colours[] =
{
   { 0, 0, 0 },
   { 0, 0, 63 },
   { 63, 0, 0 },
   { 63, 63, 63 }
};

struct PALETTE Frogger_Palettes[] =
{
    { 4, Frogger_Colours }
};

struct GFXLAYOUT Frogger_charlayout =
{
	8,8,	/* 8*8 characters */
	256,	/* 256 characters */
	2,	/* 2 bits per pixel */
	{ 0, 256*8*8 },	/* the two bitplanes are separated */
	{ 7*8, 6*8, 5*8, 4*8, 3*8, 2*8, 1*8, 0*8 },
	{ 0, 1, 2, 3, 4, 5, 6, 7 },
	8*8	/* every char takes 8 consecutive bytes */
};

struct GFXLAYOUT Frogger_spritelayout =
{
	16,16,	/* 16*16 sprites */
	64,	/* 64 sprites */
	2,	/* 2 bits per pixel */
	{ 0, 64*16*16 },	/* the two bitplanes are separated */
	{ 23*8, 22*8, 21*8, 20*8, 19*8, 18*8, 17*8, 16*8,
			7*8, 6*8, 5*8, 4*8, 3*8, 2*8, 1*8, 0*8 },
	{ 0, 1, 2, 3, 4, 5, 6, 7,
			8*8+0, 8*8+1, 8*8+2, 8*8+3, 8*8+4, 8*8+5, 8*8+6, 8*8+7 },
	32*8	/* every sprite takes 32 consecutive bytes */
};



struct GFXINFO Frogger_GfxInfo[] = 
{
   { 0x0000, &Frogger_charlayout },
	{ 0x0000, &Frogger_spritelayout }
};


struct GAMEINFO Frogger =
{
   "Frogger",
   2,
   Frogger_RomInfo,
   2,
   Frogger_GfxInfo,
   1,
   Frogger_Palettes,
   0
};




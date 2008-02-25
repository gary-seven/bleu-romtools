//
// 
// Drivers for Scramble, The End and Atlantis
//
//
// See drivers.h for description of fields
//

#include "allegro.h"
#include "..\drivers.h"

struct GFXROMINFO Scramble_RomInfo[] =
{
   { "scramble\\5f", 0x0000, 0x800 },
   { "scramble\\5h", 0x0800, 0x800 }
};

struct GFXROMINFO TheEnd_RomInfo[] =
{
   { "theend\\ic30", 0x0000, 0x800 },
   { "theend\\ic31", 0x0800, 0x800 }
};


struct GFXROMINFO Atlantis_RomInfo[] =
{
   { "atlantis\\5f", 0x0000, 0x800 },
   { "atlantis\\5h", 0x0800, 0x800 }
};



RGB Scramble_Colours[] =
{
   { 0, 0, 0 },
   { 0, 48, 0 },
   { 48, 0, 0 },
   { 48, 48, 48 }
};

struct PALETTE Scramble_Palettes[] =
{
    { 4, Scramble_Colours }
};


struct GFXLAYOUT Scramble_charlayout =
{
	8,8,	/* 8*8 characters */
	256,	/* 256 characters */
	2,	/* 2 bits per pixel */
	{ 0, 256*8*8 },	/* the two bitplanes are separated */
	{ 7*8, 6*8, 5*8, 4*8, 3*8, 2*8, 1*8, 0*8 },
	{ 0, 1, 2, 3, 4, 5, 6, 7 },
	8*8	/* every char takes 8 consecutive bytes */
};

struct GFXLAYOUT Scramble_spritelayout =
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


struct GFXINFO Scramble_GfxInfo[] = 
{
   { 0x0000, &Scramble_charlayout },
   { 0x0000, &Scramble_spritelayout }
};


struct GAMEINFO Scramble =
{
   "Scramble",
   2,
   Scramble_RomInfo,
   2,
   Scramble_GfxInfo,
   1,
   Scramble_Palettes,
   0
};


struct GAMEINFO TheEnd =
{
   "The End",
   2,
   TheEnd_RomInfo,
   2,
   Scramble_GfxInfo,
   1,
   Scramble_Palettes,
   0
};


struct GAMEINFO Atlantis =
{
   "Battle of Atlantis",
   2,
   Atlantis_RomInfo,
   2,
   Scramble_GfxInfo,
   1,
   Scramble_Palettes,
   0
};

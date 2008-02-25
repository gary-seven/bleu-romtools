//
// 
// Drivers for centipede and millipede
//
//
// See drivers.h for description of fields
//

#include "allegro.h"
#include "..\drivers.h"


struct GFXROMINFO Centipede_RomInfo[] =
{
   { "centiped\\centiped.211", 0x0000, 0x800 },
   { "centiped\\centiped.212", 0x0800, 0x800 }
};


RGB Centipede_Colours[] =
{
   { 0, 0, 0 },
   { 0, 48, 0 },
   { 48, 0, 0 },
   { 48, 48, 48 }
};

struct PALETTE Centipede_Palettes[] =
{
    { 4, Centipede_Colours }
};

struct GFXLAYOUT Centipede_charlayout =
{
	8,8,	/* 8*8 characters */
	256,	/* 256 characters */
	2,	/* 2 bits per pixel */
	{ 256*8*8, 0 },	/* the two bitplanes are separated */
	{ 0*8, 1*8, 2*8, 3*8, 4*8, 5*8, 6*8, 7*8 },
	{ 7, 6, 5, 4, 3, 2, 1, 0 },
	8*8	/* every char takes 8 consecutive bytes */
};


struct GFXINFO Centipede_GfxInfo[] = 
{
   { 0x0000, &Centipede_charlayout }
};


struct GAMEINFO Centipede =
{
   "Centipede",
   2,
   Centipede_RomInfo,
   1,
   Centipede_GfxInfo,
   1,
   Centipede_Palettes,
	0
};




//
///////////////////////////////////////////////
//

struct GFXROMINFO Millipede_RomInfo[] =
{
   { "milliped\\milliped.106", 0x0000, 0x800 },
   { "milliped\\milliped.107", 0x0800, 0x800 }
};

struct GAMEINFO Millipede =
{
   "Millipede",
   2,
   Millipede_RomInfo,
   1,
   Centipede_GfxInfo,
   1,
   Centipede_Palettes,
	0
};

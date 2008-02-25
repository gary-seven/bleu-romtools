//
// 
// Drivers for rallyx and new rallyx
//
//
// See drivers.h for description of fields
//

#include "allegro.h"
#include "..\drivers.h"


struct GFXROMINFO RallyX_RomInfo[] =
{
   { "rallyx\\rallyx.8e", 0x0000, 0x1000 },
};


struct GFXROMINFO NewRallyX_RomInfo[] =
{
   { "nrallyx\\nrallyx.8e", 0x0000, 0x1000 },
};


RGB RallyX_Colours[] =
{
   { 0, 0, 0 },
   { 32, 32, 48 },
   { 0, 0, 48 },
   { 48, 48, 0 }
};

struct PALETTE RallyX_Palettes[] =
{
    { 4, RallyX_Colours }
};


struct GFXLAYOUT RallyX_charlayout =
{
	8,8,	/* 8*8 characters */
	256,	/* 256 characters */
	2,	/* 2 bits per pixel */
	{ 0, 4 },	/* the two bitplanes for 4 pixels are packed into one byte */
	{ 8*8+0, 8*8+1, 8*8+2, 8*8+3, 0, 1, 2, 3 },	/* bits are packed in groups of four */
	{ 0*8, 1*8, 2*8, 3*8, 4*8, 5*8, 6*8, 7*8 },
	16*8	/* every char takes 16 bytes */
};

struct GFXLAYOUT RallyX_spritelayout =
{
	16,16,	/* 16*16 sprites */
	64,	/* 64 sprites */
	2,	/* 2 bits per pixel */
	{ 0, 4 },	/* the two bitplanes for 4 pixels are packed into one byte */
	{ 8*8+0, 8*8+1, 8*8+2, 8*8+3, 16*8+0, 16*8+1, 16*8+2, 16*8+3,	/* bits are packed in groups of four */
			 24*8+0, 24*8+1, 24*8+2, 24*8+3, 0, 1, 2, 3 },
	{ 0*8, 1*8, 2*8, 3*8, 4*8, 5*8, 6*8, 7*8,
			32*8, 33*8, 34*8, 35*8, 36*8, 37*8, 38*8, 39*8 },
	64*8	/* every sprite takes 64 bytes */
};


struct GFXINFO RallyX_GfxInfo[] = 
{
   { 0x0000, &RallyX_charlayout },
   { 0x0000, &RallyX_spritelayout }
};


struct GAMEINFO RallyX =
{
   "RallyX",
   1,
   RallyX_RomInfo,
   2,
   RallyX_GfxInfo,
   1,
   RallyX_Palettes,
   0
};


struct GAMEINFO NewRallyX =
{
   "New RallyX",
   1,
   NewRallyX_RomInfo,
   2,
   RallyX_GfxInfo,
   1,
   RallyX_Palettes,
   0
};


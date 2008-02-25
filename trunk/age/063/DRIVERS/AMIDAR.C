//
// 
// Drivers for amidar and turtles
//
//
// See drivers.h for description of fields
//

#include "allegro.h"
#include "..\drivers.h"


struct GFXROMINFO Amidar_RomInfo[] =
{
   { "amidar\\amidarus.5f", 0x0000, 0x800 },
   { "amidar\\amidarus.5h", 0x0800, 0x800 }
};

struct GFXROMINFO AmidarJp_RomInfo[] =
{
   { "amidarjp\\amidar.5f", 0x0000, 0x800 },
   { "amidarjp\\amidar.5h", 0x0800, 0x800 }
};

struct GFXLAYOUT Amidar_charlayout =
{
	8,8,	/* 8*8 characters */
	256,	/* 256 characters */
	2,	/* 2 bits per pixel */
	{ 0, 256*8*8 },	/* the two bitplanes are separated */
	{ 7*8, 6*8, 5*8, 4*8, 3*8, 2*8, 1*8, 0*8 },
	{ 0, 1, 2, 3, 4, 5, 6, 7 },
	8*8	/* every char takes 8 consecutive bytes */
};

struct GFXLAYOUT Amidar_spritelayout =
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


RGB Amidar_Colours[] =
{
   { 0, 0, 0 },
   { 0, 48, 0 },
   { 48, 0, 0 },
   { 48, 48, 48 }
};

struct PALETTE Amidar_Palettes[] =
{
    { 4, Amidar_Colours }
};

struct GFXINFO Amidar_GfxInfo[] = 
{
   { 0x0000, &Amidar_charlayout },
   { 0x0000, &Amidar_spritelayout }
};


struct GAMEINFO Amidar =
{
   "Amidar (US)",
   2,
   Amidar_RomInfo,
   2,
   Amidar_GfxInfo,
   1,
   Amidar_Palettes,
	0
};

struct GAMEINFO AmidarJp =
{
   "Amidar (Jap)",
   2,
   AmidarJp_RomInfo,
   2,
   Amidar_GfxInfo,
   1,
   Amidar_Palettes,
	0
};

// turtles and Turpin
struct GFXROMINFO Turtles_RomInfo[] =
{
   { "turtles\\turt_vid.5f", 0x0000, 0x800 },
   { "turtles\\turt_vid.5h", 0x0800, 0x800 }
};

struct GFXROMINFO Turpin_RomInfo[] =
{
   { "turpin\\c1", 0x0000, 0x800 },
   { "turpin\\c2", 0x0800, 0x800 }
};

struct GAMEINFO Turtles =
{
   "Turtles",
   2,
   Turtles_RomInfo,
   2,
   Amidar_GfxInfo,
   1,
   Amidar_Palettes,
	0
};
struct GAMEINFO Turpin =
{
   "Turpin",
   2,
   Turpin_RomInfo,
   2,
   Amidar_GfxInfo,
   1,
   Amidar_Palettes,
	0
};

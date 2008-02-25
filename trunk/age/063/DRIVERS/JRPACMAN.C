//
// 
// Drivers for JR Pacman
//
//
// See drivers.h for description of fields
//

#include "allegro.h"
#include "..\drivers.h"


struct GFXROMINFO JRPacMan_RomInfo[] =
{
   { "jrpacman\\jrp2c.bin", 0x0000, 0x2000 },
   { "jrpacman\\jrp2e.bin", 0x2000, 0x2000 },
};


struct GFXROMINFO JRPacMan_DataRomInfo[] =
{
   { "jrpacman\\jrp8d.bin", 0x0000, 0x2000 },
   { "jrpacman\\jrp8e.bin", 0x2000, 0x2000 },
   { "jrpacman\\jrp8h.bin", 0x4000, 0x2000 },
   { "jrpacman\\jrp8j.bin", 0x6000, 0x2000 },
   { "jrpacman\\jrp8k.bin", 0x8000, 0x2000 },
};

RGB JRPacMan_Greys[] = // a nice greyscale for jrpacman
{ { 0, 0, 0 }, { 20, 20, 20 }, { 40, 40, 40 }, { 60, 60, 60 } };

// no reason to redefine the pacman colors here, just extern them...
extern RGB PacMan_Colours0[];
extern RGB PacMan_Colours1[];
extern RGB PacMan_Colours2[];
extern RGB PacMan_Colours3[];
extern RGB PacMan_Colours4[];
extern RGB PacMan_Colours5[];
extern RGB PacMan_Colours6[];
extern RGB PacMan_Colours7[];
extern RGB PacMan_Colours8[];
extern RGB PacMan_Colours9[];
extern RGB PacMan_Colours10[];
extern RGB PacMan_Colours11[];

struct PALETTE JRPacMan_Palettes[] =
{
    { 4, JRPacMan_Greys },
    { 4, PacMan_Colours0 },
    { 4, PacMan_Colours1 },
    { 4, PacMan_Colours2 },
    { 4, PacMan_Colours3 },
    { 4, PacMan_Colours4 },
    { 4, PacMan_Colours5 },
    { 4, PacMan_Colours6 },
    { 4, PacMan_Colours7 },
    { 4, PacMan_Colours8 },
    { 4, PacMan_Colours9 },
    { 4, PacMan_Colours10 },
    { 4, PacMan_Colours11 },
};

struct GFXLAYOUT JRPacMan_charlayout =
{
    8,8,	/* 8*8 characters */
    512,	/* 512 characters */
    2,	/* 2 bits per pixel */
    { 0, 4 },	/* the two bitplanes for 4 pixels are packed into one byte */
    { 7*8, 6*8, 5*8, 4*8, 3*8, 2*8, 1*8, 0*8 }, /* characters are rotated 90 degrees */
    { 8*8+0, 8*8+1, 8*8+2, 8*8+3, 0, 1, 2, 3 },	/* bits are packed in groups of four */
    16*8	/* every char takes 16 bytes */
};

struct GFXLAYOUT JRPacMan_spritelayout =
{
    16,16,	/* 16*16 sprites */
    128,	/* 128 sprites */
    2,	/* 2 bits per pixel */
    { 0, 4 },	/* the two bitplanes for 4 pixels are packed into one byte */
    { 39 * 8, 38 * 8, 37 * 8, 36 * 8, 35 * 8, 34 * 8, 33 * 8, 32 * 8,
		    7 * 8, 6 * 8, 5 * 8, 4 * 8, 3 * 8, 2 * 8, 1 * 8, 0 * 8 },
    { 8*8, 8*8+1, 8*8+2, 8*8+3, 16*8+0, 16*8+1, 16*8+2, 16*8+3,
		    24*8+0, 24*8+1, 24*8+2, 24*8+3, 0, 1, 2, 3 },
    64*8	/* every sprite takes 64 bytes */
};


struct GFXLAYOUT JRPacMan_charspritelayout_1 = // for the fruits & guys
{
	16,16,	/* 16*16 sprites */
	128,	/* 64 sprites */
	2,	/* 2 bits per pixel */
	{ 0, 4 },	/* the two bitplanes for 4 pixels are packed into one byte */

	/* aagh!  this is aggrivating! */

	{
	  7*8+128, 6*8+128, 5*8+128, 4*8+128,
	  3*8+128, 2*8+128, 1*8+128, 0*8+128,
	  7*8,     6*8,     5*8,     4*8,
	  3*8,     2*8,     1*8,     0*8,
	}, /* characters are rotated 90 degrees */

	{
	  8*8+0,     8*8+1,     8*8+2,     8*8+3,
	      0,         1,         2,         3,
	  8*8+0+256, 8*8+1+256, 8*8+2+256, 8*8+3+256,
	      0+256,     1+256,     2+256,     3+256,
	},/* bits are packed in groups of four */

	64*8	/* every sprite takes 64 bytes */
};

struct GFXINFO JRPacMan_GfxInfo[] =
{
   { 0x0000, &JRPacMan_charlayout },
   { 0x2000, &JRPacMan_spritelayout },
   { 0x0000, &JRPacMan_charspritelayout_1 },
};


struct ROMDATA JRPacMan_Data =
{
    5,			// 4 more roms to load
    JRPacMan_DataRomInfo, // array of 4 roms
    0,			// one map
    NULL,	// map location
    0,			// number of categories
    NULL,	// string information
    0,		// number of attribs
    NULL,	// attribute information
};


struct GAMEINFO JRPacMan =
{
    "Jr. Pac Man",
   2,
   JRPacMan_RomInfo,
   3,
   JRPacMan_GfxInfo,
   13,
   JRPacMan_Palettes,
   &JRPacMan_Data
};

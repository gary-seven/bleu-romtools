//
// 
// Drivers for Dig Dug
//
//
// See drivers.h for description of fields
//

#include "allegro.h"
#include "..\drivers.h"


struct GFXROMINFO DigDugNM_RomInfo[] =
{
   { "digdugnm\\dd1.9",  0x0000, 0x0800 },
   { "digdugnm\\dd1.11", 0x1000, 0x1000 },
   { "digdugnm\\dd1.15", 0x2000, 0x1000 },
   { "digdugnm\\dd1.14", 0x3000, 0x1000 },
   { "digdugnm\\dd1.13", 0x4000, 0x1000 },
   { "digdugnm\\dd1.12", 0x5000, 0x1000 },
};

struct GFXROMINFO DigDugAT_RomInfo[] =
{
   { "digdugat\\136007.108", 0x0000, 0x0800 },
   { "digdugat\\136007.115", 0x1000, 0x1000 },
   { "digdugat\\136007.116", 0x2000, 0x1000 },
   { "digdugat\\136007.117", 0x3000, 0x1000 },
   { "digdugat\\136007.118", 0x4000, 0x1000 },
   { "digdugat\\136007.119", 0x5000, 0x1000 },
};


RGB DigDug_Greys[] = // a nice greyscale 
{ { 0, 0, 0 }, { 20, 20, 20 }, { 40, 40, 40 }, { 60, 60, 60 } };

struct PALETTE DigDug_Palettes[] =
{
    { 4, DigDug_Greys },
};


struct GFXLAYOUT DDcharlayout1 =
{
	8,8,	/* 8*8 characters */
	128,	/* 128 characters */
	1,		/* 1 bit per pixel */
	{ 0 },	/* one bitplane */
	{ 7*8, 6*8, 5*8, 4*8, 3*8, 2*8, 1*8, 0*8 },
	{ 7, 6, 5, 4, 3, 2, 1, 0 },
	8*8	/* every char takes 8 consecutive bytes */
};

struct GFXLAYOUT DDcharlayout2 =
{
	8,8,	/* 8*8 characters */
	256,	/* 256 characters */
	2,	/* 2 bits per pixel */
	{ 0, 4 },      /* the two bitplanes for 4 pixels are packed into one byte */
	{ 7*8, 6*8, 5*8, 4*8, 3*8, 2*8, 1*8, 0*8 },   /* characters are rotated 90 degrees */
	{ 8*8+0, 8*8+1, 8*8+2, 8*8+3, 0, 1, 2, 3 },   /* bits are packed in groups of four */
	16*8	       /* every char takes 16 bytes */
};

struct GFXLAYOUT DDspritelayout =
{
	16,16,	        /* 16*16 sprites */
	256,	        /* 256 sprites */
	2,	        /* 2 bits per pixel */
	{ 0, 4 },	/* the two bitplanes for 4 pixels are packed into one byte */
	{ 39 * 8, 38 * 8, 37 * 8, 36 * 8, 35 * 8, 34 * 8, 33 * 8, 32 * 8,
			7 * 8, 6 * 8, 5 * 8, 4 * 8, 3 * 8, 2 * 8, 1 * 8, 0 * 8 },
	{ 0, 1, 2, 3, 8*8, 8*8+1, 8*8+2, 8*8+3, 16*8+0, 16*8+1, 16*8+2, 16*8+3,
			24*8+0, 24*8+1, 24*8+2, 24*8+3 },
	64*8	/* every sprite takes 64 bytes */
};

struct GFXINFO DigDug_GfxInfo[] =
{
   { 0x0000, &DDcharlayout1},
   { 0x2000, &DDspritelayout},
   { 0x1000, &DDcharlayout2},
};


struct GAMEINFO DigDugNM =
{
    "Dig Dug (Namco)",
   6,
   DigDugNM_RomInfo,
   3,
   DigDug_GfxInfo,
   1,
   DigDug_Palettes,
   NULL
};

struct GAMEINFO DigDugAT =
{
   "Dig Dug (Atari)",
   6,
   DigDugAT_RomInfo,
   3,
   DigDug_GfxInfo,
   1,
   DigDug_Palettes,
   NULL
};

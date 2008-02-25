//
// 
//
//
// See drivers.h for description of fields
//

#include "allegro.h"
#include "..\drivers.h"


struct GFXROMINFO BRubber_RomInfo[] =
{
   { "BRubber\\BRubber.4h", 0x0000, 0x2000 },
   { "BRubber\\BRubber.4f", 0x2000, 0x2000 },
   { "BRubber\\BRubber.4e", 0x4000, 0x2000 },
   { "BRubber\\BRubber.10e", 0x6000, 0x1000 },
   { "BRubber\\BRubber.10f", 0x7000, 0x1000 }
};


RGB BRubber_Colours[] =
{
#ifndef WRONG_COLORS
	{0x00, 0x00, 0x00}, // these have been adjusted to 0..63
	{0x00, 0x14, 0x00}, // rather than 0..255
	{0x00, 0x25, 0x00},
	{0x1c, 0x18, 0x14}, //  ... but they don't look right
	{0x18, 0x14, 0x10},
	{0x1c, 0x18, 0x14},
	{0x14, 0x10, 0x0b},
	{0x23, 0x1c, 0x18}
#else
	{0x00, 0x00, 0x00},
	{0x00, 0x51, 0x00},
	{0x00, 0x8f, 0x00},
	{0x70, 0x62, 0x51},
	{0x62, 0x51, 0x43},
	{0x70, 0x62, 0x51},
	{0x51, 0x43, 0x2d},
	{0x8f, 0x70, 0x62}
#endif
};

struct PALETTE BRubber_Palettes[] =
{
    { 8, BRubber_Colours }
};

struct GFXLAYOUT BRubber_charlayout =
{
	8,8,	/* 8*8 characters */
	1024,	/* 1024 characters */
	3,	/* 3 bits per pixel */
	{ 0, 1024*8*8, 2*1024*8*8 },	/* the bitplanes are separated */
	{ 0*8, 1*8, 2*8, 3*8, 4*8, 5*8, 6*8, 7*8 },
	{ 7, 6, 5, 4, 3, 2, 1, 0 },
	8*8	/* every char takes 8 consecutive bytes */
};

struct GFXLAYOUT BRubber_spritelayout =
{
	16,16,  /* 16*16 sprites */
	256,    /* 256 sprites */
	3,	/* 3 bits per pixel */
	{ 0, 256*16*16, 2*256*16*16 },	/* the bitplanes are separated */
	{ 0*8, 1*8, 2*8, 3*8, 4*8, 5*8, 6*8, 7*8,
			8*8, 9*8, 10*8, 11*8, 12*8, 13*8, 14*8, 15*8 },
	{ 7, 6, 5, 4, 3, 2, 1, 0,
			16*8+7, 16*8+6, 16*8+5, 16*8+4, 16*8+3, 16*8+2, 16*8+1, 16*8+0 },
	32*8	/* every sprite takes 32 consecutive bytes */
};

struct GFXLAYOUT BRubber_tilelayout =
{
	16,16,  /* 16*16 characters */
	64,    /* 64 characters */
	3,	/* 3 bits per pixel */
    { 2*64*16*16+4, 0, 4 },
	{ 0*8, 1*8, 2*8, 3*8, 4*8, 5*8, 6*8, 7*8,
			8*8, 9*8, 10*8, 11*8, 12*8, 13*8, 14*8, 15*8 },
    { 3, 2, 1, 0, 16*8+3, 16*8+2, 16*8+1, 16*8+0,
			2*16*8+3, 2*16*8+2, 2*16*8+1, 2*16*8+0, 3*16*8+3, 3*16*8+2, 3*16*8+1, 3*16*8+0 },
	64*8	/* every tile takes 64 consecutive bytes */
};



struct GFXINFO BRubber_GfxInfo[] = 
{
   { 0x0000, &BRubber_charlayout },
   { 0x0000, &BRubber_spritelayout },
   { 0x6000, &BRubber_tilelayout }
};


struct GAMEINFO BRubber =
{
   "Burnin' Rubber",
   5,
   BRubber_RomInfo,
   3,
   BRubber_GfxInfo,
   1,
   BRubber_Palettes,
};


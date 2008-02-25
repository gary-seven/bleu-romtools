//
// 
// Driver for Mr.Do!
//
//
// See drivers.h for description of fields
//

#include "allegro.h"
#include "..\drivers.h"


struct GFXROMINFO MrDot_RomInfo[] =
{
   { "mrdot\\d9", 0x0000, 0x1000 },
   { "mrdot\\d10", 0x1000, 0x1000 },
   { "mrdot\\d8", 0x2000, 0x1000 },
   { "mrdot\\d7", 0x3000, 0x1000 },
   { "mrdot\\d5", 0x4000, 0x1000 },
   { "mrdot\\d6", 0x5000, 0x1000 }
};


RGB MrDo_Colours[] =
{
   { 0, 0, 0 },
   { 48, 0, 0 },
   { 48, 48, 48 },
   { 0, 48, 0 }
};

struct PALETTE MrDo_Palettes[] =
{
    { 4, MrDo_Colours }
};

struct GFXLAYOUT MrDo_charlayout =
{
	8,8,	/* 8*8 characters */
	512,	/* 512 characters */
	2,	/* 2 bits per pixel */
	{ 0, 512*8*8 },	/* the two bitplanes are separated */
	{ 0*8, 1*8, 2*8, 3*8, 4*8, 5*8, 6*8, 7*8 },
	{ 0, 1, 2, 3, 4, 5, 6, 7 },
	8*8	/* every char takes 8 consecutive bytes */
};

struct GFXLAYOUT MrDo_spritelayout =
{
	16,16,	/* 16*16 sprites */
	128,	/* 128 sprites */
	2,	/* 2 bits per pixel */
	{ 4, 0 },	/* the two bitplanes for 4 pixels are packed into one byte */
	{ 0*16, 2*16, 4*16, 6*16, 8*16, 10*16, 12*16, 14*16,
			16*16, 18*16, 20*16, 22*16, 24*16, 26*16, 28*16, 30*16 },
	{ 24+0, 24+1, 24+2, 24+3, 16+0, 16+1, 16+2, 16+3,
			8+0, 8+1, 8+2, 8+3, 0, 1, 2, 3 },
	64*8	/* every sprite takes 64 consecutive bytes */
};



struct GFXINFO MrDo_GfxInfo[] = 
{
   { 0x0000, &MrDo_charlayout },
   { 0x2000, &MrDo_charlayout },
   { 0x4000, &MrDo_spritelayout }
};


struct GAMEINFO MrDot =
{
   "Mr.Do! (taito)",
   6,
   MrDot_RomInfo,
   3,
   MrDo_GfxInfo,
   1,
   MrDo_Palettes,
   0
};

// Mr.Do Universal
struct GFXROMINFO MrDo_RomInfo[] =
{
   { "mrdo\\s8-09.bin", 0x0000, 0x1000 },
   { "mrdo\\u8-10.bin", 0x1000, 0x1000 },
   { "mrdo\\r8-08.bin", 0x2000, 0x1000 },
   { "mrdo\\n8-07.bin", 0x3000, 0x1000 },
   { "mrdo\\h5-05.bin", 0x4000, 0x1000 },
   { "mrdo\\k5-06.bin", 0x5000, 0x1000 }
};

struct GAMEINFO MrDo =
{
   "Mr.Do!",
   6,
   MrDo_RomInfo,
   3,
   MrDo_GfxInfo,
   1,
   MrDo_Palettes,
   0
};

// Mr.Lo 
struct GFXROMINFO MrLo_RomInfo[] =
{
   { "mrlo\\s8-09.bin", 0x0000, 0x1000 },
   { "mrlo\\u8-10.bin", 0x1000, 0x1000 },
   { "mrlo\\r8-08.bin", 0x2000, 0x1000 },
   { "mrlo\\n8-07.bin", 0x3000, 0x1000 },
   { "mrlo\\h5-05.bin", 0x4000, 0x1000 },
   { "mrlo\\k5-06.bin", 0x5000, 0x1000 }
};

struct GAMEINFO MrLo = 
{
   "Mr.Lo!",
   6,
   MrLo_RomInfo,
   3,
   MrDo_GfxInfo,
   1,
   MrDo_Palettes,
   0
};

// ladybug
struct GFXLAYOUT Ladybug_spritelayout =
{
	16,16,	/* 16*16 sprites */
	128,	/* 128 sprites */
	2,	/* 2 bits per pixel */
	{ 1, 0 },	/* the two bitplanes are packed in two consecutive bits */
	{ 23*16, 22*16, 21*16, 20*16, 19*16, 18*16, 17*16, 16*16,
			7*16, 6*16, 5*16, 4*16, 3*16, 2*16, 1*16, 0*16 },
	{ 8*16+14, 8*16+12, 8*16+10, 8*16+8, 8*16+6, 8*16+4, 8*16+2, 8*16+0,
			14, 12, 10, 8, 6, 4, 2, 0 },
	64*8	/* every sprite takes 64 consecutive bytes */
};

struct GFXINFO Ladybug_GfxInfo[] = 
{
   { 0x0000, &MrDo_charlayout },
   { 0x2000, &Ladybug_spritelayout }
};

struct GFXROMINFO Ladybug_RomInfo[] =
{
   { "ladybug\\lb9.vid", 0x0000, 0x1000 },
   { "ladybug\\lb10.vid", 0x1000, 0x1000 },
   { "ladybug\\lb8.cpu", 0x2000, 0x1000 },
   { "ladybug\\lb7.cpu", 0x3000, 0x1000 }
};

struct GAMEINFO Ladybug = 
{
   "Ladybug",
   4,
   Ladybug_RomInfo,
   2,
   Ladybug_GfxInfo,
   1,
   MrDo_Palettes,
   0
};


// Mr Do's Castle
struct GFXLAYOUT DoCastle_charlayout =
{
	8,8,    /* 8*8 characters */
	512,    /* 512 characters */
	4,      /* 4 bits per pixel */
	{ 0, 1, 2, 3 }, /* the bitplanes are packed in one nibble */
	{ 0*32, 1*32, 2*32, 3*32, 4*32, 5*32, 6*32, 7*32 },
	{ 28, 24, 20, 16, 12, 8, 4, 0 },
	32*8   /* every char takes 32 consecutive bytes */
};

struct GFXLAYOUT DoCastle_spritelayout =
{
	16,16,  /* 16*16 sprites */
	256,    /* 256 sprites */
	4,      /* 4 bits per pixel */
	{ 0, 1, 2, 3 }, /* the bitplanes are packed in one nibble */
	{ 0*64, 1*64, 2*64, 3*64, 4*64, 5*64, 6*64, 7*64,
			8*64, 9*64, 10*64, 11*64, 12*64, 13*64, 14*64, 15*64 },
	{ 60, 56, 52, 48, 44, 40, 36, 32,
			28, 24, 20, 16, 12, 8, 4, 0 },
	128*8  /* every sprite takes 128 consecutive bytes */
};

struct GFXINFO DoCastle_GfxInfo[] = 
{
   { 0x0000, &DoCastle_charlayout },
   { 0x4000, &DoCastle_spritelayout }
};

struct GFXROMINFO DoCastle_RomInfo[] =
{
   { "docastle\\a5", 0x0000, 0x4000 },
   { "docastle\\a6", 0x4000, 0x2000 },
   { "docastle\\a7", 0x6000, 0x2000 },
   { "docastle\\a8", 0x8000, 0x2000 },
   { "docastle\\a9", 0xa000, 0x2000 }
};

RGB DoCastle_Colours[] =
{
   {0x00, 0x00, 0x00},
   {0x30, 0x30, 0x30},
   {0x25, 0x25, 0x25},
   {0x20, 0x20, 0x20},
   {0x15, 0x15, 0x15},
   {0x10, 0x10, 0x10},
   {0x30, 0x30, 0x30},
   {0x25, 0x25, 0x25},
   {0x30, 0x30, 0x30},
   {0x15, 0x15, 0x15},
   {0x00, 0x30, 0x00},
   {0x30, 0x00, 0x00},
   {0x30, 0x30, 0x00},
   {0x00, 0x00, 0x30},
   {0x15, 0x00, 0x00},
   {0x05, 0x00, 0x00}
};

struct PALETTE DoCastle_Palettes[] =
{
    { 16, DoCastle_Colours }
};

struct GAMEINFO DoCastle = 
{
   "Mr.Do's Castle",
   5,
   DoCastle_RomInfo,
   2,
   DoCastle_GfxInfo,
   1,
   DoCastle_Palettes,
   0
};


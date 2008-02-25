//
// 
// Drivers for donkey kong and donkey kong junior
//
//
// See drivers.h for description of fields
//

#include "allegro.h"
#include "..\drivers.h"


struct GFXROMINFO DKong_RomInfo[] =
{
   { "dkong\\dk.3n", 0x0000, 0x800 },
   { "dkong\\dk.3p", 0x0800, 0x800 },
   { "dkong\\dk.7c", 0x1000, 0x800 },
   { "dkong\\dk.7d", 0x1800, 0x800 },
   { "dkong\\dk.7e", 0x2000, 0x800 },
   { "dkong\\dk.7f", 0x2800, 0x800 } 
};

struct GFXROMINFO DKongJnr_RomInfo[] =
{
   { "dkongjr\\dkj.3n", 0x0000, 0x1000 },
   { "dkongjr\\dkj.3p", 0x1000, 0x1000 },
   { "dkongjr\\dkj.7c", 0x2000, 0x800 },
   { "dkongjr\\dkj.7d", 0x2800, 0x800 },
   { "dkongjr\\dkj.7e", 0x3000, 0x800 },
   { "dkongjr\\dkj.7f", 0x3800, 0x800 } 
};

RGB DKong_Colours1[] =
{
   { 0, 0, 0 },
   { 48, 48, 16 },
   { 48, 0, 0 },
   { 0, 0, 48 }
};

RGB DKong_Colours2[] =
{
   { 0, 0, 0 },
   { 32, 0, 0 },
   { 48, 48, 16 },
   { 48, 48, 48 }
};

struct PALETTE DKong_Palettes[] =
{
    { 4, DKong_Colours1 },
    { 4, DKong_Colours2 }
};

struct GFXLAYOUT DKong_charlayout =
{
	8,8,	/* 8*8 characters */
	256,	/* 256 characters */
	2,	/* 2 bits per pixel */
	{ 256*8*8, 0 },	/* the two bitplanes are separated */
	{ 7*8, 6*8, 5*8, 4*8, 3*8, 2*8, 1*8, 0*8 },
	{ 0, 1, 2, 3, 4, 5, 6, 7 },	/* pretty straightforward layout */
	8*8	/* every char takes 8 consecutive bytes */
};
struct GFXLAYOUT DKongJnr_charlayout =
{
	8,8,	/* 8*8 characters */
	512,	/* 512 characters */
	2,	/* 2 bits per pixel */
	{ 0, 512*8*8 },	/* the two bitplanes are separated */
	{ 7*8, 6*8, 5*8, 4*8, 3*8, 2*8, 1*8, 0*8 },
	{ 0, 1, 2, 3, 4, 5, 6, 7 },	/* pretty straightforward layout */
	8*8	/* every char takes 8 consecutive bytes */
};
struct GFXLAYOUT DKong_spritelayout =
{
	16,16,	/* 16*16 sprites */
	128,	/* 128 sprites */
	2,	/* 2 bits per pixel */
	{ 128*16*16, 0 },	/* the two bitplanes are separated */
	{ 15*8, 14*8, 13*8, 12*8, 11*8, 10*8, 9*8, 8*8,
			7*8, 6*8, 5*8, 4*8, 3*8, 2*8, 1*8, 0*8 },
	{ 0, 1, 2, 3, 4, 5, 6, 7,	/* the two halves of the sprite are separated */
			64*16*16+0, 64*16*16+1, 64*16*16+2, 64*16*16+3, 64*16*16+4, 64*16*16+5, 64*16*16+6, 64*16*16+7 },
	16*8	/* every sprite takes 16 consecutive bytes */
};


struct GFXINFO DKong_GfxInfo[] = 
{
   { 0x0000, &DKong_charlayout },
	{ 0x1000, &DKong_spritelayout }
};

struct GFXINFO DKongJnr_GfxInfo[] = 
{
   { 0x0000, &DKongJnr_charlayout },
	{ 0x2000, &DKong_spritelayout }
};


struct GAMEINFO DKong =
{
   "Donkey Kong",
   6,
   DKong_RomInfo,
   2,
   DKong_GfxInfo,
   2,
   DKong_Palettes,
	0
};


struct GAMEINFO DKongJnr =
{
   "Donkey Kong Jnr",
   6,
   DKongJnr_RomInfo,
   2,
   DKongJnr_GfxInfo,
   2,
   DKong_Palettes,
   0
};



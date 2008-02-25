//
// 
// Drivers for galaxian and pisces and war of the bugs
//
//
// See drivers.h for description of fields
//

#include "allegro.h"
#include "..\drivers.h"


struct GFXROMINFO Galaxian_RomInfo[] =
{
   { "galaxian\\1h", 0x0000, 0x800 },
   { "galaxian\\1k", 0x0800, 0x800 }
};

struct GFXROMINFO Galmidw_RomInfo[] =
{
   { "galmidw\\galaxian.1j", 0x0000, 0x800 },
   { "galmidw\\galaxian.1k", 0x0800, 0x800 }
};

struct GFXROMINFO Galnamco_RomInfo[] =
{
   { "galnamco\\galaxian.1h", 0x0000, 0x800 },
   { "galnamco\\galaxian.1k", 0x0800, 0x800 }
};

struct GFXROMINFO SuperG_RomInfo[] =
{
   { "superg\\galaxian.1h", 0x0000, 0x800 },
   { "superg\\galaxian.1k", 0x0800, 0x800 }
};

struct GFXROMINFO GalaPX_RomInfo[] =
{
   { "galapx\\galx.1h", 0x0000, 0x800 },
   { "galapx\\galx.1k", 0x0800, 0x800 }
};

struct GFXROMINFO GalaP1_RomInfo[] =
{
   { "galap1\\galx_1c1.rom", 0x0000, 0x800 },
   { "galap1\\galx_1c2.rom", 0x0800, 0x800 }
};

struct GFXROMINFO GalaP4_RomInfo[] =
{
   { "galap4\\galx_4c1.rom", 0x0000, 0x800 },
   { "galap4\\galx_4c2.rom", 0x0800, 0x800 }
};

struct GFXROMINFO GalTurbo_RomInfo[] =
{
   { "galturbo\\galaxian.1h", 0x0000, 0x800 },
   { "galturbo\\galaxian.1k", 0x0800, 0x800 }
};

struct GFXROMINFO Pisces_RomInfo[] =
{
   { "pisces\\pisces.1j", 0x0000, 0x1000 },
   { "pisces\\pisces.1k", 0x1000, 0x1000 }
};

struct GFXROMINFO JapIrem_RomInfo[] =
{
   { "japirem\\h01_1.bin",   0x0000, 0x0800},
   { "japirem\\h01_2.bin",   0x0800, 0x0800},
   { "japirem\\k01_1.bin",   0x1000, 0x0800},
   { "japirem\\k01_2.bin",   0x1800, 0x0800}
};

struct GFXROMINFO Uniwars_RomInfo[] =
{
   { "uniwars\\u10",   0x0000, 0x0800},
   { "uniwars\\u12",   0x0800, 0x0800},
   { "uniwars\\u9",   0x1000, 0x0800},
   { "uniwars\\u11",   0x1800, 0x0800}
};

struct GFXROMINFO WarOfBugs_RomInfo[] =
{
   { "warofbug\\warofbug.1j", 0x0000, 0x800 },
   { "warofbug\\warofbug.1k", 0x0800, 0x800 }
};

struct GFXROMINFO RedUfo_RomInfo[] =
{
   { "redufo\\ruhja", 0x0000, 0x800 },
   { "redufo\\rukla", 0x0800, 0x800 }
};


RGB Galaxian_Colours[] =
{
   { 0, 0, 0 },
   { 0, 48, 0 },
   { 48, 0, 0 },
   { 48, 48, 48 }
};


struct PALETTE Galaxian_Palettes[] =
{
    { 4, Galaxian_Colours }
};


struct GFXLAYOUT Galaxian_charlayout =
{
	8,8,	/* 8*8 characters */
	256,	/* 256 characters */
	2,	/* 2 bits per pixel */
	{ 0, 256*8*8 },	/* the two bitplanes are separated */
	{ 7*8, 6*8, 5*8, 4*8, 3*8, 2*8, 1*8, 0*8 },
	{ 0, 1, 2, 3, 4, 5, 6, 7 },
	8*8	/* every char takes 8 consecutive bytes */
};

struct GFXLAYOUT Galaxian_spritelayout =
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

struct GFXLAYOUT Pisces_charlayout =
{
	8,8,	/* 8*8 characters */
	512,	/* 512 characters */
	2,	/* 2 bits per pixel */
	{ 0, 512*8*8 },	/* the two bitplanes are separated */
	{ 7*8, 6*8, 5*8, 4*8, 3*8, 2*8, 1*8, 0*8 },
	{ 0, 1, 2, 3, 4, 5, 6, 7 },
	8*8	/* every char takes 8 consecutive bytes */
};

struct GFXLAYOUT Pisces_spritelayout =
{
	16,16,	/* 16*16 sprites */
	128,	/* 128 sprites */
	2,	/* 2 bits per pixel */
	{ 0, 128*16*16 },	/* the two bitplanes are separated */
	{ 23*8, 22*8, 21*8, 20*8, 19*8, 18*8, 17*8, 16*8,
			7*8, 6*8, 5*8, 4*8, 3*8, 2*8, 1*8, 0*8 },
	{ 0, 1, 2, 3, 4, 5, 6, 7,
			8*8+0, 8*8+1, 8*8+2, 8*8+3, 8*8+4, 8*8+5, 8*8+6, 8*8+7 },
	32*8	/* every sprite takes 32 consecutive bytes */
};

struct GFXINFO Galaxian_GfxInfo[] = 
{
   { 0x0000, &Galaxian_charlayout },
   { 0x0000, &Galaxian_spritelayout }
};

struct GFXINFO Pisces_GfxInfo[] = 
{
   { 0x0000, &Pisces_charlayout },
   { 0x0000, &Pisces_spritelayout }
};

struct GAMEINFO Galaxian =
{
   "Galaxian",
   2,
   Galaxian_RomInfo,
   2,
   Galaxian_GfxInfo,
   1,
   Galaxian_Palettes,
   0
};

struct GAMEINFO Galmidw =
{
    "Galaxian (Midway)",
   2,
   Galmidw_RomInfo,
   2,
   Galaxian_GfxInfo,
   1,
   Galaxian_Palettes,
   0
};

struct GAMEINFO Galnamco =
{
    "Galaxian Namco mod",
   2,
   Galnamco_RomInfo,
   2,
   Galaxian_GfxInfo,
   1,
   Galaxian_Palettes,
   0
};

struct GAMEINFO SuperG =
{
    "Super Galaxian",
   2,
   SuperG_RomInfo,
   2,
   Galaxian_GfxInfo,
   1,
   Galaxian_Palettes,
   0
};

struct GAMEINFO GalaPX =
{
    "Galaxian Part X",
   2,
   GalaPX_RomInfo,
   2,
   Galaxian_GfxInfo,
   1,
   Galaxian_Palettes,
   0
};

struct GAMEINFO GalaP1 =
{
    "Galaxian Part 1",
   2,
   GalaP1_RomInfo,
   2,
   Galaxian_GfxInfo,
   1,
   Galaxian_Palettes,
   0
};

struct GAMEINFO GalaP4 =
{
    "Galaxian Part 4",
   2,
   GalaP4_RomInfo,
   2,
   Galaxian_GfxInfo,
   1,
   Galaxian_Palettes,
   0
};

struct GAMEINFO GalTurbo =
{
    "Galaxian Turbo",
   2,
   GalTurbo_RomInfo,
   2,
   Galaxian_GfxInfo,
   1,
   Galaxian_Palettes,
   0
};


    struct GAMEINFO RedUfo =
    {
	"Red Ufo",
   2,
   RedUfo_RomInfo,
   2,
   Galaxian_GfxInfo,
   1,
   Galaxian_Palettes,
   0
};

struct GAMEINFO Pisces =
{
   "Pisces",
   2,
   Pisces_RomInfo,
   2,
   Pisces_GfxInfo,
   1,
   Galaxian_Palettes,
};

struct GAMEINFO JapIrem =
{
   "Japanese Irem",
   2,
   JapIrem_RomInfo,
   2,
   Pisces_GfxInfo,
   1,
   Galaxian_Palettes,
};

struct GAMEINFO Uniwars =
{
   "Uniwars",
   2,
   Uniwars_RomInfo,
   2,
   Pisces_GfxInfo,
   1,
   Galaxian_Palettes,
};


struct GAMEINFO WarOfBugs =
{
   "War of the Bugs",
   2,
   WarOfBugs_RomInfo,
   2,
   Galaxian_GfxInfo,
   1,
   Galaxian_Palettes,
};



//
// 
// Drivers for centipede and millipede
//
//
// See drivers.h for description of fields
//

#include "allegro.h"
#include "..\drivers.h"


struct GFXROMINFO Mappy_RomInfo[] =
{
   { "mappy\\mappy3b.32", 0x0000, 0x1000 },
   { "mappy\\mappy3m.64", 0x1000, 0x2000 },
   { "mappy\\mappy3n.64", 0x3000, 0x2000 }
};


RGB Mappy_Colours[] =
{
   { 0, 0, 0 },
   { 0, 0, 48 },
   { 32, 32, 32 }, // grey
   { 40, 0, 0 },
   { 48, 48, 0 }, // yellow
   { 0, 0, 32 }, // dk blue
   { 48, 32, 32 },  // pink
   { 32, 16, 48 }, 
   { 0, 48, 0 }, // green
   { 48, 0, 0 }, // red
   { 32, 0, 0 }, // dk brown
   { 32, 10, 10 }, // lt brown
   { 32, 0, 0 },
   { 32, 0, 32 },
   { 48, 48, 48 }, // white
   { 0, 0, 0 } // black
};

struct PALETTE Mappy_Palettes[] =
{
    { 16, Mappy_Colours }
};



/* layout of the 8x8x2 character data */
struct GFXLAYOUT Mappy_charlayout =
{
	8,8,             /* 8*8 characters */
	256,             /* 256 characters */
	2,             /* 2 bits per pixel */
	{ 0, 4 },      /* the two bitplanes for 4 pixels are packed into one byte */
	{ 7*8, 6*8, 5*8, 4*8, 3*8, 2*8, 1*8, 0*8 },   /* characters are rotated 90 degrees */
	{ 8*8+0, 8*8+1, 8*8+2, 8*8+3, 0, 1, 2, 3 },   /* bits are packed in groups of four */
	16*8           /* every char takes 16 bytes */
};


/* layout of the 16x16x4 sprite data */
struct GFXLAYOUT Mappy_spritelayout =
{
	16,16,       /* 16*16 sprites */
	128,            /* 128 sprites */
	4,                 /* 4 bits per pixel */
	{ 0, 4, 8192*8, 8192*8+4 },     /* the two bitplanes for 4 pixels are packed into one byte */
	{ 39 * 8, 38 * 8, 37 * 8, 36 * 8, 35 * 8, 34 * 8, 33 * 8, 32 * 8,
			7 * 8, 6 * 8, 5 * 8, 4 * 8, 3 * 8, 2 * 8, 1 * 8, 0 * 8 },
	{ 0, 1, 2, 3, 8*8, 8*8+1, 8*8+2, 8*8+3, 16*8+0, 16*8+1, 16*8+2, 16*8+3,
			24*8+0, 24*8+1, 24*8+2, 24*8+3 },
	64*8    /* every sprite takes 64 bytes */
};


struct GFXINFO Mappy_GfxInfo[] = 
{
   { 0x0000, &Mappy_charlayout },
   { 0x1000, &Mappy_spritelayout },
};


struct GAMEINFO Mappy =
{
   "Mappy",
   3,
   Mappy_RomInfo,
   2,
   Mappy_GfxInfo,
   1,
   Mappy_Palettes,
   0
};




//
///////////////////////////////////////////////
//

struct GFXROMINFO DigDug2_RomInfo[] =
{
   { "digdug2\\ddug2-3b.bin", 0x0000, 0x1000 },
   { "digdug2\\ddug2-3m.bin", 0x1000, 0x4000 },
   { "digdug2\\ddug2-3n.bin", 0x5000, 0x4000 }

};



struct GFXLAYOUT DigDug2_spritelayout =
{
	16,16,       /* 16*16 sprites */
	256,            /* 256 sprites */
	4,                 /* 4 bits per pixel */
	{ 0, 4, 16384*8, 16384*8+4 },   /* the two bitplanes for 4 pixels are packed into one byte */
	{ 39 * 8, 38 * 8, 37 * 8, 36 * 8, 35 * 8, 34 * 8, 33 * 8, 32 * 8,
			7 * 8, 6 * 8, 5 * 8, 4 * 8, 3 * 8, 2 * 8, 1 * 8, 0 * 8 },
	{ 0, 1, 2, 3, 8*8, 8*8+1, 8*8+2, 8*8+3, 16*8+0, 16*8+1, 16*8+2, 16*8+3,
			24*8+0, 24*8+1, 24*8+2, 24*8+3 },
	64*8    /* every sprite takes 64 bytes */
};


RGB DigDug2_Colours[] =
{
   { 0, 0, 0 },
   { 0, 0, 32 },
   { 0, 32, 0 },
   { 0, 32, 32 },
   { 32, 0, 0 },
   { 32, 0, 32 },
   { 32, 32, 0 },
   { 32, 32, 32 },
   { 0, 0, 0 },
   { 0, 0, 48 },
   { 0, 48, 0 },
   { 0, 48, 48 },
   { 48, 0, 0 },
   { 48, 0, 48 },
   { 48, 48, 0 },
   { 48, 48, 48 },
};

struct PALETTE DigDug2_Palettes[] =
{
    { 16, DigDug2_Colours }
};


struct GFXINFO DigDug2_GfxInfo[] = 
{
   { 0x0000, &Mappy_charlayout },
   { 0x1000, &DigDug2_spritelayout },
};


struct GAMEINFO DigDug2 =
{
   "DigDug 2",
   3,
   DigDug2_RomInfo,
   2,
   DigDug2_GfxInfo,
   1,
   DigDug2_Palettes,
   0
};

//
///////////////////////////////////////////////
//

struct GFXROMINFO Motos_RomInfo[] =
{
   { "motos\\mts_3b.bin", 0x0000, 0x1000 },
   { "motos\\mts_3m.bin", 0x1000, 0x4000 },
   { "motos\\mts_3n.bin", 0x5000, 0x4000 }

};

RGB Motos_Colours[] =
{
   { 0, 0, 0 },
   { 0, 0, 32 },
   { 0, 32, 0 },
   { 0, 32, 32 },
   { 32, 0, 0 },
   { 32, 0, 32 },
   { 32, 32, 0 },
   { 32, 32, 32 },
   { 0, 0, 0 },
   { 0, 0, 48 },
   { 0, 48, 0 },
   { 0, 48, 48 },
   { 48, 0, 0 },
   { 48, 0, 48 },
   { 48, 48, 0 },
   { 48, 48, 48 },
};

struct PALETTE Motos_Palettes[] =
{
    { 16, Motos_Colours }
};


struct GAMEINFO Motos =
{
   "Motos",
   3,
   Motos_RomInfo,
   2,
   DigDug2_GfxInfo,
   1,
   Motos_Palettes,
   0
};


//
///////////////////////////////////////////////
//

struct GFXROMINFO Druaga_RomInfo[] =
{
   { "todruaga\\druaga3b.bin", 0x0000, 0x1000 },
   { "todruaga\\druaga3m.bin", 0x1000, 0x2000 },
   { "todruaga\\druaga3n.bin", 0x3000, 0x2000 }
};

RGB Druaga_Colours[] =
{
   { 0, 0, 0 },
   { 0, 0, 32 },
   { 0, 32, 0 },
   { 0, 32, 32 },
   { 32, 0, 0 },
   { 32, 0, 32 },
   { 32, 32, 0 },
   { 32, 32, 32 },
   { 0, 0, 0 },
   { 0, 0, 48 },
   { 0, 48, 0 },
   { 0, 48, 48 },
   { 48, 0, 0 },
   { 48, 0, 48 },
   { 48, 48, 0 },
   { 48, 48, 48 },
};

struct PALETTE Druaga_Palettes[] =
{
    { 16, Druaga_Colours }
};


struct GAMEINFO Druaga =
{
   "Tower of Druaga",
   3,
   Druaga_RomInfo,
   2,
   Mappy_GfxInfo,
   1,
   Druaga_Palettes,
   0
};



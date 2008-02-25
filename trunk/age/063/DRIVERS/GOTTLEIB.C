//
// 
// Drivers for Reactor, et al.
//
//
// See drivers.h for description of fields
//

#include "allegro.h"
#include "..\drivers.h"

// Yet to do:
//
// 	reactor driver (characters)
//
// 	qbert driver (characters)
// 	qbert colors


RGB Rainbow_Colours[] =
{
   {  0,  0,  0 }, { 21, 21, 21 }, { 42, 42, 42 }, { 63, 63, 63 },

   { 63,  0,  0 }, { 30,  0,  0 },
   { 63, 63,  0 }, { 30, 30,  0 },
   {  0, 63,  0 }, {  0, 30,  0 },
   {  0, 63, 63 }, {  0, 30, 30 },
   {  0,  0, 63 }, {  0,  0, 30 },
   { 63,  0, 63 }, { 30,  0, 30 },
};

struct GFXROMINFO Reactor_RomInfo[] =
{
   { "reactor\\FG0", 0x0000, 0x1000 },
   { "reactor\\FG1", 0x1000, 0x1000 },
   { "reactor\\FG2", 0x2000, 0x1000 },
   { "reactor\\FG3", 0x3000, 0x1000 }
};


RGB Reactor_Colours[] =
{
   {  0,  0,  0 }, { 60,  0,  0 }, { 30, 30, 30 }, { 40, 40, 63 },
   { 50, 45,  0 }, { 63, 63,  0 }, { 50,  0,  0 }, { 40, 20, 20 },
   { 60, 48,  0 }, { 48,  0,  0 }, { 30,  0, 30 }, { 40,  0, 40 },
   { 32,  0,  0 }, { 50, 40,  0 }, { 48, 48, 48 }, { 45,  0, 20 }
};

struct PALETTE Reactor_Palettes[] =
{
    { 16, Reactor_Colours },
    { 16, Rainbow_Colours }
};



/* layout of the character data */
struct GFXLAYOUT Reactor_charlayout =
{
    // this is broken... fix it.
	8,8,    /* 8*8 characters */
	256,    /* 256 characters */
	4,      /* 4 bits per pixel */
	{ 0, 1, 2, 3 },
	{ 0, 4, 8, 12, 16, 20, 24, 28},
	{ 0*32, 1*32, 2*32, 3*32, 4*32, 5*32, 6*32, 7*32 },
	32*8    /* every char takes 32 consecutive bytes */
};


/* layout of the sprite data */
struct GFXLAYOUT Reactor_spritelayout =
{
	16,16,  /* 16*16 sprites */
	128,    /* 256 sprites */
	4,      /* 4 bits per pixel */
	{ 0, 0x1000*8, 0x2000*8, 0x3000*8 },
	{ 0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15 },

	{ 0*16, 1*16, 2*16, 3*16, 4*16, 5*16, 6*16, 7*16,
	8*16, 9*16, 10*16, 11*16, 12*16, 13*16, 14*16, 15*16 },
	32*8    /* every sprite takes 32 consecutive bytes */
};


struct GFXINFO Reactor_GfxInfo[] =
{
   { 0x0000, &Reactor_spritelayout },
   { 0x3000, &Reactor_charlayout }, // can't figure this out...
};


struct GAMEINFO Reactor =
{
   "Reactor",
   4,
   Reactor_RomInfo,
   2,
   Reactor_GfxInfo,
   2,
   Reactor_Palettes,
   0
};

///////////////////////////////////////////////////////////

struct GFXROMINFO Qbert_RomInfo[] =
{
   { "qbert\\qb-bg0.bin", 0x0000, 0x1000 },
   { "qbert\\qb-bg1.bin", 0x1000, 0x1000 },
   { "qbert\\qb-fg3.bin", 0x2000, 0x2000 },
   { "qbert\\qb-fg2.bin", 0x4000, 0x2000 },
   { "qbert\\qb-fg1.bin", 0x6000, 0x2000 },
   { "qbert\\qb-fg0.bin", 0x8000, 0x2000 }
};

struct GFXROMINFO QbertJP_RomInfo[] =
{
   { "qbertjp\\qb-bg0.bin", 0x0000, 0x1000 },
   { "qbertjp\\qb-bg1.bin", 0x1000, 0x1000 },
   { "qbertjp\\qb-fg3.bin", 0x2000, 0x2000 },
   { "qbertjp\\qb-fg2.bin", 0x4000, 0x2000 },
   { "qbertjp\\qb-fg1.bin", 0x6000, 0x2000 },
   { "qbertjp\\qb-fg0.bin", 0x8000, 0x2000 }
};

struct GFXROMINFO QbertQub_RomInfo[] =
{
   { "qbertqub\\qq-bg0.bin", 0x0000, 0x1000 },
   { "qbertqub\\qq-bg1.bin", 0x1000, 0x1000 },
   { "qbertqub\\qq-fg3.bin", 0x2000, 0x4000 },
   { "qbertqub\\qq-fg2.bin", 0x6000, 0x4000 },
   { "qbertqub\\qq-fg1.bin", 0xa000, 0x4000 },
   { "qbertqub\\qq-fg0.bin", 0xe000, 0x4000 }
};


/* layout of the character data */
struct GFXLAYOUT Qbert_charlayout =
{
	8,8,    /* 8*8 characters */
	256,    /* 256 characters */
	4,      /* 4 bits per pixel */
	{ 0, 1, 2, 3 },
	{ 0, 4, 8, 12, 16, 20, 24, 28},
	{ 0*32, 1*32, 2*32, 3*32, 4*32, 5*32, 6*32, 7*32 },
	32*8    /* every char takes 32 consecutive bytes */
	
};

/* layout of the sprite data */
struct GFXLAYOUT Qbert_spritelayout =
{
	16,16,  /* 16*16 sprites */
	256,    /* 256 sprites */
	4,      /* 4 bits per pixel */
	{ 0, 0x2000*8, 0x4000*8, 0x6000*8 },
	{ 0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15 },
	{ 0*16, 1*16, 2*16, 3*16, 4*16, 5*16, 6*16, 7*16,
	8*16, 9*16, 10*16, 11*16, 12*16, 13*16, 14*16, 15*16 },
	32*8    /* every sprite takes 32 consecutive bytes */
};

/* layout of the sprite data */
struct GFXLAYOUT QbertQub_spritelayout =
{
	16,16,  /* 16*16 sprites */
	512,    /* 256 sprites */
	4,      /* 4 bits per pixel */
	{ 0, 0x4000*8, 0x8000*8, 0xC000*8 },
	{ 0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15 },
	{ 0*16, 1*16, 2*16, 3*16, 4*16, 5*16, 6*16, 7*16,
	8*16, 9*16, 10*16, 11*16, 12*16, 13*16, 14*16, 15*16 },
	32*8    /* every sprite takes 32 consecutive bytes */
};


struct GFXINFO Qbert_GfxInfo[] =
{
   { 0x0000, &Qbert_charlayout },
   { 0x2000, &Qbert_spritelayout },
};

struct GFXINFO QbertQub_GfxInfo[] =
{
   { 0x0000, &Qbert_charlayout },
   { 0x2000, &QbertQub_spritelayout },
};

RGB Qbert_Colours[] =
{
   { 0, 0, 0 },
   { 0, 0, 48 },
   { 48, 0, 0 },
   { 0, 48, 0 },
   { 48, 48, 0 },
   { 0, 0, 32 },
   { 48, 32, 32 },
   { 32, 16, 48 }, 
   { 0, 48, 0 }, 
   { 48, 0, 0 },
   { 62, 60, 0 },
   { 62, 10, 60 },
   { 62, 0, 0 },
   { 62, 0, 32 },
   { 62, 48, 48 }, 
   { 63, 63, 63 } // white
};

struct PALETTE Qbert_Palettes[] =
{
    { 16, Qbert_Colours },
    { 16, Rainbow_Colours }
};

struct GAMEINFO Qbert =
{
   "Q*Bert US",
   6,
   Qbert_RomInfo,
   2,
   Qbert_GfxInfo,
   2,
   Qbert_Palettes,
   0
};

struct GAMEINFO QbertJP =
{
   "Q*Bert JP",
   6,
   QbertJP_RomInfo,
   2,
   Qbert_GfxInfo,
   2,
   Qbert_Palettes,
   0
};



struct GAMEINFO QbertQub =
{
   "Q*Bert's Qubes",
   6,
   QbertQub_RomInfo,
   2,
   QbertQub_GfxInfo,
   2,
   Qbert_Palettes,
   0
};

///////////////////////////////////////////////////////////
// Mad Planets

/* layout of the sprite data */
struct GFXLAYOUT Mplanets_spritelayout =
{
	16,16,  /* 16*16 sprites */
	256,    /* 256 sprites */
	4,      /* 4 bits per pixel */
	{ 0, 0x2000*8, 0x4000*8, 0x6000*8 },
	{ 15, 14, 13, 12, 11, 10, 9, 8, 7, 6, 5, 4, 3, 2, 1, 0},

	{ 0*16, 1*16, 2*16, 3*16, 4*16, 5*16, 6*16, 7*16, 8*16,
	  9*16, 10*16, 11*16, 12*16, 13*16, 14*16, 15*16 },

	32*8    /* every sprite takes 32 consecutive bytes */
};

struct GFXROMINFO Mplanets_RomInfo[] =
{
   { "mplanets\\bg0", 0x0000, 0x1000 },
   { "mplanets\\bg1", 0x1000, 0x1000 },
   { "mplanets\\fg3", 0x2000, 0x2000 },
   { "mplanets\\fg2", 0x4000, 0x2000 },
   { "mplanets\\fg1", 0x6000, 0x2000 },
   { "mplanets\\fg0", 0x8000, 0x2000 }
};

struct GFXINFO Mplanets_GfxInfo[] =
{
   { 0x0000, &Qbert_charlayout },
   { 0x2000, &Mplanets_spritelayout },
};

struct PALETTE Mplanets_Palettes[] =
{
    { 16, Rainbow_Colours },
    { 16, Qbert_Colours },
    { 16, Reactor_Colours },
};

struct GAMEINFO Mplanets =
{
   "Mad Planets",
   6,
   Mplanets_RomInfo,
   2,
   Mplanets_GfxInfo,
   3,
   Mplanets_Palettes,
   0
};

///////////////////////////////////////////////////////////
// Krull


struct GFXROMINFO Krull_RomInfo[] =
{
   { "krull\\fg3.bin", 0x0000, 0x2000 },
   { "krull\\fg2.bin", 0x2000, 0x2000 },
   { "krull\\fg1.bin", 0x4000, 0x2000 },
   { "krull\\fg0.bin", 0x6000, 0x2000 }
};

struct GFXINFO Krull_GfxInfo[] =
{
   { 0x0000, &Qbert_spritelayout },
   { 0x4000, &Qbert_charlayout }, // same problem as the reactor set. <shrug>
};

struct GAMEINFO Krull =
{
   "Krull",
   4,
   Krull_RomInfo,
   1,
   Krull_GfxInfo,
   2,
   Qbert_Palettes,
   0
};

///////////////////////////////////////////////////////////
// stooges


struct GFXROMINFO Stooges_RomInfo[] =
{
   { "3stooges\\GV113FG3", 0x0000, 0x2000 },
   { "3stooges\\GV113FG2", 0x2000, 0x2000 },
   { "3stooges\\GV113FG1", 0x4000, 0x2000 },
   { "3stooges\\GV113FG0", 0x6000, 0x2000 }
};

struct GFXINFO Stooges_GfxInfo[] =
{
   { 0x0000, &Qbert_spritelayout },
   { 0x0000, &Qbert_charlayout }, // same problem as the reactor set. <shrug>
   // this game seems to dynamicly change the character layout base position
};

struct GAMEINFO Stooges =
{
   "Three Stooges",
   4,
   Stooges_RomInfo,
   1,
   Stooges_GfxInfo,
   2,
   Qbert_Palettes,
   0
};

//
// 
// Driver for Tron
//
//
// See drivers.h for description of fields
//

// NOTE: characters must be rotated 90!

#include "allegro.h"
#include "..\drivers.h"

// Yet to do:
//
// 	correct colors for tron

struct GFXROMINFO Tron_RomInfo[] =
{
   { "tron\\SCPU_BGG.BIN", 0x0000, 0x2000 },
   { "tron\\SCPU_BGH.BIN", 0x2000, 0x2000 },
   { "tron\\VG_0.BIN", 0x4000, 0x2000 },
   { "tron\\VG_1.BIN", 0x6000, 0x2000 },
   { "tron\\VG_2.BIN", 0x8000, 0x2000 },
   { "tron\\VG_3.BIN", 0xa000, 0x2000 }
};


RGB Tron_Colours[] =
{
   { 0, 0, 0 }, { 48, 0, 0 }, { 0, 48, 0 }, { 48, 48, 48 },
   { 10, 10, 10 }, { 15, 15, 15 }, { 20, 20, 20 }, { 25, 25, 25 },
   { 30, 30, 30 }, { 35, 35, 35 }, { 40, 40, 40 }, { 45, 45, 45 },
   { 50, 50, 50 }, { 55, 55, 55 }, { 60, 60, 60 }, { 63, 63, 63 }
};

RGB MCR_Rainbow_Colours[] =
{
   {  0,  0,  0 }, { 21, 21, 21 }, { 42, 42, 42 }, { 63, 63, 63 },

   { 63,  0,  0 }, { 30,  0,  0 },
   { 63, 63,  0 }, { 30, 30,  0 },
   {  0, 63,  0 }, {  0, 30,  0 },
   {  0, 63, 63 }, {  0, 30, 30 },
   {  0,  0, 63 }, {  0,  0, 30 },
   { 63,  0, 63 }, { 30,  0, 30 },
};

struct PALETTE Tron_Palettes[] =
{
	{ 16, MCR_Rainbow_Colours},
    { 16, Tron_Colours }
};


struct GFXLAYOUT MCR2_charlayout_512 =
{
    16,16,	/* 16*16 characters */
    512,	/* 512 characters */
    4,	/* 4 bits per pixel */
    { 512*16*8, 512*16*8+1, 0, 1 },
    { 0, 0, 2, 2, 4, 4, 6, 6, 8, 8, 10, 10, 12, 12, 14, 14 },
    { 0, 0, 2*8, 2*8, 4*8, 4*8, 6*8, 6*8, 8*8, 8*8, 10*8, 10*8, 12*8, 12*8, 14*8, 14*8 },
    16*8
};


/* 64 sprites; used by all mcr2 games but Journey */
#define X (64*128*8)
#define Y (2*X)
#define Z (3*X)
struct GFXLAYOUT MCR2_spritelayout_64 =
{
   32,32,
   64,
   4,
   { 0, 1, 2, 3 },
   {  Z+0, Z+4, Y+0, Y+4, X+0, X+4, 0, 4, Z+8, Z+12, Y+8, Y+12, X+8, X+12, 8, 12,
      Z+16, Z+20, Y+16, Y+20, X+16, X+20, 16, 20, Z+24, Z+28, Y+24, Y+28,
      X+24, X+28, 24, 28 },
   {  0, 32, 32*2, 32*3, 32*4, 32*5, 32*6, 32*7, 32*8, 32*9, 32*10, 32*11,
      32*12, 32*13, 32*14, 32*15, 32*16, 32*17, 32*18, 32*19, 32*20, 32*21,
      32*22, 32*23, 32*24, 32*25, 32*26, 32*27, 32*28, 32*29, 32*30, 32*31 },
   128*8
};
#undef X
#undef Y
#undef Z



/* 128 sprites; used by Journey - it features an mcr3 spriteboard */
#define X (128*128*8)
#define Y (2*X)
#define Z (3*X)
struct GFXLAYOUT MCR3_spritelayout_128 =
{
   32,32,
   128,
   4,
   { 0, 1, 2, 3 },
   {  Z+0, Z+4, Y+0, Y+4, X+0, X+4, 0, 4, Z+8, Z+12, Y+8, Y+12, X+8, X+12, 8, 12,
      Z+16, Z+20, Y+16, Y+20, X+16, X+20, 16, 20, Z+24, Z+28, Y+24, Y+28,
      X+24, X+28, 24, 28 },
   {  0, 32, 32*2, 32*3, 32*4, 32*5, 32*6, 32*7, 32*8, 32*9, 32*10, 32*11,
      32*12, 32*13, 32*14, 32*15, 32*16, 32*17, 32*18, 32*19, 32*20, 32*21,
      32*22, 32*23, 32*24, 32*25, 32*26, 32*27, 32*28, 32*29, 32*30, 32*31 },
   128*8
};
#undef X
#undef Y
#undef Z




struct GFXINFO MCR2_GfxInfo[] =
{
   { 0x0000, &MCR2_charlayout_512 },
   { 0x4000, &MCR2_spritelayout_64}
};


struct GAMEINFO Tron =
{
   "Tron (MCR2)",
   6,
   Tron_RomInfo,
   2,
   MCR2_GfxInfo,
   2,
   Tron_Palettes,
   0
};


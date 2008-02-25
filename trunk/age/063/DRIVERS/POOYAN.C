//
// 
// Drivers for Pooyan
//
//
// See drivers.h for description of fields
//

#include "allegro.h"
#include "..\drivers.h"


struct GFXROMINFO Pooyan_RomInfo[] =
{
   { "pooyan\\ic13_g10.cpu", 0x0000, 0x1000 },
   { "pooyan\\ic14_g9.cpu", 0x1000, 0x1000 },
   { "pooyan\\ic16_a8.cpu", 0x2000, 0x1000 },
   { "pooyan\\ic15_a9.cpu", 0x3000, 0x1000 }
};

struct GFXLAYOUT Pooyan_charlayout =
{
	8,8,	/* 8*8 characters */
	256,	/* 256 characters */
	4,	/* 4 bits per pixel */
	{ 0x1000*8+4, 0x1000*8+0, 4, 0 },
	{ 7*8, 6*8, 5*8, 4*8, 3*8, 2*8, 1*8, 0*8 },
	{ 0, 1, 2, 3, 8*8+0,8*8+1,8*8+2,8*8+3 },
	16*8	/* every char takes 16 consecutive bytes */
};

struct GFXLAYOUT Pooyan_spritelayout =
{
	16,16,	/* 16*16 sprites */
	64,	/* 64 sprites */
	4,	/* 4 bits per pixel */
	{ 0x1000*8+4, 0x1000*8+0, 4, 0 },
	{ 39*8, 38*8, 37*8, 36*8, 35*8, 34*8, 33*8, 32*8,
			7*8, 6*8, 5*8, 4*8, 3*8, 2*8, 1*8, 0*8 },
	{ 0, 1, 2, 3,  8*8, 8*8+1, 8*8+2, 8*8+3,
			16*8+0, 16*8+1, 16*8+2, 16*8+3,  24*8+0, 24*8+1, 24*8+2, 24*8+3 },
	64*8	/* every sprite takes 64 consecutive bytes */
};

struct GFXINFO Pooyan_GfxInfo[] = 
{
   { 0x0000, &Pooyan_charlayout },
   { 0x2000, &Pooyan_spritelayout }
};

RGB Pooyan_Colours[] =
{
   {0x00, 0x00, 0x00},
   {0x20, 0x10, 0x00},
   {0x25, 0x25, 0x25},
   {0x20, 0x20, 0x20},
   {0x15, 0x15, 0x30},
   {0x30, 0x00, 0x00},
   {0x30, 0x20, 0x30},
   {0x20, 0x30, 0x30},
   {0x20, 0x10, 0x00},
   {0x00, 0x30, 0x00},
   {0x10, 0x30, 0x00},
   {0x30, 0x00, 0x10},
   {0x30, 0x30, 0x00},
   {0x00, 0x00, 0x30},
   {0x15, 0x00, 0x00},
   {0x30, 0x30, 0x30}
};

struct PALETTE Pooyan_Palettes[] =
{
    { 4, Pooyan_Colours }
};

struct GAMEINFO Pooyan = 
{
   "Pooyan",
   4,
	Pooyan_RomInfo,
   2,
   Pooyan_GfxInfo,
   1,
   Pooyan_Palettes,
   0
};


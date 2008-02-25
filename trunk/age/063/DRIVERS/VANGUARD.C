//
// 
// Driver for vanguard
//
//
// See drivers.h for description of fields
//

#include "allegro.h"
#include "..\drivers.h"


struct GFXROMINFO Vanguard_RomInfo[] =
{
   { "vanguard\\sk5_ic50.bin", 0x0000, 0x800 },
   { "vanguard\\sk5_ic51.bin", 0x0800, 0x800 }
};


RGB Vanguard_Colours[] =
{
   { 0, 0, 0 },
   { 0, 48, 0 },
   { 48, 0, 0 },
   { 48, 48, 48 }
};

struct PALETTE Vanguard_Palettes[] =
{
    { 4, Vanguard_Colours }
};

struct GFXLAYOUT Vanguard_charlayout =
{
        8,8,    /* 8*8 characters */
        256,    /* 256 characters */
        2,      /* 2 bits per pixel */
        { 0, 256*8*8 }, /* the two bitplanes are separated */
        { 7*8, 6*8, 5*8, 4*8, 3*8, 2*8, 1*8, 0*8 },
        { 0, 1, 2, 3, 4, 5, 6, 7 },
        8*8     /* every char takes 8 consecutive bytes */
};


struct GFXINFO Vanguard_GfxInfo[] = 
{
   { 0x0000, &Vanguard_charlayout }
};


struct GAMEINFO Vanguard =
{
   "Vanguard",
   2,
   Vanguard_RomInfo,
   1,
   Vanguard_GfxInfo,
   1,
   Vanguard_Palettes,
   0
};





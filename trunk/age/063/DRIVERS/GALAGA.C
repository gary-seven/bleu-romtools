//
// 
// Drivers for galaga et al.
//
//
// See drivers.h for description of fields
//

#include "allegro.h"
#include "..\drivers.h"


struct GFXROMINFO Galaga_RomInfo[] =
{
    // Galaga (Midway)
    {"galaga\\2600j_4l.bin", 0x0000, 0x1000 },
    {"galaga\\2800l_4d.bin", 0x1000, 0x1000 },
    {"galaga\\2700k.bin",    0x2000, 0x1000 },
};

struct GFXROMINFO GalagaNM_RomInfo[] =
{
    // Galaga (Namco)
    {"galaganm\\07m_g08.bin", 0x0000, 0x1000 },
    {"galaganm\\07e_g10.bin", 0x1000, 0x1000 },
    {"galaganm\\07h_g09.bin", 0x2000, 0x1000 },
};

struct GFXROMINFO GalagaBL_RomInfo[] =
{
    // Galaga (Bootleg)
    {"galagabl\\galagabl.1_8", 0x0000, 0x1000 },
    {"galagabl\\galagabl.1_a", 0x1000, 0x1000 },
    {"galagabl\\galagabl.1_9", 0x2000, 0x1000 },
};

struct GFXROMINFO Gallag_RomInfo[] =
{
    // Gallag (bootleg)
    {"gallag\\gallag.8", 0x0000, 0x1000 },
    {"gallag\\gallag.9", 0x1000, 0x1000 },
    {"gallag\\gallag.a", 0x2000, 0x1000 },
};


RGB Galaga_Colours_Wingnut[] =
{ { 0, 0, 0}, {30,20, 0}, { 0,35,35}, {55,55, 0} };

RGB Galaga_Colours_Wingnut2[] =
{ { 0, 0, 0}, {40, 0,35}, { 0, 0,55}, {55, 0,55} };

RGB Galaga_Colours_Bee[] =
{ { 0, 0, 0}, {50, 0, 0}, { 0, 0,50}, {55,55, 0} };

RGB Galaga_Colours_Bug[] =
{ { 0, 0, 0}, {55,55,55}, { 0, 0,55}, {55, 0, 0} };

RGB Galaga_Colours_Pollen[] =
{ { 0, 0, 0}, { 0,55,55}, { 55,0, 0}, {55,45, 0} };

struct PALETTE Galaga_Palettes[] =
{
    { 4, Galaga_Colours_Wingnut },
    { 4, Galaga_Colours_Wingnut2 },
    { 4, Galaga_Colours_Bee },
    { 4, Galaga_Colours_Bug },
    { 4, Galaga_Colours_Pollen },
};


struct GFXLAYOUT Galaga_charlayout =
{
	8,8,           /* 8*8 characters */
	128,           /* 128 characters */
	2,             /* 2 bits per pixel */
	{ 0, 4 },       /* the two bitplanes for 4 pixels are packed into one byte */
	{ 7*8, 6*8, 5*8, 4*8, 3*8, 2*8, 1*8, 0*8 },   /* characters are rotated 90 degrees */
	{ 8*8+0, 8*8+1, 8*8+2, 8*8+3, 0, 1, 2, 3 },   /* bits are packed in groups of four */
	16*8           /* every char takes 16 bytes */
} ;


struct GFXLAYOUT Galaga_spritelayout =
{
	16,16,          /* 16*16 sprites */
	128,            /* 128 sprites */
	2,              /* 2 bits per pixel */
	{ 0, 4 },       /* the two bitplanes for 4 pixels are packed into one byte */
	{ 39 * 8, 38 * 8, 37 * 8, 36 * 8, 35 * 8, 34 * 8, 33 * 8, 32 * 8,
			7 * 8, 6 * 8, 5 * 8, 4 * 8, 3 * 8, 2 * 8, 1 * 8, 0 * 8 },
	{ 0, 1, 2, 3, 8*8, 8*8+1, 8*8+2, 8*8+3, 16*8+0, 16*8+1, 16*8+2, 16*8+3,
			24*8+0, 24*8+1, 24*8+2, 24*8+3 },
	64*8    /* every sprite takes 64 bytes */
};


struct GFXINFO Galaga_GfxInfo[] =
{
    { 0x0000, &Galaga_charlayout },
    { 0x1000, &Galaga_spritelayout },
};


struct GAMEINFO Galaga =
{
    "Galaga (Midway)",
    3,
    Galaga_RomInfo,
    2,
    Galaga_GfxInfo,
    5,
    Galaga_Palettes,
    0
};


struct GAMEINFO GalagaNM =
{
    "Galaga (Namco)",
    3,
    GalagaNM_RomInfo,
    2,
    Galaga_GfxInfo,
    5,
    Galaga_Palettes,
    0
};

struct GAMEINFO GalagaBL =
{
    "Galaga (Bootleg)",
    3,
    GalagaBL_RomInfo,
    2,
    Galaga_GfxInfo,
    5,
    Galaga_Palettes,
    0
};

struct GAMEINFO Gallag =
{
    "Gallag (Bootleg)",
    3,
    Gallag_RomInfo,
    2,
    Galaga_GfxInfo,
    5,
    Galaga_Palettes,
    0
};

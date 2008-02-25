// 
// Drivers for pacman, crush roller, pengo and ms pacman
//   as well as all variants...
//
// added 11/97 by Scott "Jerry" Lawrence
//   jerry@mail.csh.rit.edu
//   http://www.csh.rit.edu/~jerry
//
// new sprite/character layouts added 1/23/98
//
// text string layouts added 1/27/98
// special thanks to M.C.Silvius (msilvius@caiw.nl) for pacman
//  variant name information!
//
// See drivers.h for description of fields
//

#include "allegro.h"
#include "..\drivers.h"
#include "..\mapdrv.h"


////////////// Pac Man & Variants //////////////

struct GFXROMINFO PacMan_RomInfo[] =
{
   { "pacman\\pacman.5e", 0x0000, 0x1000 }, // each of these roms are 4k apiece
   { "pacman\\pacman.5f", 0x1000, 0x1000 }
};

struct GFXROMINFO PacMan_DataRomInfo[] =
{
   { "pacman\\pacman.6e", 0x0000, 0x1000 }, // each of these roms are 4k apiece
   { "pacman\\pacman.6f", 0x1000, 0x1000 },
   { "pacman\\pacman.6h", 0x2000, 0x1000 },
   { "pacman\\pacman.6j", 0x3000, 0x1000 },
};


struct GFXROMINFO MsPacMan_RomInfo[] =
{
   { "mspacman\\5e", 0x0000, 0x1000 }, // each of these roms are 4k apiece
   { "mspacman\\5f", 0x1000, 0x1000 }
};
struct GFXROMINFO MsPacMan_DataRomInfo[] =
{
   { "mspacman\\boot1", 0x0000, 0x1000 }, // each of these roms are 4k apiece
   { "mspacman\\boot2", 0x1000, 0x1000 },
   { "mspacman\\boot3", 0x2000, 0x1000 },
   { "mspacman\\boot4", 0x3000, 0x1000 },
   { "mspacman\\boot5", 0x4000, 0x1000 },
   { "mspacman\\boot6", 0x5000, 0x1000 },
};

struct GFXROMINFO Crush_RomInfo[] =
{
   { "crush\\CRA", 0x0000, 0x800 }, // each of these roms are 2k apiece
   { "crush\\CRC", 0x0800, 0x800 },
   { "crush\\CRB", 0x1000, 0x800 },
   { "crush\\CRD", 0x1800, 0x800 }
};

// alternate sets...
struct GFXROMINFO MsPacAtk_RomInfo[] =
{
   { "mspacatk\\5e", 0x0000, 0x1000 }, // each of these roms are 4k apiece
   { "mspacatk\\5f", 0x1000, 0x1000 }
};
struct GFXROMINFO MsPacAtk_DataRomInfo[] =
{
   { "mspacatk\\mspacatk.1", 0x0000, 0x1000 }, // each of these roms are 4k apiece
   { "mspacatk\\mspacatk.2", 0x1000, 0x1000 },
   { "mspacatk\\mspacatk.3", 0x2000, 0x1000 },
   { "mspacatk\\mspacatk.4", 0x3000, 0x1000 },
   { "mspacatk\\mspacatk.5", 0x4000, 0x1000 },
   { "mspacatk\\mspacatk.6", 0x5000, 0x1000 },
};

struct GFXROMINFO NamcoPac_RomInfo[] =
{
   { "namcopac\\namcopac.5e", 0x0000, 0x1000 }, // each of these roms are 4k apiece
   { "namcopac\\namcopac.5f", 0x1000, 0x1000 }
};
struct GFXROMINFO NamcoPac_DataRomInfo[] =
{
   { "namcopac\\namcopac.6e", 0x0000, 0x1000 }, // each of these roms are 4k apiece
   { "namcopac\\namcopac.6f", 0x1000, 0x1000 },
   { "namcopac\\namcopac.6h", 0x2000, 0x1000 },
   { "namcopac\\namcopac.6j", 0x3000, 0x1000 },
};

struct GFXROMINFO PacMod_RomInfo[] =
{
   { "pacmod\\pacmanh.5e", 0x0000, 0x1000 }, // each of these roms are 4k apiece
   { "pacmod\\pacmanh.5f", 0x1000, 0x1000 }
};
struct GFXROMINFO PacMod_DataRomInfo[] =
{
   { "pacmod\\pacmanh.6e", 0x0000, 0x1000 }, // each of these roms are 4k apiece
   { "pacmod\\pacmanh.6f", 0x1000, 0x1000 },
   { "pacmod\\pacmanh.6h", 0x2000, 0x1000 },
   { "pacmod\\pacmanh.6j", 0x3000, 0x1000 },
};

struct GFXROMINFO PacPlus_RomInfo[] =
{
   { "pacplus\\pacplus.5e", 0x0000, 0x1000 }, // each of these roms are 4k apiece
   { "pacplus\\pacplus.5f", 0x1000, 0x1000 }
};
struct GFXROMINFO PacPlus_DataRomInfo[] =
{
   { "pacplus\\pacplus.6e", 0x0000, 0x1000 }, // each of these roms are 4k apiece
   { "pacplus\\pacplus.6f", 0x1000, 0x1000 },
   { "pacplus\\pacplus.6h", 0x2000, 0x1000 },
   { "pacplus\\pacplus.6j", 0x3000, 0x1000 },
};

struct GFXROMINFO Hangly_RomInfo[] =
{
   { "hangly\\hangly.5e", 0x0000, 0x1000 }, // each of these roms are 4k apiece
   { "hangly\\hangly.5f", 0x1000, 0x1000 }
};
struct GFXROMINFO Hangly_DataRomInfo[] =
{
   { "hangly\\hangly.6e", 0x0000, 0x1000 }, // each of these roms are 4k apiece
   { "hangly\\hangly.6f", 0x1000, 0x1000 },
   { "hangly\\hangly.6h", 0x2000, 0x1000 },
   { "hangly\\hangly.6j", 0x3000, 0x1000 },
};

struct GFXROMINFO PuckMan_RomInfo[] =
{
   { "puckman\\puckman.5e", 0x0000, 0x1000 }, // each of these roms are 4k apiece
   { "puckman\\puckman.5f", 0x1000, 0x1000 }
};
struct GFXROMINFO PuckMan_DataRomInfo[] =
{
   { "puckman\\puckman.6e", 0x0000, 0x1000 }, // each of these roms are 4k apiece
   { "puckman\\puckman.6f", 0x1000, 0x1000 },
   { "puckman\\puckman.6h", 0x2000, 0x1000 },
   { "puckman\\puckman.6j", 0x3000, 0x1000 },
};

struct GFXROMINFO PacManJP_RomInfo[] =
{
   { "pacmanjp\\chg1", 0x0000, 0x0800 }, // each of these roms are 2k apiece
   { "pacmanjp\\chg2", 0x0800, 0x0800 },
   { "pacmanjp\\chg3", 0x1000, 0x0800 },
   { "pacmanjp\\chg4", 0x1800, 0x0800 }
};
struct GFXROMINFO PacManJP_DataRomInfo[] =
{
   { "pacmanjp\\prg1", 0x0000, 0x0800 }, // each of these roms are 2k apiece
   { "pacmanjp\\prg2", 0x0800, 0x0800 },
   { "pacmanjp\\prg3", 0x1000, 0x0800 },
   { "pacmanjp\\prg4", 0x1800, 0x0800 },
   { "pacmanjp\\prg5", 0x2000, 0x0800 },
   { "pacmanjp\\prg6", 0x2800, 0x0800 },
   { "pacmanjp\\prg7", 0x3000, 0x0800 },
   { "pacmanjp\\prg8", 0x3800, 0x0800 },
};

struct GFXROMINFO Piranha_RomInfo[] =
{
   { "piranha\\pr5.cpu", 0x0000, 0x0800 }, // each of these roms are 2k apiece
   { "piranha\\pr7.cpu", 0x0800, 0x0800 },
   { "piranha\\pr6.cpu", 0x1000, 0x0800 },
   { "piranha\\pr8.cpu", 0x1800, 0x0800 }
};
struct GFXROMINFO Piranha_DataRomInfo[] =
{
   { "piranha\\pr1.cpu", 0x0000, 0x1000 }, // each of these roms are 4k apiece
   { "piranha\\pr2.cpu", 0x1000, 0x1000 },
   { "piranha\\pr3.cpu", 0x2000, 0x1000 },
   { "piranha\\pr4.cpu", 0x3000, 0x1000 },
};


struct GFXROMINFO PacManBL_RomInfo[] =
{
    { "pacmanbl\\blpac12b", 0x0000, 0x0800},
    { "pacmanbl\\blpac11b", 0x0800, 0x0800},
    { "pacmanbl\\blpac10b", 0x1000, 0x0800},
    { "pacmanbl\\blpac9b",  0x1800, 0x0800},
};

/*
Color Key:
0: editing ghosts 
1: scared ghost
2: pacman & 6th sprite (bird)
3: 0th sprite (cherry)
4: 1st sprite (strawberry)
5: 2nd sprite (orange)
6: 3rd sprite (bell)
7: 4th sprite (apple)
8: 5th sprite (grenade looking thing)
9: 7th sprite (key)
10: map editing colors 1 (contrasts)
11: map editing colors 2 (like the game)
*/
RGB PacMan_Colours0[] =
{
    // colors for editing ghosts
    // show Oikake/Urchin/Shadow/Blinky (red ghost)
    {  0,  0,  0 }, { 50, 50, 50 }, {  0,  0, 30 }, { 50,  0,  0 }
};

RGB PacMan_Colours1[] =
{							  
    // colors for editing edible (blue) ghosts
    {  0,  0,  0 }, {  0, 50,  0 }, {  0,  0, 30 }, { 50, 40, 20 }
};

RGB PacMan_Colours2[] =
{							  
     // colors for editing pacman himself
    {  0,  0,  0 }, {  0,  0, 50 }, { 60,  0,  0 }, { 60, 60,  0 }
};

RGB PacMan_Colours3[] =
{							  
    // Cherry colors
    {  0,  0,  0 }, { 50,  0,  0 }, { 30, 30,  0 }, { 50, 50, 50 }
};

RGB PacMan_Colours4[] =
{							  
    // Strawberry colors
    {  0,  0,  0 }, { 50,  0,  0 }, {  0, 50,  0 }, { 50, 50, 50 }
};

RGB PacMan_Colours5[] =
{							  
    // Orange colors
    {  0,  0,  0 }, { 40, 40,  0 }, {  0, 50,  0 }, { 30, 30,  0 }
};

RGB PacMan_Colours6[] =
{							  
    // Bell colors
    {  0,  0,  0 }, { 50, 50,  0 }, { 30, 30, 50 }, { 50, 50, 50 }
};

RGB PacMan_Colours7[] =
{							  
    // Apple colors
    {  0,  0,  0 }, { 50,  0,  0 }, { 30, 30,  0 }, { 50, 50, 50 }
};

RGB PacMan_Colours8[] =
{							  
    // Grenade-looking thing colors
    {  0,  0,  0 }, {  0, 40, 50 }, {  0, 50,  0 }, { 50, 50, 50 }
};

RGB PacMan_Colours9[] =
{							  
    // Key colors
    {  0,  0,  0 }, {  0,  0,  0 }, { 30, 30, 50 }, { 50, 50, 50 }
};

RGB PacMan_Colours10[] =
{							  
    // map editing colors (sharp contrasts)
    { 60,  0,  0 }, { 60, 60, 60 }, {  0, 60,  0 }, {  0,  0, 50 }
};

RGB PacMan_Colours11[] =
{							  
    // map editing colors (realistic to game)
    {  0,  0,  0 }, { 60, 60, 60 }, {  0,  0,  0 }, {  0,  0, 50 }
};

struct PALETTE PacMan_Palettes[] =
{
    { 4, PacMan_Colours0 },
    { 4, PacMan_Colours1 },
    { 4, PacMan_Colours2 },
    { 4, PacMan_Colours3 },
    { 4, PacMan_Colours4 },
    { 4, PacMan_Colours5 },
    { 4, PacMan_Colours6 },
    { 4, PacMan_Colours7 },
    { 4, PacMan_Colours8 },
    { 4, PacMan_Colours9 },
    { 4, PacMan_Colours10 },
    { 4, PacMan_Colours11 },
};


struct GFXLAYOUT PacMan_charlayout =
{
    8,8,	/* 8*8 characters */
    256,	/* 256 characters */
    2,	/* 2 bits per pixel */
    { 0, 4},	/* the two bitplanes for 4 pixels are packed into one byte */
    { 7*8, 6*8, 5*8, 4*8, 3*8, 2*8, 1*8, 0*8 }, /* characters are rotated 90 degrees */
    { 8*8+0, 8*8+1, 8*8+2, 8*8+3, 0, 1, 2, 3 },	/* bits are packed in groups of four */
    16*8	/* every char takes 16 bytes */
};


struct GFXLAYOUT PacMan_spritelayout =
{
	16,16,	/* 16*16 sprites */
	64,	/* 64 sprites */
	2,	/* 2 bits per pixel */
	{ 0, 4 },	/* the two bitplanes for 4 pixels are packed into one byte */
	{ 39 * 8, 38 * 8, 37 * 8, 36 * 8, 35 * 8, 34 * 8, 33 * 8, 32 * 8,
	    7 * 8, 6 * 8, 5 * 8, 4 * 8, 3 * 8, 2 * 8, 1 * 8, 0 * 8 },
	{ 8*8, 8*8+1, 8*8+2, 8*8+3, 16*8+0, 16*8+1, 16*8+2, 16*8+3,
			24*8+0, 24*8+1, 24*8+2, 24*8+3, 0, 1, 2, 3 },
	64*8	/* every sprite takes 64 bytes */
};


struct GFXLAYOUT PacMan_charspritelayout_1 = // for the fruits & guys
{
	16,16,	/* 16*16 sprites */
	64,	/* 64 sprites */
	2,	/* 2 bits per pixel */
	{ 0, 4 },	/* the two bitplanes for 4 pixels are packed into one byte */

	/* aagh!  this is aggrivating! */

	{
	  7*8+128, 6*8+128, 5*8+128, 4*8+128,
	  3*8+128, 2*8+128, 1*8+128, 0*8+128,
	  7*8,     6*8,     5*8,     4*8,
	  3*8,     2*8,     1*8,     0*8,
	}, /* characters are rotated 90 degrees */

	{
	  8*8+0,     8*8+1,     8*8+2,     8*8+3,
	      0,         1,         2,         3,
	  8*8+0+256, 8*8+1+256, 8*8+2+256, 8*8+3+256,
	      0+256,     1+256,     2+256,     3+256,
	},/* bits are packed in groups of four */

	64*8	/* every sprite takes 64 bytes */
};

struct GFXLAYOUT PacMan_charspritelayout_2 = // for the demo ghost
{
	16,16,	/* 16*16 sprites */
	64,	/* 64 sprites */
	2,	/* 2 bits per pixel */
	{ 0, 4 },	/* the two bitplanes for 4 pixels are packed into one byte */

	/* aagh!  this is aggrivating! */

	{
	  7*8,     6*8,     5*8,     4*8,
	  3*8,     2*8,     1*8,     0*8,
	  7*8+128, 6*8+128, 5*8+128, 4*8+128,
	  3*8+128, 2*8+128, 1*8+128, 0*8+128,
	}, /* characters are rotated 90 degrees */

	{
	  8*8+0,     8*8+1,     8*8+2,     8*8+3,
	      0,         1,         2,         3,
	  8*8+0+256, 8*8+1+256, 8*8+2+256, 8*8+3+256,
	      0+256,     1+256,     2+256,     3+256,
	},/* bits are packed in groups of four */

	64*8	/* every sprite takes 64 bytes */
};


struct GFXINFO PacMan_GfxInfo[] = 
{
   { 0x0000, &PacMan_charlayout },	    // the original chars
   { 0x1000, &PacMan_spritelayout },        // the original sprites
   { 0x0000, &PacMan_charspritelayout_1 },  // fruit & extra lives
   { 0x0000, &PacMan_charspritelayout_2 }   // demo ghost (sprite 2c,2d)
};


char pacman_hc[] = { 1, 4,  26, 4,  1, 24,  26, 24};
struct MAPINFO PacMan_MapInfo[] =
{
    { 0x3436, 0x35b5, 240, 28, 32, 0, &PacMan_Functions, 4, pacman_hc },
	// map starts at 6j + 0436h,
	// pellot overlay is at 6j + 05b5h
	//  is 28x32 chars big,
	//  uses gfx bank number 0,
	//  and is a PACMAN map
	// it has 4 hardcoded bits, at the positions in pacman_hc
};

struct TEXTED_STRING PacMan_Strings_1[] =
{
    {"Red Name", 0x3D59, 10},
    {"Red Nick", 0x3d69, 9},
    {"Pink Name", 0x3d78, 10},
    {"Pink Nick", 0x3d88, 9},
    {"Aqua Name", 0x3d97, 10},
    {"Aqua Nick", 0x3da7, 9},
    {"Orange Name", 0x3db6, 10},
    {"Orange Nick", 0x3dc6, 9},
};

struct TEXTED_STRING PacMan_Strings_2[] =
{
    {"Red Name", 0x3dd5, 10},
    {"Red Nick", 0x3de5, 9},
    {"Pink Name", 0x3df4, 10},
    {"Pink Nick", 0x3e04, 9},
    {"Aqua Name", 0x3e13, 10},
    {"Aqua Nick", 0x3e23, 9},
    {"Orange Name", 0x3e32, 10},
    {"Orange Nick", 0x3e42, 9},
};

struct TEXTED_STRING PacMan_Strings_3[] =
{
    {"High Score", 0x3715, 10},
    {"Credit", 0x3725, 8},
    {"Free Play", 0x3734, 9},
    {"Player One", 0x3743, 10},
    {"Player Two", 0x375c, 10},
    {"Game Over", 0x376c, 10},
    {"Ready!", 0x377c, 6},
    {"Char / Nick", 0x37ff, 20},
    {"Bonus...", 0x3d02, 13},
    {"  ...Pts", 0x3d0f, 14},
    {"Push Start", 0x3788, 17},
    {"1P only", 0x379f, 14},
    {"1/2 Players", 0x37b3, 14},
    {"Start Cprt", 0x3d23, 21},
    {"Names Cprt", 0x3d3e, 21},
};

struct TEXTED_LOOKUP PacMan_Lookup[] =
{
    //{ascii, rom}
    // lowercase letters
    {'a',0x41}, {'b',0x42}, {'c',0x43}, {'d',0x44}, {'e',0x45}, {'f',0x46},
    {'g',0x47}, {'h',0x48}, {'i',0x49}, {'j',0x4a}, {'k',0x4b}, {'l',0x4c},
    {'m',0x4d}, {'n',0x4e}, {'o',0x4f}, {'p',0x50}, {'q',0x51}, {'r',0x52},
    {'s',0x53}, {'t',0x54}, {'u',0x55}, {'v',0x56}, {'w',0x57}, {'x',0x58},
    {'y',0x59}, {'z',0x5a},
    // uppercase letters
    {'A',0X41}, {'B',0X42}, {'C',0X43}, {'D',0X44}, {'E',0X45}, {'F',0X46},
    {'G',0X47}, {'H',0X48}, {'I',0X49}, {'J',0X4A}, {'K',0X4B}, {'L',0X4C},
    {'M',0X4D}, {'N',0X4E}, {'O',0X4F}, {'P',0X50}, {'Q',0X51}, {'R',0X52},
    {'S',0X53}, {'T',0X54}, {'U',0X55}, {'V',0X56}, {'W',0X57}, {'X',0X58},
    {'Y',0X59}, {'Z',0X5A},
    // numbers
    {'0',0x30}, {'1',0x31}, {'2',0x32}, {'3',0x33}, {'4',0x34}, {'5',0x35},
    {'6',0x36}, {'7',0x37}, {'8',0x38}, {'9',0x39},
    // punctuation
    {'.',0x25}, {'/',0x3a}, {'!',0x5b}, {'@',0x5c}, {' ',0x40}, {'-',0x3b},
    {'\"', 0x26}, {'\'', 0x27},
    // misc
    {'[',0x5d}, {']',0x5e}, {'\\',0x5f},  // " pts "

    {'#',0x28}, {'$',0x29}, {'%',0x2a}, {'^',0x2b},
    {'&',0x2c}, {'*',0x2d}, {'(',0x2e}, // NAMCO
    // end
    {0, 0x40}, // end character, and character to insert
};

struct TEXTINFO PacMan_TextInfo[] =
{
    {"Main Ghost Names", 8,  PacMan_Strings_1, PacMan_Lookup},
    {"Alt. Ghost Names", 8,  PacMan_Strings_2, PacMan_Lookup},
    {"Screen Text",      15, PacMan_Strings_3, PacMan_Lookup},
};

struct ROMDATA PacManData =
{
    4,			// 4 more roms to load
    PacMan_DataRomInfo, // array of 4 roms
    1,			// one map
    PacMan_MapInfo,	// map location
    3,			// number of categories
    PacMan_TextInfo,	// string information
    0,
    NULL,
};


struct GAMEINFO PacMan = 
{
   "Pac-Man (Midway)",
   2,
   PacMan_RomInfo,
   4, 
   PacMan_GfxInfo,
   12,
   PacMan_Palettes,
   &PacManData
};



extern struct GFXLAYOUT Galaxian_charlayout;
extern struct GFXLAYOUT Galaxian_spritelayout;

struct GFXINFO PacManBL_GfxInfo[] =
{
   { 0x0000, &Galaxian_charlayout },
   { 0x1000, &Galaxian_spritelayout },
};

struct GAMEINFO PacManBL =
{
   "Pac-Man On Galaxian",
   2,
   PacManBL_RomInfo,
   2,
   PacManBL_GfxInfo,
   12,
   PacMan_Palettes,
   0
};


struct ROMDATA NamcoPacData =
{
    4,			// 4 more roms to load
    NamcoPac_DataRomInfo, // array of 4 roms
    1,			// one map
    PacMan_MapInfo,	// map location
    0,
    NULL,		// string information
    0,
    NULL,
};
struct GAMEINFO NamcoPac =
{
   "Pac-Man (Namco)",
   2,
   NamcoPac_RomInfo,
   4, 
   PacMan_GfxInfo,
   12,
   PacMan_Palettes,
   &NamcoPacData
};


struct ROMDATA PacModData =
{
    4,			// 4 more roms to load
    PacMod_DataRomInfo, // array of 4 roms
    1,			// one map
    PacMan_MapInfo,	// map location
    3,			// number of categories
    PacMan_TextInfo,	// string information
    0,
    NULL,
};
struct GAMEINFO PacMod =
{
   "Pac-Man (Modified)",
   2,
   PacMod_RomInfo,
   4, 
   PacMan_GfxInfo,
   12,
   PacMan_Palettes,
   &PacModData
};


struct ROMDATA PacPlusData =
{
    4,			// 4 more roms to load
    PacPlus_DataRomInfo, // array of 4 roms
    1,			// one map
    PacMan_MapInfo,	// map location
    3,			// number of categories
    PacMan_TextInfo,	// string information
    0,
    NULL,
};
struct GAMEINFO PacPlus =
{
   "Pac-Man Plus",
   2,
   PacPlus_RomInfo,
   4, 
   PacMan_GfxInfo,
   12,
   PacMan_Palettes,
   &PacPlusData
};


struct ROMDATA HanglyData =
{
    4,			// 4 more roms to load
    Hangly_DataRomInfo, // array of 4 roms
    1,			// one map
    PacMan_MapInfo,	// map location
    0,
    NULL,		// string information
    0,
    NULL,
};
struct GAMEINFO Hangly =
{
   "Hangly Man",
   2,
   Hangly_RomInfo,
   4, 
   PacMan_GfxInfo,
   12,
   PacMan_Palettes,
   &HanglyData
};


struct ROMDATA PuckManData =
{
    4,			// 4 more roms to load
    PuckMan_DataRomInfo, // array of 4 roms
    1,			// one map
    PacMan_MapInfo,	// map location
    0,
    NULL,		// string information
    0,
    NULL,
};
struct GAMEINFO PuckMan =
{
   "Puck Man",
   2,
   PuckMan_RomInfo,
   4, 
   PacMan_GfxInfo,
   12,
   PacMan_Palettes,
   &PuckManData
};


struct ROMDATA PacManJPData =
{
    8,			// 4 more roms to load
    PacManJP_DataRomInfo, // array of 8 roms
    1,			// one map
    PacMan_MapInfo,	// map location
    0,
    NULL,		// string information
    0,
    NULL,
};
struct GAMEINFO PacManJP =
{
   "Pac-Man (Namco Alt)",
   4,
   PacManJP_RomInfo,
   4, 
   PacMan_GfxInfo,
   12,
   PacMan_Palettes,
   &PacManJPData
};


struct ROMDATA PiranhaData =
{
    4,			// 4 more roms to load
    Piranha_DataRomInfo, // array of 4 roms
    1,			// one map
    PacMan_MapInfo,	// map location
    0,
    NULL,		// string information
    0,
    NULL,
};
struct GAMEINFO Piranha =
{
   "Piranha",
   4,
   Piranha_RomInfo,
   4, 
   PacMan_GfxInfo,
   12,
   PacMan_Palettes,
   &PiranhaData
};

/////////// Ms Pac stuff

struct TEXTED_STRING MsPacMan_Strings_1[] =
{
    {"Red Name",    0x3000+3434, 8},
    {"Pink Name",   0x3000+3465, 8},
    {"Aqua Name",   0x3000+3496, 8},
    {"Orange Name", 0x3000+3529, 6},
};

struct TEXTED_STRING MsPacMan_Strings_2[] =
{
    {"High Score", 0x3715, 10},
    {"Credit",     0x3725, 9},
    {"Free Play",  0x3734, 9},
    {"Player 1",   0x3743, 10},
    {"Player 2",   0x375c, 10},
    {"Game Over",  0x376c, 10},
    {"Ready!",     0x377c, 6},
    {"Press Start",0x3788, 17},
    {"1P Only",    0x379f, 14},
    {"1/2 Players",0x37b3, 14},
};

struct TEXTED_STRING MsPacMan_Strings_3[] =
{
    {"\"Ms PacMan\"",  0x37ff, 21},
    {"Additional...", 0x3d02, 14},
    {"...at ??000", 0x3d10, 13},
    {"copyright", 0x3d3e, 17},
    {"1980/1981", 0x3db6, 12},
    {"With", 0x3de5, 9},
    {"Starring", 0x3e04, 9},
    {"Ms. Pacman",  0x3d97, 10},
};

struct TEXTED_STRING MsPacMan_Strings_4[] =
{
    {"The Chase", 0x3df4, 9},
    {"They Meet", 0x3e42, 9},
    {"Junior",    0x3dd5, 9},
};

struct TEXTINFO MsPacMan_TextInfo[] =
{
    {"Ghost Names", 4,  MsPacMan_Strings_1, PacMan_Lookup},
    {"Screen Text 1", 10,  MsPacMan_Strings_2, PacMan_Lookup},
    {"Screen Text 2", 8,  MsPacMan_Strings_3, PacMan_Lookup},
    {"Intermission Text", 3,  MsPacMan_Strings_4, PacMan_Lookup},
};

char mspac_hc_1[] = { 1, 3,  26, 3,  1, 28,  26, 28};
char mspac_hc_2[] = { 1, 5,  26, 5,  1, 27,  26, 27};
char mspac_hc_3[] = { 1, 4,  26, 4,  1, 24,  26, 24};
char mspac_hc_4[] = { 1, 4,  26, 4,  1, 28,  26, 28};

struct MAPINFO MsPacMan_MapInfo[] =
{
    { 0x48c2, 0x4a3b, 220, 28, 32, 0, &PacMan_Functions, 4, mspac_hc_1},
    { 0x4baf, 0x4d27, 240, 28, 32, 0, &PacMan_Functions, 4, mspac_hc_2},
    { 0x4ea9, 0x5018, 238, 28, 32, 0, &PacMan_Functions, 4, mspac_hc_3},
    { 0x517a, 0x52ec, 234, 28, 32, 0, &PacMan_Functions, 4, mspac_hc_4},
};

/*
struct MAPINFO MsPacAtk_MapInfo[] =
{
    { 0x48c2, 0x4a3b, 234, 28, 32, 0, &PacMan_Functions, 4, mspac_hc_1},
    { 0x4baf, 0x4d27, 238, 28, 32, 0, &PacMan_Functions, 4, mspac_hc_2},
    { 0x4ea9, 0x5018, 240, 28, 32, 0, &PacMan_Functions, 4, mspac_hc_3},
    { 0x517a, 0x52ec, 220, 28, 32, 0, &PacMan_Functions, 4, mspac_hc_4},
};
*/

struct ROMDATA MsPacManData =
{
    6,			// 6 more roms to load
    MsPacMan_DataRomInfo, // array of 4 roms
    4,			// 4 maps
    MsPacMan_MapInfo,	// map location
    4,
    MsPacMan_TextInfo,	// string information
    0,
    NULL,
};

struct GAMEINFO MsPacMan = 
{
   "Ms.Pac-Man",
   2,
   MsPacMan_RomInfo,
   4, 
   PacMan_GfxInfo,
   12,
   PacMan_Palettes,
   &MsPacManData
};


struct ROMDATA MsPacAtkData =
{
    6,			// 6 more roms to load
    MsPacAtk_DataRomInfo, // array of 4 roms
    4,		
    MsPacMan_MapInfo,	// map location
    //MsPacAtk_MapInfo,	// map location
    4,
    MsPacMan_TextInfo,	// string information
    0,
    NULL,
};
struct GAMEINFO MsPacAtk =
{
   "Ms.Pac Attack",
   2,
   MsPacAtk_RomInfo,
   4, 
   PacMan_GfxInfo,
   12,
   PacMan_Palettes,
   &MsPacAtkData
};



////////////// Crush Roller //////////////

RGB Crush_Colours[] =
{
    {  0,  0,  0 },
    { 40, 40,  0 },
    {  0,  0, 40 },
    { 50,  0,  0 }
};

struct PALETTE Crush_Palettes[] =
{
    { 4, Crush_Colours }
};


struct GAMEINFO CrushRoller =  // also Make Trax (probably)
{
   "Crush Roller",
   4,
   Crush_RomInfo,
   4, 
   PacMan_GfxInfo,
   1,
   Crush_Palettes,
   0
};


////////////// Pengo & Variants //////////////

struct TEXTED_LOOKUP Pengo_Lookup[] =
{
    //{ascii, rom}
    // lowercase letters
    {'a',0x41}, {'b',0x42}, {'c',0x43}, {'d',0x44}, {'e',0x45}, {'f',0x46},
    {'g',0x47}, {'h',0x48}, {'i',0x49}, {'j',0x4a}, {'k',0x4b}, {'l',0x4c},
    {'m',0x4d}, {'n',0x4e}, {'o',0x4f}, {'p',0x50}, {'q',0x51}, {'r',0x52},
    {'s',0x53}, {'t',0x54}, {'u',0x55}, {'v',0x56}, {'w',0x57}, {'x',0x58},
    {'y',0x59}, {'z',0x5a},
    // uppercase letters
    {'A',0X41}, {'B',0X42}, {'C',0X43}, {'D',0X44}, {'E',0X45}, {'F',0X46},
    {'G',0X47}, {'H',0X48}, {'I',0X49}, {'J',0X4A}, {'K',0X4B}, {'L',0X4C},
    {'M',0X4D}, {'N',0X4E}, {'O',0X4F}, {'P',0X50}, {'Q',0X51}, {'R',0X52},
    {'S',0X53}, {'T',0X54}, {'U',0X55}, {'V',0X56}, {'W',0X57}, {'X',0X58},
    {'Y',0X59}, {'Z',0X5A},
    // numbers
    {'0',0x30}, {'1',0x31}, {'2',0x32}, {'3',0x33}, {'4',0x34}, {'5',0x35},
    {'6',0x36}, {'7',0x37}, {'8',0x38}, {'9',0x39},
    // punctuation
    {'.',0x3a}, {',',0x3b}, {'\"',0x3c}, {'!',0x3d}, {'\'',0x3e},
    {'-',0x3f}, {'@',0x40}, {'*',0x21}, {' ', 0x20},
    // end
    {0, 0x20}, // end character, and character to insert
};

RGB Pengo_Colours_1[] = // Ice block / diamond block
{ {  0,  0,  0 }, {  0, 50, 50 }, {  0,  0, 50 }, { 50, 50, 50 } };

RGB Pengo_Colours_2[] = // Pengo / Sno-bee
{ {  0,  0,  0 }, {  0, 50,  0 }, { 50,  0,  0 }, { 50, 50, 50 } };

struct PALETTE Pengo_Palettes[] =
{
    { 4, Pengo_Colours_1 }, // Ice Block / Diamond block
    { 4, Pengo_Colours_2 }, // Pengo / Sno-Bee
};

struct GFXROMINFO Pengo_RomInfo[] =
{
   { "pengo\\IC92", 0x0000, 0x2000 }, // each of these roms are 8k apiece
   { "pengo\\IC105", 0x2000, 0x2000 }
};

struct GFXROMINFO Pengoa_RomInfo[] =
{
   { "pengoa\\pengo.u92", 0x0000, 0x2000 }, // each of these roms are 8k apiece
   { "pengoa\\pengo.105", 0x2000, 0x2000 }
};

struct GFXROMINFO Pengoa_DataRomInfo[] =
{
   { "pengoa\\pengo.u8",  0x0000, 0x1000 },
   { "pengoa\\pengo.u7",  0x1000, 0x1000 },
   { "pengoa\\pengo.u15", 0x2000, 0x1000 },
   { "pengoa\\pengo.u14", 0x3000, 0x1000 },
   { "pengoa\\pengo.u21", 0x4000, 0x1000 },
   { "pengoa\\pengo.u20", 0x5000, 0x1000 },
   { "pengoa\\pengo.u32", 0x6000, 0x1000 },
   { "pengoa\\pengo.u31", 0x7000, 0x1000 },
};

struct GFXROMINFO Penta_RomInfo[] =
{
   { "penta\\092_PN09.bin", 0x0000, 0x2000 }, // each of these roms are 8k apiece
   { "penta\\105_PN10.bin", 0x2000, 0x2000 }
};

struct GFXINFO Pengo_GfxInfo[] = 
{
   { 0x0000, &PacMan_charlayout },
   { 0x0000, &PacMan_charspritelayout_2 }, // whee! this is useful...
   { 0x1000, &PacMan_spritelayout },
   { 0x3000, &PacMan_spritelayout },
   { 0x2000, &PacMan_charlayout },
};

struct GAMEINFO Pengo =
{
   "Pengo",
   2,
   Pengo_RomInfo,
   5, 
   Pengo_GfxInfo,
   2,
   Pengo_Palettes,
   0
};


struct TEXTED_STRING Pengoa_Strings_1[] =
{
    {"Todays Best",   0x27fa, 14},
    {"Enter Initial", 0x281d, 18},
    {"Start",         0x2a8a, 5},
    {"Thanks for..", 0x04df, 19},
    {"Try once...",  0x04fc, 16},
    {"Squash..",     0x057d, 18},
    {"Peng",         0x0593, 4},
    {"snow-be",      0x059b, 6},
    {"Ice bloc",     0x05a5, 8},
    {"Diamond Bloc", 0x05b1, 12},
};

struct TEXTED_STRING Pengoa_Strings_2[] =
{
    {"Game Time",    0x0b23, 23},
    {"00 to 19" ,    0x0B3e, 23},
    {"20 to 29" ,    0x0b59, 23},
    {"30 to 39" ,    0x0b74, 23},
    {"40 to 49" ,    0x0b8f, 23},
    {"50 to 59",     0x0baa, 23}, // moo!
    {"60 & over",    0x0bc5, 23},
};

struct TEXTINFO Pengoa_TextInfo[] =
{
    {"Screen Text", 10,  Pengoa_Strings_1, Pengo_Lookup},
    {"Scoring",     7,   Pengoa_Strings_2, Pengo_Lookup},
};

struct ROMDATA PengoaData =
{
    8,
    Pengoa_DataRomInfo,
    0,		
    NULL,
    3,
    Pengoa_TextInfo,	// string information
    0,
    NULL,
};

struct GAMEINFO Pengoa =
{
   "Pengo (Alternate)",
   2,
   Pengoa_RomInfo,
   5, 
   Pengo_GfxInfo,
   2,
   Pengo_Palettes,
   &PengoaData
};

struct GAMEINFO Penta =
{
   "Penta",
   2,
   Penta_RomInfo,
   5, 
   Pengo_GfxInfo,
   2,
   Pengo_Palettes,
   0
};

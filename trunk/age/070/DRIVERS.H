#include "allegro.h"
//
// Definitions of driver fields
//
//

struct MAPED_FUNCTIONS {
    void (* decode)();
    void (* encode)();
    void (* make_play_base)(BITMAP * playfield);
    void (* make_play_over)(BITMAP * playfield);
    void (* make_play_hc)(BITMAP * playfield);
    int  (* valid_overpos)(int x, int y);
    void (* display_info)(int mode);
};
extern struct MAPED_FUNCTIONS * maped_current;


struct GFXROMINFO
{
   char *ROMName;	// the name of the rom file
   long LoadAddress;	// where to load it into the allocated memory
   long Size;		// how big the file is
};


struct GFXLAYOUT
{
   int width,height;	/* width and height of chars/sprites */
   int total;	/* total numer of chars/sprites in the rom */
   int planes;	/* number of bitplanes */
   int planeoffset[8];	/* start of every bitplane */
   int xoffset[64];	/* coordinates of the bit corresponding to the pixel */
   int yoffset[64];	/* of the given coordinates */
   int charincrement;	/* distance between two consecutive characters/sprites */
};


struct GFXINFO
{
   long StartAddress;	// offset of start of this bank
   struct GFXLAYOUT * GfxLayout;  // layout of this bank
};

struct PALETTE
{
   int NumColours;	// how many colours the following structure
   struct RGB * Colours;	// suitable colours for display
};


// text string related information
struct TEXTED_STRING{ // one of these for each string
    char * stringname;		// text clue for the string (12 is max)
    unsigned long startpos;	// start pos in loaded rom
    unsigned long length;	// length of string in bytes (23 is max)
};

struct TEXTED_LOOKUP{ // lookup table for the characters...
    char ascii; // the user input
    char rom;   // the associated rom character (could be 1:1)
};

struct TEXTINFO{ // one of these for each "category"
    char * category_name;	    // name for the category
    int numStrings;		    // number of strings to follow
    struct TEXTED_STRING * strings; // pointer to an array of the above struct
    struct TEXTED_LOOKUP * lu;      // pointer to the above struct
};


// attribute editor information
struct ATTRIBUTEINFO{
    char * attr_name;	// name of the attribute
    char * info;	// more info (null for none)
    long spos;		// start position
    int size;		// number of bytes (1/2/4)->(byte/word/dword)
};

/* ie for pacman:
{ "fruit 1 shape", 0x0efd, 1},
{ "fruit 1 color", 0x0efe, 1},
{ "fruit 1 score", 0x0eff, 1}
*/

// map related information
struct MAPINFO
{
    long StartAddress; 	// start address in the roms
    long OverlayAddr;   // start address of the overlay data in the roms
    int OverlayCount;   // number for overlay count (!)
    int width; 		// number of characters wide
    int height;		// number of characters high
    int gfx_bank_no;	// which bank number does the map use?
    struct MAPED_FUNCTIONS * funcs;  // the map control functions
    int numHardcoded;
    char * hardcoded;   // array of x/y positions for hardcoded bits
};

struct EXTROMDATA // extra rom information
{
    // for map data
    int numMaps;
    struct MAPINFO * MapData;
    int numCategories;
    struct TEXTINFO * TextData;
    int numAttribs;
    struct ATTRIBUTEINFO * AttData;
};

#define MAX_COLOUR_PLANES  (5)

// orientation values - taken from mame src\driver.h
#define	ORIENTATION_DEFAULT		0x00
#define	ORIENTATION_FLIP_X		0x01	/* mirror everything in the X direction */
#define	ORIENTATION_FLIP_Y		0x02	/* mirror everything in the Y direction */
#define ORIENTATION_SWAP_XY		0x04	/* mirror along the top-left/bottom-right diagonal */
#define	ORIENTATION_ROTATE_90	(ORIENTATION_SWAP_XY|ORIENTATION_FLIP_X)	/* rotate clockwise 90 degrees */
#define	ORIENTATION_ROTATE_180	(ORIENTATION_FLIP_X|ORIENTATION_FLIP_Y)	/* rotate 180 degrees */
#define	ORIENTATION_ROTATE_270	(ORIENTATION_SWAP_XY|ORIENTATION_FLIP_Y)	/* rotate counter-clockwise 90 degrees */


struct GAMEINFO
{
   char GameName[30];	// the textual name of the game for the load select
   int NumGfxRoms;	// number of roms used by this game
   int Orientation;  // the orientation of the graphics - see above
   struct GFXROMINFO * GfxRoms;	// array of roms - see above
   int NumGfxBanks;	// Number of graphics banks
   struct GFXINFO * GfxInfo; // graphics bank info -see above
   int NumPalettes[MAX_COLOUR_PLANES];	// how many colour palettes per (plane -1) i.e. 2 planes is NumPalettes[1]
   struct PALETTE * Palettes[MAX_COLOUR_PLANES]; // colour palettes per (plane -1) -see above 
    // for the data filenames & locations
    int numDataRoms;
    struct GFXROMINFO * DataRoms;

};

extern struct GAMEINFO Driver;

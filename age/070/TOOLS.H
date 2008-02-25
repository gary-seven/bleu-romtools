// bitmap manipulations tools
// November, 1997 jerry@mail.csh.rit.edu

// Manipulation Operations
enum {
   TOOLS_CLEAR_BITMAP,  /* these must be in the same order as the */
   TOOLS_FLOOD_FILL_FG, /* text list in tools.c !!! */
   TOOLS_FLOOD_FILL_BG,
   TOOLS_EYEDROPPER_FG,
   TOOLS_EYEDROPPER_BG,
   TOOLS_VERT_FLIP,
   TOOLS_HORIZ_FLIP,
   TOOLS_CW_ROTATE,
   TOOLS_CW_ROTATE_45,
   TOOLS_SLIDE_UP,
   TOOLS_SLIDE_DOWN,
   TOOLS_SLIDE_LEFT,
   TOOLS_SLIDE_RIGHT,
   TOOLS_SWAP_CHARS,
   
   MAX_TOOLS // this entry must always be last!!!
};

#define TOOLS_WRAP_UP    (MAX_TOOLS +1)
#define TOOLS_WRAP_DOWN  (MAX_TOOLS +2)
#define TOOLS_WRAP_LEFT  (MAX_TOOLS +3)
#define TOOLS_WRAP_RIGHT (MAX_TOOLS +4)
#define TOOLS_PAINT_FG   (MAX_TOOLS +5)
#define TOOLS_PAINT_BG   (MAX_TOOLS +6)
#define TOOLS_CCW_ROTATE_45 (MAX_TOOLS +7)


// bring up the manipulations requestor.  it returns one of the above values.
int ToolSelector(void);

// do the selected manipulation, on the CurrentChar, and in location x,y
// if applicable (send in 0's if not applicable) - only used in flood fill
// so far...
void do_tool(int selection, BITMAP * CurrentChar, int x, int y);

// take the bitmap in CurChar, blit it to the screen in the preview location,
// then enlarge it appropriately to the zoom/edit view
void Blit_And_Zoom(BITMAP * CurChar, struct GFXLAYOUT *gl);

void GetImageFromSet(struct GFXLAYOUT * gl, int SelectedChar, BITMAP * dest);

void SnapRomsetBitmap(void);
int GetSpriteFromPCX(void);

// used by orientation code in coding and decoding romsets
void tools_flip(BITMAP * CurChar, int direction);
void tools_cw_rotate(BITMAP * CurChar);

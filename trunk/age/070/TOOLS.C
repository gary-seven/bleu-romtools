// bitmap manipulations tools
// November, 1997 jerry@mail.csh.rit.edu
#include <stdio.h>
#include <conio.h>

#include "allegro.h"

#include "drivers.h"
#include "age.h"
#include "ini.h"
#include "mouse.h"
#include "palette.h"
#include "utils.h"
#include "tools.h"
#include "snap.h"
#include "maped.h"      // for shift palette

/* bitmap manipulations & tools
**   jerry@mail.csh.rit.edu  November 1997
**
**  based vaguely on Deluxe Paint operations
   DPaint II commands: (from the Amiga version)
Brush commands:
   F1 matte 	F2 color 	F3 replace 	f4 smear 	f5 shade
   f6 blend 	f7 cycle 	-  smaller 	=  larger 	Z  stretch
   h  halve 	H  double 	x  flip h 	y  flip v 	z  90 deg rot
Toolbox commands:
   b  brush selector 		B  restore last custom brush
   c  unfilled circle tool 	C  filled circle tool
   d  continuous freehand tool 	D  continuous freehand tool w/ 1 pixel brush
   e  unfilled ellipse tool 	E  filled ellipse tool
   f  fill tool 		F  brings up fill requestor
   g  grid toggle 		G  grid to current brush position
   j  spare/swap page 		K  Clear screen
   m  magnify tool 		p  palette requester
   q  curve tool 		r  unfilled rectangle tool
   R  filled rectangle tool 	s  dotted freehand tool
   t  text 			u  undo
   v  straight line tool 	,  pick color tool (eyedropper)
   .  one pixel brush, dotted line tool
   [] cycle through foreground colors
   {} cycle through background colors
*/

/*
   for the manips selector, we should have an on-screen requestor,
   showing icons of each function... maybe just for edit mode features
   like "line", "box", and "flood fill"
*/

#define ARBITRARY_CHAR (126) /* this should be less than 127 (256/2) */


// these are for the wrap/slide function, and are not necessarily public
#define CHOOSE_SLIDE 1
#define CHOOSE_WRAP  2

// the lookup table for the keyboard shortcuts...
struct lookup_list Tools_Lookups[] = {
    {'K', TOOLS_CLEAR_BITMAP},
    {'k', TOOLS_CLEAR_BITMAP},

    {'f', TOOLS_FLOOD_FILL_FG},
    {'F', TOOLS_FLOOD_FILL_BG},

    {'p', TOOLS_PAINT_FG},
    {'P', TOOLS_PAINT_BG},

    {'e', TOOLS_EYEDROPPER_FG},
    {'E', TOOLS_EYEDROPPER_BG},

    {'Y', TOOLS_VERT_FLIP},
    {'y', TOOLS_VERT_FLIP},
    {'V', TOOLS_VERT_FLIP},
    {'v', TOOLS_VERT_FLIP},

    {'X', TOOLS_HORIZ_FLIP},
    {'x', TOOLS_HORIZ_FLIP},
    {'H', TOOLS_HORIZ_FLIP},
    {'h', TOOLS_HORIZ_FLIP},

    {'z', TOOLS_CW_ROTATE},
    {'Z', TOOLS_CW_ROTATE},
    {'w', TOOLS_CW_ROTATE_45},
    {'W', TOOLS_CCW_ROTATE_45},

    {'u', TOOLS_SLIDE_UP},
    {'d', TOOLS_SLIDE_DOWN},
    {'l', TOOLS_SLIDE_LEFT},
    {'r', TOOLS_SLIDE_RIGHT},

    {'U', TOOLS_WRAP_UP},
    {'D', TOOLS_WRAP_DOWN},
    {'L', TOOLS_WRAP_LEFT},
    {'R', TOOLS_WRAP_RIGHT},

    {'S', TOOLS_SWAP_CHARS},
    {'s', TOOLS_SWAP_CHARS},

    {ARBITRARY_CHAR, ARBITRARY_CHAR}
};


// text string matching manipulations above
static char * ToolNames[] = {
    // the leftmost character is the Keyboard shortcut
    // these MUST be in the same order as the ENUM in tools.h!
    "k  Clear        ",
    "f  Flood Fill FG",
    "F  Flood Fill BG",
    "e  Eyedropper FG",
    "E  Eyedropper BG",
    "y  Vert Flip    ",
    "x  Horiz Flip   ",
    "z  CW Rotate    ",
    "w  Rotate CW 45 ",
    "u  Slide Up     ",
    "d  Slide Down   ",
    "l  Slide Left   ",
    "r  Slide Right  ",
    "s  Swap Chars   "
};


void Blit_And_Zoom(BITMAP * CurChar, struct GFXLAYOUT *gl)
{
	ZoomChar();
	
    // display char
    blit(CurChar, screen, 0,0, 109,18, gl->width, gl->height);
}


void tools_clear_to_background(BITMAP * CurChar)
{
   // Clear bitmap and current drawing
    clear_to_color(CurChar, PALETTEBASE+GetRightMouseColour());

   // we need to clear the zoom window...
	 ClearZoomWindow();
}

void tools_flood_fill(BITMAP * CurChar, int x, int y, int fgbg)
{
    if (fgbg == TOOLS_FLOOD_FILL_FG)
	floodfill(CurChar, x, y, PALETTEBASE + GetLeftMouseColour());
    else if (fgbg == TOOLS_FLOOD_FILL_BG)
	floodfill(CurChar, x, y, PALETTEBASE + GetRightMouseColour());
}

void tools_eyedropper(BITMAP * CurChar, int x, int y, int fgbg)
{
    if (fgbg == TOOLS_EYEDROPPER_FG)
	SetLeftMouseColour( getpixel(CurChar, x,y) - PALETTEBASE);
    else if (fgbg == TOOLS_EYEDROPPER_BG)
	SetRightMouseColour( getpixel(CurChar, x,y) - PALETTEBASE);
}

void tools_paint(BITMAP * CurChar, int x, int y, int fgbg)
{
    if (fgbg == TOOLS_PAINT_FG)
	putpixel(CurChar, x,y, GetLeftMouseColour() + PALETTEBASE);
    else if (fgbg == TOOLS_PAINT_BG)
	putpixel(CurChar, x,y, GetRightMouseColour() + PALETTEBASE);
}

void tools_flip(BITMAP * CurChar, int direction)
{
    struct GFXLAYOUT * gl = Driver.GfxInfo[CurrentBank].GfxLayout;
    BITMAP * tbmp = create_bitmap(gl->width,gl->height);

    // do the necessary manipulation on CurChar

    // let allegro flip it for us!  whee!
    blit(CurChar, tbmp, 0,0, 0,0, gl->width,gl->height);
    clear_to_color(CurChar, 0 /*PALETTEBASE+GetRightMouseColour()*/);
    // we need to clear these bitmaps in here, cause the sprite routine
    // is basically a blit with transparency... good for games, bad for us.
    if (direction == TOOLS_VERT_FLIP)
	draw_sprite_v_flip(CurChar, tbmp, 0, 0);
    else if (direction == TOOLS_HORIZ_FLIP)
	draw_sprite_h_flip(CurChar, tbmp, 0, 0);
    else
	draw_sprite_vh_flip(CurChar, tbmp, 0, 0);
    destroy_bitmap(tbmp);
}

void tools_cw_rotate(BITMAP * CurChar)
{
    int sx,sy;
    struct GFXLAYOUT * gl = Driver.GfxInfo[CurrentBank].GfxLayout;
    BITMAP * tbmp = create_bitmap(gl->width,gl->height);

    // NOTE: I couldn't use allegro's rotate sprite function.  Allegro's
    //       function worked fine for rotating the bitmap, but it was
    //       inaccurate for rotating such small bitmaps with precsision.
    //       If you were to do a rotate 90 four times, you would lose
    //       the upper and leftmost row of pixels.  Yow!  (And yes, I was
    //       using the function properly)  This is a couple lines of
    //       code more than that, but it works flawlessly on any size
    //       bitmap/sprite.  -jerry

    blit(CurChar, tbmp, 0,0, 0,0, gl->width,gl->height);
    for(sx=0 ; sx<gl->width ; sx++)
	for(sy=0 ; sy<gl->height ; sy++)
	    putpixel(CurChar, gl->width-sx-1, sy, getpixel(tbmp, sy,sx));

    destroy_bitmap(tbmp);
}

void tools_rotate_angle(BITMAP * CurChar, int degrees)
{
    struct GFXLAYOUT * gl = Driver.GfxInfo[CurrentBank].GfxLayout;
    BITMAP * tbmp = create_bitmap(gl->width,gl->height);

    // do the necessary manipulation on CurChar

    // let allegro rotate it for us!  whee!
    blit(CurChar, tbmp, 0,0, 0,0, gl->width,gl->height);

    // we need to clear these bitmaps in here, cause the sprite routine
    // is basically a blit with transparency... good for games, bad for us.
    clear_to_color(CurChar, PALETTEBASE+GetRightMouseColour());

    rotate_sprite(CurChar, tbmp, 0, 0, itofix(degrees));

    destroy_bitmap(tbmp);
}

void tools_slide(BITMAP * CurChar, int osx, int osy, int odx, int ody, int fmt)
{
    struct GFXLAYOUT * gl = Driver.GfxInfo[CurrentBank].GfxLayout;
    BITMAP * tbmp = create_bitmap(gl->width,gl->height);

    // let allegro do it for us!  whee!
    blit(CurChar, tbmp, 0,0, 0,0, gl->width,gl->height);
    clear_to_color(CurChar, PALETTEBASE+GetRightMouseColour());
    blit(tbmp, CurChar, osx,osy, odx,ody, gl->width,gl->height);

    if (fmt == CHOOSE_WRAP) {
	if (osx) blit(tbmp, CurChar, 0,0,  gl->width - osx, 0, gl->width, gl->height);
	if (odx) blit(tbmp, CurChar, 0,0, -gl->width + odx, 0, gl->width, gl->height);
	if (osy) blit(tbmp, CurChar, 0,0, 0, gl->height - osy, gl->width, gl->height);
	if (ody) blit(tbmp, CurChar, 0,0, 0,-gl->height + ody, gl->width, gl->height);
    }

    destroy_bitmap(tbmp);
}


// these next few functions are for the function tools_swap_chars()

void GetImageFromSet(struct GFXLAYOUT * gl, int SelectedChar, BITMAP * dest)
{
   int pos, x, y;

   pos = SelectedChar * (gl->width * gl->height);
   for (y = 0; y < gl->height; y ++)
   {
      for (x = 0; x < gl->width; x ++)
      {
	 putpixel(dest,x,y, DecodedSet[pos]); //+PALETTEBASE);
	 pos++;
      }
   }
}

void PutImageBackInSet(int SelectedChar, BITMAP * source)
{
   struct GFXLAYOUT * gl = Driver.GfxInfo[CurrentBank].GfxLayout;
   int pos, x, y;

   pos = SelectedChar * (gl->width * gl->height);
   for (y = 0; y < gl->height; y ++)
   {
      for (x = 0; x < gl->width; x ++)
      {
	 DecodedSet[pos] = getpixel(source, x, y);
	 pos++;
      }
   }

   DisplayFont(gl,0);
}

void tools_swap_chars(void)
{
    struct GFXLAYOUT * gl = Driver.GfxInfo[CurrentBank].GfxLayout;
    int first_char;
    int second_char;
    BITMAP * tbmp1 = create_bitmap(gl->width,gl->height);
    BITMAP * tbmp2 = create_bitmap(gl->width,gl->height);
    BITMAP * Background = create_bitmap(260,28);

    // store the bitmap under where we're going to put the box
    blit(screen, Background, 30,30, 0,0, 260,28);
    rectfill(screen, 30,30,289,57, 0);
    rect(screen, 30,30, 289,57, 2);
    rect(screen, 31,31, 288,56, 1);

    textout_centre(screen, font, "Select the first character", 160, 40, 2);

    // select the first character
    first_char = SelectChar(gl);
    if (first_char != -1) {
	wait_for_button_release();
    
	rectfill(screen, 32,32,287,55, 0);
	textout_centre(screen, font, "Select the second character",160, 40, 2);

	// select the second character
	second_char = SelectChar(gl);
	if (second_char != -1) {
	    wait_for_button_release();
	
	    GetImageFromSet(gl,first_char,tbmp1);
	    GetImageFromSet(gl,second_char,tbmp2);
	    PutImageBackInSet(first_char,tbmp2);
	    PutImageBackInSet(second_char,tbmp1);
	}
    }
    blit(Background, screen, 0,0, 30,30, 260,28);
    destroy_bitmap(Background);
    destroy_bitmap(tbmp1);
    destroy_bitmap(tbmp2);
}


//////////////////////////////////////

int ToolSelector(void)
{
    BITMAP * Background = create_bitmap(164,162);
    static int Selection = 0;
    int tempsel = 0;
    int key, key2, exit = 0;
    int i;
    int MouseDir = 0, MouseButt = 0;

	if (Selection == -1) Selection = 0;
    
    // copy image of the screeen
    blit(screen, Background, 78,23, 0,0, 164,162);
    
    // draw load selector box containing current drivers
    rectfill(screen, 78,23,241,184, 0);
    rect(screen, 78,23,241,184, 1);
    
    // drawe title
    textout_centre(screen, font, "Bitmap Tools", 160, 25, 2);
    
    // draw options
    for (i = 0; i < MAX_TOOLS; i ++)
	textout_centre(screen, font, ToolNames[i], 160, 35 + (i * 10), 1);
    
    // box the first entry for selection
	rect(screen, 79, 34 + (Selection * 10), 240, 43 + (Selection * 10), 2);
    
    // allow selection
    while(!exit) {
	while (!kbhit() && MouseDir == 0 && MouseButt == 0) {
	    MouseDir = mouse_move();
	    MouseButt = mouse_butt();
	}
	
	key = 0;
	
	if (MouseDir == 0 && MouseButt == 0)
	    key = getch(); 
	
	/* if button pressed simulate space */
	if (MouseButt > 0) {
	    key = 32;
	    MouseButt = 0;
	}
	
	switch(key) {
	case 0:
	    // cursor key
	    if (MouseDir == 0) {
		key2 = getch();
	    } else {
		key2 = ConvertMouseDirection(MouseDir);
		MouseDir = 0;
	    }
	    
	    switch(key2) {
	    case F12:
		screen_snap();
		break;

	    case CURSOR_UP:
		// up
		if (Selection > 0) {
		    // remove old selection
		    rect(screen, 79, 34 + (Selection * 10), 240, 43 + (Selection * 10), 0);
		    Selection --;
		    //draw new
		    rect(screen, 79, 34 + (Selection * 10), 240, 43 + (Selection * 10), 2);
		}
		break;  
	    
	    case CURSOR_DOWN:
		// down
		if (Selection < MAX_TOOLS-1) {
		    // remove old selection
		    rect(screen, 79, 34 + (Selection * 10), 240, 43 + (Selection * 10), 0);
		    Selection ++;
		    //draw new
		    rect(screen, 79, 34 + (Selection * 10), 240, 43 + (Selection * 10), 2);
		}
		break;  
		
	    }
	    break;

	case 27: // escape to abort
	    exit = 1;
	    Selection = -1;
	    break;
	
	case 32: // space to select
	case 13: // enter to select
	    exit = 1;
	    break;

	default:
	    tempsel = lookup_translation(key, Tools_Lookups, ARBITRARY_CHAR);
	    if (tempsel != ARBITRARY_CHAR)
	    {
		// it was found in the lookup table, so exit.
		Selection = tempsel;
		exit = 1;
	    }
	    break;
	}

    }
   
   
    // restore screen
    blit(Background, screen, 0,0, 78,23, 164,162);
    destroy_bitmap(Background);
    
    // return selection number -1 signals no selection
    return Selection;
}



void do_tool(int selection, BITMAP * CurrentChar, int x, int y)
{
    switch(selection) {
    case(TOOLS_VERT_FLIP): // vertical Flip
	tools_flip(CurrentChar, TOOLS_VERT_FLIP);
	break;
    
    case(TOOLS_HORIZ_FLIP): // horizontal Flip
	tools_flip(CurrentChar, TOOLS_HORIZ_FLIP);
	break;
    
    case(TOOLS_CW_ROTATE): // cw rotate
	tools_cw_rotate(CurrentChar);
	break;
	
    case(TOOLS_CW_ROTATE_45): // cw rotate 45
	tools_rotate_angle(CurrentChar, 32);
	break;

    case(TOOLS_CCW_ROTATE_45): // ccw rotate 45
	tools_rotate_angle(CurrentChar, -32);
	break;
	
    case(TOOLS_SLIDE_UP): // slide up
	tools_slide(CurrentChar, 0,1,0,0, CHOOSE_SLIDE);
	break;
	
    case(TOOLS_SLIDE_DOWN): // slide down
	tools_slide(CurrentChar, 0,0,0,1, CHOOSE_SLIDE);
	break;
	
    case(TOOLS_SLIDE_LEFT): // slide left
	tools_slide(CurrentChar, 1,0,0,0, CHOOSE_SLIDE);
	break;
    
    case(TOOLS_SLIDE_RIGHT): // slide right
	tools_slide(CurrentChar, 0,0,1,0, CHOOSE_SLIDE);
	break;
    
    case(TOOLS_WRAP_UP): // wrap up
	tools_slide(CurrentChar, 0,1,0,0, CHOOSE_WRAP);
	break;
	
    case(TOOLS_WRAP_DOWN): // wrap down
	tools_slide(CurrentChar, 0,0,0,1, CHOOSE_WRAP);
	break;
	
    case(TOOLS_WRAP_LEFT): // wrap left
	tools_slide(CurrentChar, 1,0,0,0, CHOOSE_WRAP);
	break;

    case(TOOLS_WRAP_RIGHT): // wrap right
	tools_slide(CurrentChar, 0,0,1,0, CHOOSE_WRAP);
	break;

    case(TOOLS_FLOOD_FILL_FG): // flood fill
    case(TOOLS_FLOOD_FILL_BG): // flood fill
	tools_flood_fill(CurrentChar, x, y, selection);
	break;

    case(TOOLS_SWAP_CHARS): // swap chars
	tools_swap_chars();
	break;

    case (TOOLS_CLEAR_BITMAP): // clear bitmap
	tools_clear_to_background(CurrentChar);
	break;

    case (TOOLS_EYEDROPPER_FG): // eyedropper (get color)
    case (TOOLS_EYEDROPPER_BG): // eyedropper (get color)
	tools_eyedropper(CurrentChar, x, y, selection);
	break;
       
    case (TOOLS_PAINT_FG): // paint (set color)
    case (TOOLS_PAINT_BG): // paint (set color)
	tools_paint(CurrentChar, x, y, selection);
	break;
       
    default:
	break;
    }
    
    // re-draw the character
    Blit_And_Zoom(CurrentChar, Driver.GfxInfo[CurrentBank].GfxLayout);
    // UpdateScratch(CurrentChar);
}


/////////////////////////////////////////////////////////////////////////////

// the next functions deal with the loading and saving of rom sets
// to a
#define TEMPNAME "romfile.pcx"

void SnapRomsetBitmap(void)
{
    struct GFXLAYOUT *gl = Driver.GfxInfo[CurrentBank].GfxLayout;
    RGB tpal[256];
    RGB trgb;
    int TotCols;
    int CharNo;
    int row = 0;
    int col = 0;
    int x,y;
    int maxcol = 0;
    int maxrow = 0;
    BITMAP * rsb;
    BITMAP * tch;

    TotCols = (256/gl->width);
    // compute the number of rows & columns
    maxcol = 320 / gl->width;
    if (maxcol < 8) maxcol = 4;
    else if (maxcol < 16) maxcol = 8;
    else if (maxcol < 32) maxcol = 16;
    else maxcol = 32;

    maxrow = gl->total / maxcol;

    rsb = create_bitmap(maxcol*gl->width, maxrow*gl->height);
    tch = create_bitmap(gl->width, gl->height);
    if (tch == NULL)
    {
	if (rsb) destroy_bitmap(rsb);
	return;
    }
    if (rsb == NULL)
    {
	if (tch) destroy_bitmap(tch);
	return;
    }

    clear(rsb);

    for (CharNo = 0; CharNo < gl->total ; CharNo++)
    {
	// get the character/sprite
	BlitChar(tch,0,0, gl->width, gl->height, CharNo);

	// right here - subtract (from each pixel) PALETTEBASE
	for(x=0 ; x<tch->w; x++)
	    for(y=0 ; y<tch->h ; y++)
		putpixel(tch, x,y, (getpixel(tch, x,y) - PALETTEBASE));

	// blit it to the bitmap
	blit(tch, rsb, 0, 0,
	     (col * gl->width), (row * gl->height),
	     gl->width, gl->height);

	col++;
	if(col >= TotCols)
	{
	    col =0;
	    row++;
	}
    }

    get_palette(tpal);
    for(x=0 ; x<255-PALETTEBASE ; x++)
    {
	get_color(x+PALETTEBASE, &trgb);
	tpal[x].r = trgb.r;
	tpal[x].g = trgb.g;
	tpal[x].b = trgb.b;
	tpal[x].filler = 0;
    }

    // create a new palette starting with PALETTEBASE as color 0
    save_pcx(TEMPNAME, rsb, tpal);

    destroy_bitmap(rsb);
    destroy_bitmap(tch);

    (void)Dialog_Box("Bitmap saved.", "Hit any key to continue.");

}


void mod_bitmap(BITMAP * bmp, int nplanes)
{
    int value = 1 << (nplanes+1);
    int x,y;

    // this is so that if you were to load a bitmap with too
    // many colors, it will reduce them down to the requested
    // colors.  perhaps a color selector should load up, and
    // allow the user to decide on colors to be swapped with
    // the source colors.
    // i hope that made sence... i'm half-asleep

    for (x=0 ; x<bmp->w ; x++)
	for (y=0 ; y<bmp->h ; y++)
	    putpixel(bmp, x,y, (getpixel(bmp, x,y)%value));
}

int increment = 1;

int GetSpriteFromPCX(void)
{
    int MouseDir = 0, MouseButt = 0;
    char temp_text[32];
    struct GFXLAYOUT *gl = Driver.GfxInfo[CurrentBank].GfxLayout;
    BITMAP * fbmp;
    BITMAP * fbmp2;
    BITMAP * fbmp3;
    BITMAP * bscr = create_bitmap(SCREEN_W, SCREEN_H);
    BITMAP * display = create_bitmap(SCREEN_W, SCREEN_H);
    BITMAP * tch = create_bitmap(gl->width, gl->height);
    PALETTE tpal;
    int yoff = 0;
    int xcpos = 0, ycpos = 0;
    int dest_char;
    int done;

    // store away the background...
    blit(screen, bscr, 0, 0, 0, 0, SCREEN_W, SCREEN_H);


    fbmp = load_bitmap(TEMPNAME, tpal);
    if (!fbmp)
    {
	destroy_bitmap(fbmp);
	destroy_bitmap(bscr);
	destroy_bitmap(tch);
	return -1;
    }
    mod_bitmap(fbmp, gl->planes); // reduce the colors of the original

    fbmp2 = create_bitmap(fbmp->w, fbmp->h);
    fbmp3 = create_bitmap(fbmp->w, fbmp->h);
    blit(fbmp, fbmp2, 0, 0, 0, 0, fbmp->w, fbmp->h);
    ShiftColor(fbmp2); // shift around the colors -- only do this once.

    // so the bitmaps to this point:
    //  fbmp the source bitmap
    //  fbmp2  the source bitmap, color shifted
    //  fbmp3  the source bitmap, color shifted, to have overlays added

    // first select the character to load into

	rectfill(screen, 30,30,289,57, 0);
	rect(screen, 30,30, 289,57, 2);
	rect(screen, 31,31, 288,56, 1);
    
	textout_centre(screen, font, "Select the destination...", 160, 40, 2);
	dest_char = SelectChar(gl);
	wait_for_button_release();
    
    if (dest_char == -1)
	done = 1;
    else
	done = 0;

    while (!done)
    {
	clear(display);
	textout_centre(display, font, title_text, 160, 1, PAL_LTGREY);
	blit(fbmp2, fbmp3, 0, 0, 0, 0, fbmp->w, fbmp->h);
	rect(fbmp3, xcpos-1, ycpos-1,
	    xcpos+gl->width, ycpos+gl->height, PAL_GREEN);

	sprintf(temp_text, "%2d", increment);
	textout(display, font, temp_text, 320-24, 1, PAL_GREEN);

	blit(fbmp3, display, 0, yoff, 0, 15, SCREEN_W, 176);

	MouseDir = mouse_move();
	MouseButt = mouse_butt();
	if (MouseDir || MouseButt){
	    if (MouseButt) {
		done = ' ';
		MouseDir = 0;
		MouseButt = 0;
		wait_for_button_release();
	    } 
	    if (MouseDir)
		done = 0;
	} else {
	    if (kbhit())
		done = getch();
	    else
		done = -1;
	    MouseDir = 0;
	    MouseButt = 0;
	}


	switch(done)
	{
	    case('S'):
	    case('s'):
		if (increment == 1)       increment = 2;
		else if (increment == 2)  increment = 4;
		else if (increment == 4)  increment = 8;
		else if (increment == 8)  increment = 16;
		else if (increment == 16) increment = 32;
		else if (increment == 32) increment = 64;
		else 
		    increment = 1;
		done = 0;
		break;

	    case(0):
		if (MouseDir)
		    done = ConvertMouseDirection(MouseDir);
		else
		    done = getch();

		switch (done){
		    case(CURSOR_LEFT):
			if (xcpos > increment-1)
			    xcpos -= increment;
			else
			    xcpos = 0;
			// auto scroll if necessary ... eventually
			done = 0;
			break;
		    case(CURSOR_RIGHT):
			if (xcpos < fbmp->w-gl->width-increment-1)
			    xcpos += increment;
			else
			    xcpos = fbmp->w-gl->width;
			// auto scroll if necessary ... eventually
			done = 0;
			break;
		    case(CURSOR_UP):
			if (ycpos > increment-1)
			    ycpos -= increment;
			else
			    ycpos = 0;
			done = 0;
			break;
		    case(CURSOR_DOWN):
			if (ycpos < fbmp->h-gl->height-increment-1) 
			    ycpos += increment;
			else
			    ycpos = fbmp->h-gl->height;
			done = 0;
			break;
    
		    case (HOME):
			yoff = 0;
			ycpos = 0;
			xcpos = 0;
			done = 0;
			break;

		    default:
			done = 0;
			break;
		}
		break;

#ifdef MANUAL_CONTROL
	    case('d'):
	    case('D'):
		yoff -= increment;
		if (yoff < 0) // limit it
		    yoff = 0;
		done = 0;
		break;

	    case('u'):
	    case('U'):
		if (fbmp->h-yoff >190) // limit it
		    yoff += increment;
		done = 0;
		break;
#endif


	    case(ESCAPE):
		break;

	    case(' '):
	    case(13):
		// select the character
		blit(fbmp, tch, xcpos, ycpos, 0, 0, gl->width, gl->height);
		PutImageBackInSet(dest_char,tch);
		Blit_And_Zoom(tch, gl);
		break;
	    default:
		done = 0;
	}
#ifndef MANUAL_CONTROL
	// auto scroll if necessary 
	if (ycpos < yoff)
	    yoff = ycpos;

	if (ycpos > yoff+190 - (gl->height*2) -1)
	    yoff += increment;
#endif

	blit(display, screen, 0, 0, 0, 0, SCREEN_W, SCREEN_H);
    }
    Save_INI();

    blit(bscr, screen, 0, 0, 0, 0, SCREEN_W, SCREEN_H);

    destroy_bitmap(fbmp3);
    destroy_bitmap(fbmp2);
    destroy_bitmap(fbmp);
    destroy_bitmap(bscr);
    destroy_bitmap(tch);
    return (dest_char);
}

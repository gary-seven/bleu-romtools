// this file contains code to handle the displaying and selections
// of colours from the palette.

#include <stdio.h>
#include <conio.h>
#include <allegro.h>  
#include "drivers.h"
#include "age.h"
#include "mouse.h"
#include "palette.h"
#include "help.h"

#define PALETTE_TOP	(58)  // x and y co-ords of top left colour (colour 0)
#define PALETTE_LEFT	(139)
#define RIGHT_COLOUR_X (108) 
#define RIGHT_COLOUR_Y (69) 
#define LEFT_COLOUR_X (RIGHT_COLOUR_X + 4) 
#define LEFT_COLOUR_Y (RIGHT_COLOUR_Y + 4) 
#define MAX_COLOURS_X	(4)
#define MAX_COLOURS_Y	(4)


static int LeftMouseColour = 1;
static int RightMouseColour = 0;
static int ColourColumns = 0;
static int PaletteNumber = 0;


void SetReservedColours(void)
{
   RGB grey = { 32, 32, 32 };
   RGB ltgrey = { 55, 55, 55 };
   RGB dkgrey = { 25, 25, 25 };
   RGB black = { 0, 0, 0 };
   RGB red = {63,0,0};
   RGB green = {0,40,0};
   RGB yellow = {63,63,0};
   RGB blue = {0,0,63};
   RGB cyan = {0,63,63};

   // set reserved colours
   set_color(PAL_BLACK, &black);
   set_color(PAL_GREY, &grey);
   set_color(PAL_LTGREY, &ltgrey);
   set_color(PAL_DKGREY, &dkgrey);
   set_color(PAL_RED, &red);
   set_color(PAL_YELLOW, &yellow);
   set_color(PAL_GREEN, &green);
   set_color(PAL_CYAN, &cyan);
   set_color(PAL_BLUE, &blue);
}


// the outer rect shows the current background colour as selected by the
// right mouse button. The inner is the foreground colour (left mouse button)
static void DrawMouseColours(void)
{
	rectfill(screen, RIGHT_COLOUR_X, RIGHT_COLOUR_Y,
				RIGHT_COLOUR_X + 15, RIGHT_COLOUR_Y + 15, PALETTEBASE + RightMouseColour);

	rectfill(screen, LEFT_COLOUR_X, LEFT_COLOUR_Y,
				LEFT_COLOUR_X + 7, LEFT_COLOUR_Y + 7, PALETTEBASE + LeftMouseColour);
}



// a tacky function but it does the job as long as
// MAX_COLOURS_Y remains set to 4.
static int ColumnsFromPlanes(int Planes)
{
	switch (Planes)
	{
	case 2:
		return 1;

	case 3:
		return 2;

	case 4:
		return 4;

	default:
		break;
	}

	return 4;
}


void SetColours(struct GFXLAYOUT * gl, int display_palette)
{	
   int i;
   RGB * pColours = Drivers[DriverNumber]->Palettes[PaletteNumber].Colours;
	RGB black = { 0, 0, 0 };

	// how many columns of colours do we need for this
	// graphics set
	ColourColumns = ColumnsFromPlanes(gl->planes);

	LeftMouseColour = 1;
	RightMouseColour = 0;
	if (display_palette)
	   InitialisePalette();
	
	// only works assuming max_colours_y remains at 4
   for (i = 0; i < ColourColumns * MAX_COLOURS_Y; i ++)
   {
      set_color(PALETTEBASE + i, &pColours[i]);
   }

	// blank out the rest of the colours
	for (; i < (MAX_COLOURS_X * MAX_COLOURS_Y); i ++)
	{
      set_color(PALETTEBASE + i, &black);
	}
}



// draws a 16 colour palette 
void InitialisePalette(void)
{
	int cx,cy;
	int DispColourColumns =
		MAX_COLOURS_X < ColourColumns ? MAX_COLOURS_X : ColourColumns;

	// blank out old outline and palette
	rectfill (screen, PALETTE_LEFT - 2, PALETTE_TOP - 2,
				 PALETTE_LEFT + (MAX_COLOURS_X * 10) -1,
				 PALETTE_TOP + (MAX_COLOURS_Y * 10) -1, 0);

	// outline palette
	rect (screen, PALETTE_LEFT - 2, PALETTE_TOP - 2,
			PALETTE_LEFT + (DispColourColumns * 10) -1,
			PALETTE_TOP + (MAX_COLOURS_Y * 10) -1, 2);

	// draw outline for current selections
	rect (screen, RIGHT_COLOUR_X - 2, RIGHT_COLOUR_Y - 2,
			RIGHT_COLOUR_X + 17, RIGHT_COLOUR_Y + 17, 2);
	
	
	// draw palette
	for (cx = 0; cx < DispColourColumns; cx ++)
	{
		for (cy = 0; cy < MAX_COLOURS_Y; cy ++)
		{
			rectfill(screen, PALETTE_LEFT + (cx * 10), PALETTE_TOP + (cy * 10),
						PALETTE_LEFT + (cx * 10) + 7, PALETTE_TOP + (cy * 10) + 7,
						PALETTEBASE + (cx * MAX_COLOURS_Y) + cy);
		}
	}

	DrawMouseColours();
}


// access functions to find out what the 2 drawing colours are
int GetLeftMouseColour(void)
{
	return LeftMouseColour;
}
int GetRightMouseColour(void)
{
	return RightMouseColour;
}
void SetLeftMouseColour(int colour)
{
	LeftMouseColour = colour;
	DrawMouseColours();
}
void SetRightMouseColour(int colour)
{
	RightMouseColour = colour;
	DrawMouseColours();
}



static void DrawCursor(int x, int y, int Colour)
{
   rect(screen, PALETTE_LEFT + (x*10) -1, PALETTE_TOP + (y*10) -1,
		  PALETTE_LEFT + (x*10) +8, PALETTE_TOP + (y*10) +8, Colour);
}




// incs/decs or resets PaletteNumber.
// return 1 for success, 0 for failure
int ModifyPaletteNumber(int No)
{
	int Max = Drivers[DriverNumber]->NumPalettes;
	
	// reset palette number - always succeeds
	if (No == 0)
	{
		PaletteNumber = 0;
		return 1;
	}

	// check limits
	if (((PaletteNumber + No) < 0) || ((PaletteNumber + No) >= Max))
		return 0;

	// everything OK
	PaletteNumber += No;
	return 1;
}


void SelectColour(void)
{
	int exit = 0;
	static int y = 0;
	static int x = 0;
	int key;
	int MouseDir = 0, MouseButt = 0;
	char str_cnum[40];

	// is x position invalid for this palette
	if (x > ColourColumns  -1)
		x = 0;

	// box the first entry for selection
	DrawCursor(x, y, 2);

	// allow selection
	do
	{      
		// JERRY added 11/18/97
		sprintf(str_cnum, " 0x%02X ", y + (x * MAX_COLOURS_Y) );
		textout_centre(screen, font, str_cnum, 160, 100, 2);

		while (!kbhit() && MouseDir == 0 && MouseButt == 0)
		{
			MouseDir = mouse_move();
			MouseButt = mouse_butt();
		}

		key = 0;

		if (MouseDir == 0 && MouseButt == 0)
			key = getch(); 

		/* if button pressed simulate space */
		// JERRY
		if (MouseButt & MOUSE_LMB) key = 32;
		if (MouseButt & MOUSE_RMB) key = 'z';
		MouseButt = 0;

		switch(key)
		{
		case 0:
		{
			int key2;
			if (MouseDir == 0)
			{
				key2 = getch();
			}
			else
			{
				key2 = ConvertMouseDirection(MouseDir);
				MouseDir = 0;
			}

			switch(key2)
			{
            case KEY_F1:
               DisplayHelp(HELP_SELECT_COLOUR);
               break;
               
			case CURSOR_UP:
				if (y > 0)
				{
					// remove old selection
					DrawCursor(x, y, 0);
					y --;
					//draw new
					DrawCursor(x, y, 2);
				}
				break;  

			case CURSOR_DOWN:
				if (y < (MAX_COLOURS_Y -1))
				{
					// remove old selection
					DrawCursor(x, y, 0);
					y ++;
					//draw new
					DrawCursor(x, y, 2);					
				}
				break;  

			case CURSOR_LEFT:
				if (x > 0)
				{
					// remove old selection
					DrawCursor(x, y, 0);
					x --;
					//draw new
					DrawCursor(x, y, 2);
				}
				break;  

			case CURSOR_RIGHT:
				if ((x < MAX_COLOURS_X -1) && (x < ColourColumns  -1))
				{
					// remove old selection
					DrawCursor(x, y, 0);
					x ++;
					//draw new
					DrawCursor(x, y, 2);					
				}
				break;  
			}
		}
		break;

		case ',': // decrease palette number
			if (ModifyPaletteNumber(-1))
			{
				SetColours(Drivers[DriverNumber]->GfxInfo[CurrentBank].GfxLayout,1);
			}
			break;

		case '.': // increase palette number
			if (ModifyPaletteNumber(1))
			{
				SetColours(Drivers[DriverNumber]->GfxInfo[CurrentBank].GfxLayout,1);
			}
			break;
			
		case 32: // left mouse/space bar colour selected
			LeftMouseColour = y + (x * MAX_COLOURS_Y);
			DrawMouseColours();
			exit = 1;
			break;
			
		case 'z': // left mouse/space bar colour selected
		case 'Z':
			RightMouseColour = y + (x * MAX_COLOURS_Y);
			DrawMouseColours();
			exit = 1;
			break;

			case 27: // escape or space to select
			exit = 1;
			break;
		}

	} while(!exit);

	// JERRY added 11/18/97
	textout_centre(screen, font, "      ", 160, 100, 2);

	// remove cursor
	DrawCursor(x, y, 0);
}
	

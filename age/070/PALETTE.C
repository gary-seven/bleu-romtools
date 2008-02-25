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
#include "fileio.h"

#define PALETTE_TOP	(58)  // x and y co-ords of top left colour (colour 0)
#define PALETTE_LEFT	(139)
#define RIGHT_COLOUR_X (108) 
#define RIGHT_COLOUR_Y (69) 
#define LEFT_COLOUR_X (RIGHT_COLOUR_X + 4) 
#define LEFT_COLOUR_Y (RIGHT_COLOUR_Y + 4) 
#define MAX_COLOURS_X	(4)
#define MAX_COLOURS_Y	(4)
#define DIALOG_X (150)
#define DIALOG_Y (30)
#define DIALOG_CX (75)
#define DIALOG_CY (75)

static void PaletteDialog(int ColIndex);


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


// returns the plane number -1 which is used as a index into the
// 2D array of palette stuff. i.e. a game with a 2 plane graphics
// set will have its palette stuff stored in Driver.NumPalettes[2 - 1]
// and Drivers.Palettes[2 - 1][PaletteNumber].Colours
int GetPlanesMinus1(void)
{
   return (Driver.GfxInfo[CurrentBank].GfxLayout->planes - 1);
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
   int Index = GetPlanesMinus1();
   
   RGB * pColours = Driver.Palettes[Index][PaletteNumber].Colours;
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
	int Max = Driver.NumPalettes[GetPlanesMinus1()];
	
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

void GetPaletteInfo(int * CurPalette, int * MaxPalette)
{
   *MaxPalette = Driver.NumPalettes[GetPlanesMinus1()];
   *CurPalette = PaletteNumber;
}


void CreateNewPalette(void)
{
   int i;
   int Index = GetPlanesMinus1();
   struct PALETTE * ptmpPalette;
   Driver.NumPalettes[Index] ++;

   // create addition space for the new palette

   // cuz realloc does copy all structures into new address space I will do an
   // equivalent of realloc myself
   ptmpPalette = malloc(Driver.NumPalettes[Index] * sizeof(struct PALETTE));
   for (i = 0; i < Driver.NumPalettes[Index] -1; i ++)
   {
      ptmpPalette[i].NumColours = Driver.Palettes[Index][i].NumColours;
      ptmpPalette[i].Colours = Driver.Palettes[Index][i].Colours;
   }
   free(Driver.Palettes[Index]);
   Driver.Palettes[Index] = ptmpPalette;

   // this new palette has the same amount of colours as the previous palette
   Driver.Palettes[Index][Driver.NumPalettes[Index] - 1].NumColours =
      Driver.Palettes[Index][PaletteNumber].NumColours;
   
   // create space for the colours contained within the new palette
   Driver.Palettes[Index][Driver.NumPalettes[Index] - 1].Colours =
      malloc(Driver.Palettes[Index][Driver.NumPalettes[Index] - 1].NumColours * sizeof(RGB));

   // copy all of the previous colour into the new palette
   memcpy(Driver.Palettes[Index][Driver.NumPalettes[Index] - 1].Colours,
          Driver.Palettes[Index][PaletteNumber].Colours,
          Driver.Palettes[Index][PaletteNumber].NumColours * sizeof(RGB));

   // now select the new palette
   PaletteNumber = Driver.NumPalettes[Index] - 1;
   SetColours(Driver.GfxInfo[CurrentBank].GfxLayout,1);

   // update the display
   DisplayPaletteInfo();
}


void SelectColour(void)
{
	int exit = 0;
	static int y = 0;
	static int x = 0;
	int key;
	int MouseDir = 0, MouseButt = 0;
	char str_cnum[40];
   int Index = GetPlanesMinus1();
   
	// is x position invalid for this palette
	if (x > ColourColumns  -1)
		x = 0;

	// box the first entry for selection
	DrawCursor(x, y, 2);

	// allow selection
	do
	{      
		// JERRY added 11/18/97
      sprintf(str_cnum, "Col %02d/%02d", y + (x * MAX_COLOURS_Y)+1,
              Driver.Palettes[Index][PaletteNumber].NumColours);
      textout(screen, font, str_cnum, 116, 112, 2);

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
               DisplayHelp(HELP_SELECT_PALETTE);
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
				SetColours(Driver.GfxInfo[CurrentBank].GfxLayout,1);
            DisplayPaletteInfo();
            DrawCursor(x, y, 2);					
			}
			break;

		case '.': // increase palette number
			if (ModifyPaletteNumber(1))
			{
				SetColours(Driver.GfxInfo[CurrentBank].GfxLayout,1);
            DisplayPaletteInfo();
            DrawCursor(x, y, 2);					
			}
			break;
			
		case 32: // left mouse/space bar colour selected
			LeftMouseColour = y + (x * MAX_COLOURS_Y);
			DrawMouseColours();
			exit = 1;
			break;

      case 'p': // display palette dialog
      case 'P':
         PaletteDialog(y + (x * MAX_COLOURS_Y));
         break;

      case 'n': // create new palette
      case 'N':
         CreateNewPalette();
			DrawCursor(x, y, 2);					
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

   // clear colour number display
   textout(screen, font, "         ", 116, 112, 2);

	// remove cursor
	DrawCursor(x, y, 0);
}
	


static long PAL[] = 
{
   
   0x000000, 0x090909, 0x121212, 0x1b1b1b, 0x242424, 0x2d2d2d, 0x363636, 0x3f3f3f,
   0x000008, 0x000010, 0x000018, 0x000020, 0x000028, 0x000030, 0x000038, 0x00003f,
   0x000408, 0x000810, 0x000c18, 0x001020, 0x001428, 0x001830, 0x001c38, 0x00203f,
   0x000808, 0x001010, 0x001818, 0x002020, 0x002828, 0x003030, 0x003838, 0x003f3f,
   0x000800, 0x001000, 0x001800, 0x002000, 0x002800, 0x003000, 0x003800, 0x003f00,
   0x080800, 0x101000, 0x181800, 0x202000, 0x282800, 0x303000, 0x383800, 0x3f3f00,
   0x080000, 0x100000, 0x180000, 0x200000, 0x280000, 0x300000, 0x380000, 0x3f0000,
   0x080008, 0x100010, 0x180018, 0x200020, 0x280028, 0x300030, 0x380038, 0x3f003f
};


static void DrawPaletteCursor(int x, int y, int Colour)
{
   rect(screen, DIALOG_X + 1 + (x*9), DIALOG_Y + 1 + (y*9),
		  DIALOG_X + (x*9) +10, DIALOG_Y + (y*9) +10, Colour);
}

static void UpdateColour(int ColIndex, int NewColIndex)
{
   RGB NewCol;

   get_color(NewColIndex, &NewCol);
   set_color(ColIndex + PALETTEBASE, &NewCol);
}

static void StorePaletteChange(int ColIndex)
{
   RGB NewCol;
   int Index = GetPlanesMinus1();
   get_color(ColIndex + PALETTEBASE, &NewCol);

   Driver.Palettes[Index][PaletteNumber].Colours[ColIndex].r = NewCol.r;
   Driver.Palettes[Index][PaletteNumber].Colours[ColIndex].g = NewCol.g;
   Driver.Palettes[Index][PaletteNumber].Colours[ColIndex].b = NewCol.b;
}


// display palette modifier dialog
static void PaletteDialog(int ColIndex)
{
	int exit = 0;
	static int y = 0;
	static int x = 0;
	int key;
	int MouseDir = 0, MouseButt = 0;
   int i, j;
   RGB OldColour;
      
   // this should probably be more dynamic.. oh well..
   BITMAP * Background = create_bitmap(DIALOG_CX,DIALOG_CY);

   // store old colour in case of cancel
   get_color(ColIndex + PALETTEBASE, &OldColour);

   // copy screen image
   blit(screen, Background, DIALOG_X, DIALOG_Y, 0,0,
        DIALOG_X+DIALOG_CX -1, DIALOG_Y+DIALOG_CY-1);
   
   // draw the box
   rectfill(screen, DIALOG_X, DIALOG_Y, DIALOG_X+DIALOG_CX -1,
            DIALOG_Y+DIALOG_CY-1, 0); 
   rect(screen, DIALOG_X, DIALOG_Y, DIALOG_X+DIALOG_CX -1,
        DIALOG_Y+DIALOG_CY-1, PAL_LTGREY);

   for (i = 0; i < 64; i ++)
   {      
      RGB temp;
      temp.r = (char)(PAL[i]);
      temp.g = (char)(PAL[i] >> 8);
      temp.b = (char)(PAL[i] >> 16);

      set_color(192 + i, &temp);
   }
   
   // draw colours
   for (i = 0; i < 8; i ++)
   {
      for (j = 0; j < 8; j ++)
      {
         rectfill(screen, DIALOG_X + 2 + (9 * j), DIALOG_Y + 2 + (9 * i),
                  DIALOG_X + 9 + (9 * j), DIALOG_Y + 9 + (9 * i), 192 + (i * 8) + j);
      }
   }


   DrawPaletteCursor(x, y, 2);
   UpdateColour(ColIndex, 192+x+(y*8));

   do
	{      

		while (!kbhit() && MouseDir == 0 && (MouseButt & MOUSE_LMB) == 0)
		{
			MouseDir = mouse_move();
			MouseButt = mouse_butt();
		}

      key = 0;
      wait_for_button_release();
      
      // wait for a key unless the mouse has moved or the LMB has been pressed
		if (MouseDir == 0 && MouseButt == 0)
			key = getch(); 

		if (MouseButt != 0) key = 32;
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
               DisplayHelp(HELP_PALETTE_COLOUR);
               break;
               
			case CURSOR_UP:
				if (y > 0)
				{
					// remove old selection
					DrawPaletteCursor(x, y, 0);
					y --;
					//draw new
					DrawPaletteCursor(x, y, 2);
               UpdateColour(ColIndex, 192+x+(y*8));
				}
				break;  

			case CURSOR_DOWN:
				if (y < 7)
				{
               // remove old selection
					DrawPaletteCursor(x, y, 0);
					y ++;
					//draw new
					DrawPaletteCursor(x, y, 2);
               UpdateColour(ColIndex, 192+x+(y*8));
				}
				break;  

			case CURSOR_LEFT:
				if (x > 0)
				{
					// remove old selection
					DrawPaletteCursor(x, y, 0);
					x --;
					//draw new
					DrawPaletteCursor(x, y, 2);
               UpdateColour(ColIndex, 192+x+(y*8));
            }
				break;  

			case CURSOR_RIGHT:
				if (x < 7)
				{
					// remove old selection
					DrawPaletteCursor(x, y, 0);
					x ++;
					//draw new
					DrawPaletteCursor(x, y, 2);					
               UpdateColour(ColIndex, 192+x+(y*8));
            }
				break;  
			}
		}
		break;

      case 27: // escape or space to select
         set_color(ColIndex + PALETTEBASE, &OldColour);
         exit = 1;
			break;

      case 32: // space to accept new colour
         StorePaletteChange(ColIndex);
         exit = 1;
         break;
		}
   
   } while(!exit);
   
   // restore screen image
   blit(Background, screen, 0,0, DIALOG_X, DIALOG_Y, DIALOG_X+DIALOG_CX -1,
        DIALOG_Y+DIALOG_CY-1);

   destroy_bitmap(Background);
   return;
}

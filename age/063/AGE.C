// A.G.E. (Arcade Games editor) v0.6
// ================================= 
//
//
#include <stdio.h>
#include <conio.h>
#include <allegro.h>  
#include "ini.h"
#include "drivers.h"
#include "age.h"
#include "coding.h"
#include "fileio.h"
#include "scratch.h"
#include "mouse.h"
#include "tools.h"
#include "palette.h"
#include "maped.h"
#include "texted.h"
#include "snap.h"
#include "help.h"

char title_text[] = "Arcade Games Editor v0.6.3";

// 
// Globals 
//
int DriverNumber = -1;
BYTE * ROMSet = NULL;
BYTE * DecodedSet = NULL;
int CurrentBank = 0;
int TopRow = 0;
unsigned long timer_counter = 0xffff;
// formerly from Process Commands...
int x = 0;
int y = 0;
int CursorSize = 8;
int SizeIndex = 0;
int CurrentBankModified = 0;
int ZoomSize = 0;
int ZoomX = 0;
int ZoomY = 0;

static int EditRectX[] = { 82, 83, 99 };
static int EditRectY[] = { 97, 98, 114 };
static int ZoomArray[] = {8, 16, 32};

// holds information about currently being edited character
BITMAP * CurChar;


// list of all of the drivers
////// MOVED TO DRIVERS.C ///////


void InitialiseDisplay(void)
{
   set_gfx_mode(GFX_VGA,320,200,0,0);

   SetReservedColours();

   textout_centre(screen, font, title_text, 160, 1, PAL_LTGREY);

	rect (screen, 1, 16, EditRectX[0], EditRectY[0], PAL_LTGREY); // char editing space

	textout(screen, font, "No font loaded", 1, FONT_TITLE_POS, PAL_YELLOW);

   InitialisePalette();
   InitialiseScratch();
   mouse_init();
}


void ClearZoomWindow(void)
{
	// blank out zoom window and redraw border
	rectfill (screen, 1, 16, EditRectX[SizeIndex], EditRectY[SizeIndex], PAL_BLACK);
	rect (screen, 1, 16, EditRectX[SizeIndex], EditRectY[SizeIndex], PAL_LTGREY);
}


int NEW_Dialog_Box(char * topline, char * bottomline)
{
    if (alert(topline, bottomline, NULL, "Continue", NULL, ESCAPE, 0) == 1)
	return ESCAPE;
    else
    	return ' ';
}

int Dialog_Box(char * topline, char * bottomline)
{
   int value;
   // this should probably be more dynamic.. oh well..
   BITMAP * Background = create_bitmap(260,60);
   
   // copy screen image
   blit(screen, Background, 30,70, 0,0, 260,60);
   
   // draw the box
   rectfill(screen, 30,70,289,129, 0); // oops
   rect(screen, 30,70, 289,129, 1);
   
   // draw the text
   textout_centre(screen, font, topline,    160,  85, PAL_YELLOW);
   textout_centre(screen, font, bottomline, 160, 110, PAL_LTGREY);
   
   // wait for key press
   value = getch();
   
   // restore screen image
   blit(Background, screen, 0,0, 30,70, 260,60);
   destroy_bitmap(Background);
   return value;
}



void BlitChar(BITMAP * bmp, int x, int y, int width, int height, int No)
{
   int cx, cy;
   int pos = (No * (width * height));

   for (cy = 0; cy < height; cy ++)
   {
      for (cx = 0; cx < width; cx ++)
      {
	 putpixel(bmp, x + cx, y + cy, DecodedSet[pos] + PALETTEBASE);
	 pos++;
      }
   }
}

void DrawChar(int x, int y, int width, int height, int No)
{
    BlitChar(screen, x, y, width, height, No);
}


void DisplayFont(struct GFXLAYOUT *gl, int ColourChange)
{
    int row,column;
    int TotCols = (256/gl->width);
    int CharNo = TopRow * TotCols;
    
    if (ColourChange)
	SetColours(gl, 1);

    for (row = 0; row < (64/gl->height); row ++)
    {
	for (column = 0; column < TotCols; column++)
	{
	    if (CharNo < gl->total)
		DrawChar(32+(column * gl->width), FONT_TOP+(row * gl->height), 
		     gl->width, gl->height, CharNo);
	    else // hey Ivan... we need this!  ;)   -Jerry
		rectfill(screen,
		    32+(column * gl->width),
		    FONT_TOP+(row * gl->height),
		    32+(column * gl->width) + gl->width - 1,
		    FONT_TOP+(row * gl->height) + gl->height - 1,
		    0);
	    CharNo++;
	}
    }
}

void DisplayStatusLine(void)
{
   char banktxt[80];
   // remove any old name
   textout(screen, font, "                        ", 1, FONT_TITLE_POS, PAL_GREY);

   // add new one
   textout(screen, font, Drivers[DriverNumber]->GameName, 1, FONT_TITLE_POS, PAL_YELLOW);

   // now the bank number information..
   sprintf(banktxt, "Bank %02d/%02d", GetGfxBank()+1,
	   Drivers[DriverNumber]->NumGfxBanks);
   textout(screen, font, banktxt,
           319-text_length(font, banktxt), FONT_TITLE_POS,
	   PAL_GREEN);
}




// converts the format of the mouse direction to
// that of the getch for cursor keys
int ConvertMouseDirection(int MouseDir)
{
   if ((MouseDir & 1) == 1)
      return CURSOR_RIGHT;

   if ((MouseDir & 2) == 2)
      return CURSOR_DOWN;

   if ((MouseDir & 4) == 4)
      return CURSOR_LEFT;

   if ((MouseDir & 8) == 8)
      return CURSOR_UP;

   // default - shouldn't happen
   return 0;
}



int SelectChar(struct GFXLAYOUT *gl)
{
   static int x = 0;
   static int y = 0;
   static int prevw = 0, prevh = 0, prevtot = 0;
   char str_cnum[40];

   int key;
   int exit = 0;
   int CharNumber = -1;
   int MouseDir = 0, MouseButt = 0;


   // reset cursor position to a safe one if we have since
   // changed graphics banks to a different size
   if (gl->width != prevw || gl->height != prevh || gl->total != prevtot)
   {
      x = 0;
      y = 0;
      TopRow = 0;
      prevw = gl->width;
      prevh = gl->height;
      prevtot = gl->total;
      DisplayFont(gl,0); //1); JERR
   }


    // draw cursor
    rect(screen, 32+(x*gl->width), FONT_TOP+(y*gl->height), 
	    31+gl->width+(x*gl->width), 
	    FONT_TOP-1+gl->height+(y*gl->height), PAL_LTGREY);

    do
    {
	   sprintf(str_cnum, " 0x%02X ", ((y + TopRow) * (256/gl->width)) + x);
	   textout_centre(screen, font, str_cnum, 160, FONT_TITLE_POS, PAL_LTGREY);
	
		while (!kbhit() && MouseDir == 0 && MouseButt == 0)
	   {
		  MouseDir = mouse_move();
		  MouseButt = mouse_butt();
	   }
	
	   key = 0;
	
	   if (MouseDir == 0 && MouseButt == 0)
		  key = getch(); 
	
	   /* if button pressed simulate space */
	   if (MouseButt > 0)
	   {
		  key = 32;
		  MouseButt = 0;
	   }
	
	   switch(key)
	   {
	   case 0: // cursor key
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
		  case CURSOR_DOWN:
		  {
		 int Rows = (64/gl->height);
		 int Cols = (256/gl->width);
	
		 // scroll
		 if ((y == Rows -1) && ((Rows + TopRow) * Cols < gl->total))
		 {
			TopRow ++;
			DisplayFont(gl,0);
	
			// draw cursor over new position
			rect(screen, 32+(x*gl->width), FONT_TOP+(y*gl->height), 
			31+gl->width+(x*gl->width), 
			FONT_TOP-1+gl->height+(y*gl->height), PAL_LTGREY);
		  }
	
		  else if (y < Rows -1)
		  {
		 // replace char over cursor
		 DrawChar(32+(x*gl->width),FONT_TOP+(y*gl->height),
		 gl->width, gl->height, 
		 x+((y + TopRow) * Cols));
		  y++;
		  // draw cursor over new position
		  rect(screen, 32+(x*gl->width), FONT_TOP+(y*gl->height), 
		  31+gl->width+(x*gl->width), 
		  FONT_TOP-1+gl->height+(y*gl->height), PAL_LTGREY);
	}
	}
	break;
	
		  case CURSOR_UP:
		 if (y == 0 && TopRow > 0)
		 {
			// scroll
			TopRow --;
			DisplayFont(gl,0);
	
			// draw cursor over new position
			rect(screen, 32+(x*gl->width), FONT_TOP+(y*gl->height), 
			31+gl->width+(x*gl->width), 
			FONT_TOP-1+gl->height+(y*gl->height), PAL_LTGREY);
		  }
	
		  else if (y > 0)
		  {
		 // replace char over cursor
		 DrawChar(32+(x*gl->width),FONT_TOP+(y*gl->height),
		 gl->width, gl->height, 
		 x+((y + TopRow) * (256/gl->width)));
		  y--;
		  // draw cursor over new position
		  rect(screen, 32+(x*gl->width), FONT_TOP+(y*gl->height), 
		  31+gl->width+(x*gl->width), 
		  FONT_TOP-1+gl->height+(y*gl->height), PAL_LTGREY);
	}
	break;
	
		  case CURSOR_LEFT:
		 if (x > 0)
		 {
			// replace char over cursor
			DrawChar(32+(x*gl->width),FONT_TOP+(y*gl->height),
			gl->width, gl->height, 
			x+((y + TopRow) * (256/gl->width)));
		 x--;
		 // draw cursor over new position
		 rect(screen, 32+(x*gl->width), FONT_TOP+(y*gl->height), 
		 31+gl->width+(x*gl->width), 
		 FONT_TOP-1+gl->height+(y*gl->height), PAL_LTGREY);
	   }
	   break;
	
		  case CURSOR_RIGHT:
		 if (x < (256/gl->width) -1)
		 {
			// replace char over cursor
			DrawChar(32+(x*gl->width),FONT_TOP+(y*gl->height),
			gl->width, gl->height, 
			x+((y + TopRow) * (256/gl->width)));
		 x++;
		 // draw cursor over new position
		 rect(screen, 32+(x*gl->width), FONT_TOP+(y*gl->height), 
		 31+gl->width+(x*gl->width), 
		 FONT_TOP-1+gl->height+(y*gl->height), PAL_LTGREY);
	   }
	   break;

        case KEY_F1:
           DisplayHelp(HELP_SELECT_CHAR);
           break;
        }
	
	}
	break;
	
	   case 32: // select
		  CharNumber = ((y + TopRow) * (256/gl->width)) + x;
		  exit = 1;
		  break;
	
	   case ESCAPE: // escape
		  CharNumber = -1;
		  exit = 1;
		  break;
	   }
	
    } while (!exit);

textout_centre(screen, font, "      ", 160, FONT_TITLE_POS, 1);

// replace char over cursor
DrawChar(32+(x*gl->width),FONT_TOP+(y*gl->height),
gl->width, gl->height, x+((y + TopRow) * (256/gl->width)));

return CharNumber;
}


int EnlargeChar(int No, struct GFXLAYOUT * gl)
{
	int cx, cy;
	int NewCursorSize;

	int pos = (No * (gl->width * gl->height));
	int blocksize;
	int blockheight;

	switch(ZoomSize)
	{
		case 1:
			NewCursorSize = 4;
			blocksize = 5;
			blockheight = 3;
			break;

		case 2:
			NewCursorSize = 2;
			blocksize = 3;
			blockheight = 1;
			ZoomSize = 2;
			break;

		case 0:
		default:
			NewCursorSize = 8;
			blocksize = 10;
			blockheight = 7;
			break;
	}

	for (cy = 0; cy < gl->height; cy ++)
	{
		for (cx = 0; cx < gl->width; cx ++)
		{
			// only display enlarged if it will fit in window
			if (cy < ZoomArray[ZoomSize] && cx < ZoomArray[ZoomSize])
			{
				rectfill (screen, 3 + (cx*blocksize), 18 + (cy*blocksize), 
							 3 + blockheight + (cx*blocksize), 18 + blockheight + 
							 (cy*blocksize), DecodedSet[pos] + PALETTEBASE);
			}

			// store in array
			putpixel(CurChar, cx, cy, DecodedSet[pos] + PALETTEBASE);

			pos++;
		}
	}

	// display char
	blit(CurChar, screen, 0,0, 109,18, gl->width, gl->height);

	// have we enlarged it to be 8x8 per pixel or 4x4?
	return NewCursorSize;
}



void ZoomChar(void)
{
	int cx, cy;
	int blocksize;
	int blockheight;

	switch(ZoomSize)
	{
		case 1:
			blocksize = 5;
			blockheight = 3;
			break;

		case 2:
			blocksize = 3;
			blockheight = 1;
			break;

		case 0:
		default:
			blocksize = 10;
			blockheight = 7;
			break;
	}

	for (cy = ZoomY; cy < ZoomY + ZoomArray[ZoomSize]; cy ++)
	{
		for (cx = ZoomX; cx < ZoomX + ZoomArray[ZoomSize]; cx ++)
		{
			int pixel = getpixel(CurChar, cx, cy);
			rectfill (screen, 3 + ((cx-ZoomX)*blocksize),
						 18 + ((cy-ZoomY)*blocksize), 
						 3 + blockheight + ((cx-ZoomX)*blocksize),
						 18 + blockheight + ((cy-ZoomY)*blocksize), pixel);
		}
	}
}



int GetCharFromSet(struct GFXLAYOUT * gl)
{
   int retval = -1;

   // allow user to select char from set
   int SelectedChar = SelectChar(gl);

   // display char for editting
   if (SelectedChar != -1)
   {
	   SetRightMouseColour(0); // JERRY
      do_tool(TOOLS_CLEAR_BITMAP, CurChar, 0,0);
      retval = EnlargeChar(SelectedChar, gl);
      SelectNewCurrenCharacter(SelectedChar);
   }

   // return bits per pixel size
   return retval;
}



void PutImage(struct GFXLAYOUT * gl)
{
   int pos, x, y;

   // allow user to select char from set
   int SelectedChar = SelectChar(gl);
   if (SelectedChar == -1)
      return;

   pos = SelectedChar * (gl->width * gl->height);
   for (y = 0; y < gl->height; y ++)
   {
      for (x = 0; x < gl->width; x ++)
      {
	 DecodedSet[pos] = getpixel(CurChar, x, y) - PALETTEBASE;
	 pos++;
      }
   }

   DisplayFont(gl,0);
}


void DrawPixel(struct GFXLAYOUT *gl, int x, int y, int colour)
{
   int blocksize;
   int blockheight;

	switch(ZoomSize)
	{
		case 1:
			blocksize = 5;
			blockheight = 3;
			break;

		case 2:
			blocksize = 3;
			blockheight = 1;
			break;
			
		case 0:
		default:
			blocksize = 10;
			blockheight = 7;
			break;
	}
	
   rectfill (screen, 3 + ((x-ZoomX)*blocksize),
				 18 + ((y-ZoomY)*blocksize),
				 3 + blockheight + ((x-ZoomX)*blocksize),
				 18 + blockheight + ((y-ZoomY)*blocksize),
				 colour + PALETTEBASE);

	putpixel(CurChar, x, y, colour + PALETTEBASE);

	// display char
	blit(CurChar, screen, 0,0, 109,18, gl->width, gl->height);

	UpdateScratch(CurChar);
}


static void DrawCursor(int x, int y, int CursorSize, int Colour)
{
	int BlockSize;
	int BlockHeight;

	switch (CursorSize)
	{
		case 4:
			BlockSize = 5;
			BlockHeight = 5;
			break;

		case 2:
			BlockSize = 3;
			BlockHeight = 3;
			break;

		case 8:
		default:
			BlockSize = 10;
			BlockHeight = 9;
			break;
	}

   rect(screen, 2+(x*BlockSize), 17+(y*BlockSize),
   2+BlockHeight+(x*BlockSize), 17+BlockHeight+(y*BlockSize), Colour);
}


int GetGfxBank(void)
{
    return CurrentBank;
}


void ResizeCursors(int ZoomRatio)
{
    struct GFXLAYOUT * gl;
    int NewCursorSize;
	 int NewSizeIndex;
	 int Size;
	 
	 // has the cursor size changed?
    gl = Drivers[DriverNumber]->GfxInfo[CurrentBank].GfxLayout;
	 Size = (ZoomRatio == 0) ? gl->height : ZoomRatio;
	 
	 switch(Size)
	 {
		 case 16:
			 NewSizeIndex = 1;
			 NewCursorSize = 4;
			 break;

		 case 32:
			 NewSizeIndex = 2;
			 NewCursorSize = 2;
			 break;
			 
		 case 8:
		 default:
			 NewSizeIndex = 0;
			 NewCursorSize = 8;
			 break;
	 }
	 
	 if (CursorSize != NewCursorSize)
	 {
		 DrawCursor(x, y, CursorSize, 0);
		 rectfill (screen, 1, 16, EditRectX[SizeIndex], EditRectY[SizeIndex], PAL_BLACK); 

		 CursorSize = NewCursorSize;
		 SizeIndex = NewSizeIndex;
		 x = 0;
		 y = 0;
		 SetRightMouseColour(0); 

		 // if we are just zooming in then don't wipe out bitmap
		 if (ZoomRatio == 0)
		 {
			 rectfill (screen, 109, 18,  109+32, 18+32, 0); // wipe out old bitmap
			 do_tool(TOOLS_CLEAR_BITMAP, CurChar,0,0);
			 TopRow = 0;
		 }
		 
		 rect (screen, 1, 16, EditRectX[SizeIndex], EditRectY[SizeIndex], PAL_LTGREY); 
		 DrawCursor(x,y, CursorSize, 2);
	 }
}


void ChangeGfxBank(int bank)
{
    if (DriverNumber == -1)
    	return;

    // do we have to save changes before changing to a 
    // different graphics bank?
    if (CurrentBankModified)
    {
	Encode(&Drivers[DriverNumber]->GfxInfo[CurrentBank]);
	CurrentBankModified = 0;
    }

    // check for bounds conditions
    if (bank < 0)
	bank=0;
    if (bank >= Drivers[DriverNumber]->NumGfxBanks)
	bank = Drivers[DriverNumber]->NumGfxBanks-1;

    CurrentBank = bank;

    Decode(&Drivers[DriverNumber]->GfxInfo[CurrentBank]);

	 switch(Drivers[DriverNumber]->GfxInfo[CurrentBank].GfxLayout->height)
	 {
		 case 16:
			 ZoomSize = 1;
			 break;

		 case 32:
			 ZoomSize = 2;
			 break;

		 case 8:
		 default:
			 ZoomSize = 0;
			 break;
	 }
	 
	 ResizeCursors(0);
	 DisplayFont(Drivers[DriverNumber]->GfxInfo[CurrentBank].GfxLayout,1);
	 UpdateScratch(CurChar);
}


// returns number of blocks to make a character -1
// for the current cursor size
int GetMaxXY(void)
{
	switch(CursorSize)
	{
		case 2:
			return 31;

		case 4:
			return 15;

		case 8:
		default:
			return 7;
	}				

	return 7;
}


int CanScroll(int Val)
{
	int BitmapSize = Drivers[DriverNumber]->GfxInfo[CurrentBank].GfxLayout->height;

	return (Val < BitmapSize -1);
}



void ProcessCommands(int InitialDriverIndex)
{
   int Exit = 0;
   int MouseDir = 0, MouseButt = 0;
   int key;
   int tempval;
   int NewBank;
   int InitialLoad = 1;
   
   // draw initial cursor & choose initial graphics bank
   DrawCursor(x, y, CursorSize, 2);


   do
   {
      while (!kbhit() && MouseDir == 0 && MouseButt == 0 && !InitialLoad)
      {
         MouseDir = mouse_move();
         MouseButt = mouse_butt();
      }

      key = 0;

      if (MouseDir == 0 && MouseButt == 0 && !InitialLoad)
         key = getch(); 

      // if the load dialog is to be displayed - simulate key
      if (InitialLoad)
      {
         key = 'l';
         InitialLoad = 0;
      }

      /* if button pressed simulate space */
      // JERRY
      if (MouseButt & MOUSE_LMB) key = 32;
      if (MouseButt & MOUSE_RMB) key = 'z';
      MouseButt = 0;

      switch (key)
      {
      case ESCAPE:     // Escape to exit
         // put the "are you sure" dialog here
         // change this text into more appropriate text?
         if (Dialog_Box("You are about to quit!",
                        "Strike [esc] again to quit.") == ESCAPE)
            Exit = 1;
         break;

      case 'l':    // Load
      case 'L':
      {
         int Sel = InitialDriverIndex;
         InitialDriverIndex = -1;
         
         // no roms specified from command line
         if (Sel < 0)
         {
            Sel = LoadSelector();
            wait_for_button_release();
         }

         // did the user select a driver
         if (Sel >= 0)
         {
            DriverNumber = Sel;
            if (!LoadRoms())
            {
               // successfully loaded roms
               CurrentBank = 0;
               Decode(&Drivers[DriverNumber]->GfxInfo[CurrentBank]);

               ResizeCursors(0);
               ModifyPaletteNumber(0);
               DisplayFont(Drivers[DriverNumber]->GfxInfo[CurrentBank].GfxLayout,1);
               DisplayStatusLine();
               ClearScratch();
            }
            else
            {
               // failed to load
               DriverNumber = -1;
               // remove font name and graphics from screen
               textout(screen, font, "No font loaded          ", 1, FONT_TITLE_POS, PAL_YELLOW);
               clear_to_color(CurChar, PALETTEBASE);
               blit(CurChar, screen, 0,0, 109,18, 109+CurChar->w, 18+CurChar->h);
               ClearZoomWindow();
               DrawCursor(x, y, CursorSize, 2);
               rectfill(screen, 32, FONT_TOP, 32+(32*8), FONT_TOP+(8*8), PAL_BLACK);
            }
            ZoomX = 0;
            ZoomY = 0;
         }

         if (DriverNumber != -1)
         {
            NewBank = GetGfxBank();
            ChangeGfxBank(NewBank); // reset the graphics bank
         }


         break;
      }

      case 'g':	  // get char from charset
      case 'G':
         if (DriverNumber != -1)
         {
            int bpp;

            DrawCursor(x, y, CursorSize, 0);
            bpp = GetCharFromSet(Drivers[DriverNumber]->GfxInfo[CurrentBank].GfxLayout);

            wait_for_button_release();

            if (bpp != -1)
            {
               // if the graphics have changed size reset the cursor position
               if (bpp != CursorSize)
               {
                  x = 0;
                  y = 0;
               }
               CursorSize = bpp;
            }
            DrawCursor(x, y, CursorSize, 2);
         }
         break;

      case 'm':	  // map editor
      case 'M':
         EditMap();

         break;

      case 't':	  // bitmap tools
      case 'T':
         if (DriverNumber != -1)
         {
            int selected = ToolSelector();

            wait_for_button_release();

            if (selected != -1)
            {
               do_tool(selected, CurChar, x, y);

               // redraw cursor
               DrawCursor(x, y, CursorSize, 2);
            }
         }
         break;

      case 0: // cursor key
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
         case CURSOR_DOWN:
         {
            if (y < GetMaxXY())
            {
               //remove old cursor
               DrawCursor(x, y, CursorSize, 0);
               y++;
               // draw new cursor
               DrawCursor(x, y, CursorSize, 2);
            }
            else if (y == GetMaxXY())
            {
               // scroll?
               if (CanScroll(y + ZoomY))
               {
                  //remove old cursor
                  DrawCursor(x, y, CursorSize, 0);
                  ZoomY++;
                  ZoomChar();
                  // draw new cursor
                  DrawCursor(x, y, CursorSize, 2);
               }
            }

         }
         break;

         case CURSOR_UP:
            if (y > 0)
            {
               //remove old cursor
               DrawCursor(x, y, CursorSize, 0);
               y--;
               // draw new cursor
               DrawCursor(x, y, CursorSize, 2);
            }
            else if (y == 0 && ZoomY > 0)
            {
               //remove old cursor
               DrawCursor(x, y, CursorSize, 0);
               ZoomY--;
               ZoomChar();
               // draw new cursor
               DrawCursor(x, y, CursorSize, 2);
            }

            break;

         case CURSOR_LEFT:
            if (x > 0)
            {
               //remove old cursor
               DrawCursor(x, y, CursorSize, 0);
               x--;
               // draw new cursor
               DrawCursor(x, y, CursorSize, 2);
            }
            else if (x == 0 && ZoomX > 0)
            {
               //remove old cursor
               DrawCursor(x, y, CursorSize, 0);
               ZoomX--;
               ZoomChar();
               // draw new cursor
               DrawCursor(x, y, CursorSize, 2);
            }
            break;

         case CURSOR_RIGHT:
         {
            if (x < GetMaxXY())
            {
               //remove old cursor
               DrawCursor(x, y, CursorSize, 0);
               x++;
               // draw new cursor
               DrawCursor(x, y, CursorSize, 2);
            }
            else if (x == GetMaxXY())
            {
               // scroll?
               if (CanScroll(x + ZoomX))
               {
                  //remove old cursor
                  DrawCursor(x, y, CursorSize, 0);
                  ZoomX++;
                  ZoomChar();
                  // draw new cursor
                  DrawCursor(x, y, CursorSize, 2);
               }
            }
         }
         break;

         case PAGE_UP:
            NewBank = GetGfxBank();
            NewBank++;
            ZoomX = 0;
            ZoomY = 0;
            ChangeGfxBank(NewBank);
            DisplayStatusLine();
            break;   

         case PAGE_DOWN:
            NewBank = GetGfxBank();
            NewBank--;
            ZoomX = 0;
            ZoomY = 0;
            ChangeGfxBank(NewBank);
            DisplayStatusLine();
            break;   

         case ALT_S: // alt-s for save
            if (DriverNumber != -1)
            {
               // do we have to save changes before saving the new ROM set?
               if (CurrentBankModified)
               {
                  Encode(&Drivers[DriverNumber]->GfxInfo[CurrentBank]);
                  CurrentBankModified = 0;
               }

               SaveRoms();
               // JERRY added - 11/14/97
               (void)Dialog_Box(
                                "Graphics rom images saved.",
                                "Hit any key to continue.");
            }
            break;

         case (ALT_CURSOR_UP):
            do_tool(TOOLS_SLIDE_UP, CurChar,0,0);
            break;
         case (ALT_CURSOR_DOWN):
            do_tool(TOOLS_SLIDE_DOWN, CurChar,0,0);
            break;
         case (ALT_CURSOR_LEFT):
            do_tool(TOOLS_SLIDE_LEFT, CurChar,0,0);
            break;
         case (ALT_CURSOR_RIGHT):
            do_tool(TOOLS_SLIDE_RIGHT, CurChar,0,0);
            break;

         case (CTRL_CURSOR_UP):
            do_tool(TOOLS_WRAP_UP, CurChar,0,0);
            break;
         case (CTRL_CURSOR_DOWN):
            do_tool(TOOLS_WRAP_DOWN, CurChar,0,0);
            break;
         case (CTRL_CURSOR_LEFT):
            do_tool(TOOLS_WRAP_LEFT, CurChar,0,0);
            break;
         case (CTRL_CURSOR_RIGHT):
            do_tool(TOOLS_WRAP_RIGHT, CurChar,0,0);
            break;

            // sub editors... move them all to f-keys
         case (KEY_F1):
            DisplayHelp(HELP_CHAR_EDIT);
            break;
            
         case (KEY_F5):
            EditMap();
            break;

         case (KEY_F6):
            Edit_Text();
            break;

            // bitmaps manipulations - F9 thru F12
         case (KEY_F9):
            SnapRomsetBitmap();
            break;

         case (KEY_F10):
            tempval = GetSpriteFromPCX();
            if (tempval != -1)
            { // now select the char
               struct GFXLAYOUT * gl = Drivers[DriverNumber]->GfxInfo[CurrentBank].GfxLayout;
               (void)EnlargeChar(tempval, gl);
               SelectNewCurrenCharacter(tempval);
               DisplayFont(gl, 0);
               CurrentBankModified = 1;
               wait_for_button_release(); // not necessary
            }
            break;

         case (F12):
            screen_snap();
            break;

         }

      }
      break;

      case 32: // draw with selected colour
         if (DriverNumber != -1)
         {
            DrawPixel(Drivers[DriverNumber]->GfxInfo[CurrentBank].GfxLayout,
                      x + ZoomX, y + ZoomY, GetLeftMouseColour());
         }
         break;

      case 'z': // draw with colour 0 (black to erase)
      case 'Z':
         if (DriverNumber != -1)
         {
            DrawPixel(Drivers[DriverNumber]->GfxInfo[CurrentBank].GfxLayout,
                      x + ZoomX, y + ZoomY, GetRightMouseColour());
         }
         break;

      case 'c': // Select colour
      case 'C':
         //remove cursor
         DrawCursor(x, y, CursorSize, 0);
         SelectColour();	      
         // draw new cursor
         DrawCursor(x, y, CursorSize, 2);

         wait_for_button_release();
         break;

      case 'p': // put image in charset
      case 'P':
         if (DriverNumber != -1)
         {
            PutImage(Drivers[DriverNumber]->GfxInfo[CurrentBank].GfxLayout);
            CurrentBankModified = 1;

            wait_for_button_release();
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

      case 9: // tab - switch to scratch mode
         if (DriverNumber != -1)
         {
            DrawCursor(x, y, CursorSize, 0);
            Scratch(CurChar);
            DrawCursor(x, y, CursorSize, 2);
         }
         break;


      case '=': // zoom out
      case '+':
         if (DriverNumber != -1)
         {
            if (ZoomSize > 0)
            {
               ZoomSize --;
               ZoomX = 0;
               ZoomY = 0;
               ResizeCursors(ZoomArray[ZoomSize]);
               ZoomChar();
            }
         }
         break;

      case '-': // zoom in
         if (DriverNumber != -1)
         {

            if (ZoomSize < 2 && ZoomArray[ZoomSize] <
                Drivers[DriverNumber]->GfxInfo[CurrentBank].GfxLayout->height)
            {
               ZoomSize ++;
               ZoomX = 0;
               ZoomY = 0;
               ResizeCursors(ZoomArray[ZoomSize]);
               ZoomChar();
            }
         }
         break;

      default:
         break;
      }
   } while(!Exit);
}



void timer_handler(void)
{
    timer_counter++;
}

void setup_timers(void)
{
    if (windows_version != 3 && windows_version != 4)
    {
	i_love_bill = TRUE; // make the timer windoze-friendly
	install_timer();
	install_int(&timer_handler, 100);
    } 
}

void end_credits(void)
{
    printf("%s\n\n", title_text);
    printf("Written by:\n");
    printf("\tIvan Mackintosh         ivan@rcp.co.uk\n");
    printf("\tScott \"Jerry\" Lawrence  jerry@mail.csh.rit.edu\n");
    printf("\tChris \"Zwaxy\" Moore     chris.moore@writeme.com\n");

    printf("\nSpecial thanks to:\n");
    printf("\tDavid Caldwell, M.C. Silvius\n");
    // add more people's names in here!
    printf("\tand the people who worked on the original games & the emulators!\n");
    printf("\n");
}


// warn user about patch destroying any of there changes
// returns 0 for exit or 1 for continue
int PatchContinue(char * RomName)
{
   int key;
   int retval = 0;
   
   printf("\n! W A R N I N G !\n");
   printf("You have specified -patch. As part fo the patch process any changes\n");
   printf("that you have made to \"%s\" will be lost\n", RomName);
   printf("\nDo you wish to continue (Y/N)\n");

   for(;;)
   {
      key = getch();

      if ((key == 'n') || (key == 'N'))
      {
         retval = 0;
         break;
      }

      if ((key == 'y') || (key == 'Y'))
      {
         retval = 1;
         break;
      }
   }

   printf("\nFor future reference specifying -patchq instead of -patch will not\n");
   printf("display this warning.\n");

   return retval;
}

void main(int argc, char ** argv)
{
   int i;
   int InitialDriverIndex = -1;
   allegro_init();  // needs to be called before the ini functions
   Init_INI();

   // parse command line
   for (i = 1; i < argc; i ++)
   {
      if ((argv[i][0] == '-') || (argv[i][0] == '/'))
      {
         if (strcmp(&argv[i][1], "listhtml") == 0)
         {
            ListHtml();
            return;
         }

         if (strcmp(&argv[i][1], "listfull") == 0)
         {
            ListFull();
            return;
         }

         if (strcmp(&argv[i][1], "listroms") == 0)
         {
            ListRoms();
            return;
         }

         if (strcmp(&argv[i][1], "list") == 0)
         {
            ListSupported();
            return;
         }

         if (strcmp(&argv[i][1], "help") == 0) 
         {
            printf("\n");
            printf("%s\n", title_text);
            printf("\n");
            printf("Usage:\n");
            /* add more command line options in here */
            printf("\tage.exe [-help]    \tdisplays this info\n");
            printf("\tage.exe [-list]    \tlists all supported romsets\n");
            printf("\tage.exe [-listfull]\tlists all supported roms & names\n");
            printf("\tage.exe [-listroms]\tlists all required roms\n");
            printf("\tage.exe [-version] \tdisplay version information\n");
            printf("\tage.exe [game name]\tbypass the load selector\n");
            return;
         }

         if (strcmp(&argv[i][1], "version") == 0)
         {
            printf("\n");
            printf("%s\n", title_text);
            printf("\tAge.ini version %d.%d\n", ini_version_major, ini_version_minor);
            printf("\tBuilt: %s, %s using:\n", __DATE__, __TIME__);
            printf("\t       Allegro v%s, %s\n", ALLEGRO_VERSION_STR, ALLEGRO_DATE_STR);
            printf("\t       DJGPP v%s\n", __VERSION__);
            return;
         }

         // make IPS compatible patch file for the specified game
         if (strcmp(&argv[i][1], "makepatch") == 0)
         {
            if (InitialDriverIndex == -1)
            {
               printf("No game specified to make patch files for\n");
               return;
            }
            else
            {
               MakePatch(InitialDriverIndex);
               return;
            }
         }

         // Patch ROMs for specified game with IPS compatible patch file
         if (strcmp(&argv[i][1], "patch") == 0)
         {
            if (InitialDriverIndex == -1)
            {
               printf("No game specified to patch ROMs for\n");
               return;
            }
            else
            {
               if (PatchContinue(argv[1]))
                  ApplyPatch(InitialDriverIndex);
               return;
            }
         }

         // Patch ROMs for specified game with IPS compatible patch file
         if (strcmp(&argv[i][1], "patchq") == 0)
         {
            if (InitialDriverIndex == -1)
            {
               printf("No game specified to patch ROMs for\n");
               return;
            }
            else
            {
               ApplyPatch(InitialDriverIndex);
               return;
            }
         }
      }
      else
      {
         // command line option without - or /
         // it could be a rom name
         if (i == 1)
         {
            // only accept romnames as first paramter
            InitialDriverIndex = IndexFromShortName(argv[1]);
            if (InitialDriverIndex == -1)
            {
               printf("Game \"%s\" not currently supported\n", argv[1]);
               return;
            }
            else
            {
               InitialSelPos = InitialDriverIndex;
               Save_INI();
            }
         }
      }
   }

   // allocate memory for decoded graphics set
   if ((DecodedSet = (BYTE *)malloc(0x20000)) == NULL)
      { printf("Memory Allocation failed\n"); exit(1); }

   // character currently being edited
   CurChar = create_bitmap(32,32);
   clear_to_color(CurChar, PALETTEBASE);   

   // set the display up
   InitialiseDisplay();
   setup_timers();

   ProcessCommands(InitialDriverIndex);

   if (ROMSet)
      free (ROMSet);
   free (DecodedSet);
   destroy_bitmap(CurChar);
   set_gfx_mode(GFX_TEXT,80,25,0,0);
   DeInit_INI();
   allegro_exit();
   end_credits();
}

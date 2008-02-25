#include <stdio.h>
#include <conio.h>

#include "allegro.h"

#include "drivers.h"
#include "age.h"
#include "mouse.h"
#include "help.h"

#define MAX_SCRATCH_X	(10)
#define MAX_SCRATCH_Y	(10)
#define SCRATCH_X	(238)
#define SCRATCH_Y	(18)
#define STATE_X		(200)
#define STATE_Y		(102)
#define EMPTY_CELL	(-1)
#define CURRENT_CHAR	(-2)


typedef struct 
{
   int Bank;
   int Character;
} ScratchCell;


ScratchCell ScratchArray[MAX_SCRATCH_X][MAX_SCRATCH_Y];

// storage for whats under the cursor
static BITMAP * TempChar;
static int tc_cx = 0;
static int tc_cy = 0;


enum { STATE_EDIT, STATE_SCRATCH };

void DisplayState(int State)
{
   switch(State)
   {
   case STATE_EDIT:
      textout(screen, font, "Mode: Edit   ", STATE_X, STATE_Y, 1);
      break;
      
   case STATE_SCRATCH:
      textout(screen, font, "Mode: Scratch", STATE_X, STATE_Y, 1);
      break;

   default:
      break;
   }
}


void ClearScratch(void)
{
   int x, y;
   
   for (x = 0; x < MAX_SCRATCH_X; x ++)
   {
      for (y = 0; y < MAX_SCRATCH_Y; y ++)
      {
	 ScratchArray[x][y].Character = EMPTY_CELL;
      }
   }
   
   // blat over any screen contents
   rectfill(screen, SCRATCH_X, SCRATCH_Y, 
      SCRATCH_X + MAX_SCRATCH_X - 1, 
      SCRATCH_Y + MAX_SCRATCH_Y - 1, 
      0);

}


void InitialiseScratch(void)
{
   rect (screen, SCRATCH_X - 2, SCRATCH_Y - 2, 
      SCRATCH_X + (MAX_SCRATCH_X * 8) + 1, 
      SCRATCH_Y + (MAX_SCRATCH_Y * 8) + 1, 
      1);

   TempChar = create_bitmap(32,32);
 
   ClearScratch();
   DisplayState(STATE_EDIT);
}



static void ReplaceCharacter(int x, int y)
{
   blit(TempChar, screen, 0, 0, 
      SCRATCH_X + (x * 8), SCRATCH_Y + (y * 8), 
      tc_cx, tc_cy);
}

static void DrawCursor(int x, int y)
{
   // store what is currently on the screen
   blit(screen, TempChar, SCRATCH_X + (x * 8), SCRATCH_Y + (y * 8), 
      0, 0, tc_cx, tc_cy);

   // draw cursor
   rect(screen, SCRATCH_X+(x*8), SCRATCH_Y+(y*8), 
      SCRATCH_X+tc_cx-1+(x*8), SCRATCH_Y+tc_cy-1+(y*8), 2);
}



void Scratch(BITMAP * CurChar)
{
   static int x = 0;
   static int y = 0;
   int key;
   int exit = 0;
   int UsingEditChar = 1;
   int FetchedChar = 0;
   int MouseDir = 0, MouseButt = 0;
   struct GFXLAYOUT * gl = Drivers[DriverNumber]->GfxInfo[CurrentBank].GfxLayout;
   tc_cx = gl->width;
   tc_cy = gl->height;
   
   // draw cursor
   DrawCursor(x, y);

   DisplayState(STATE_SCRATCH);
   
   do
   {
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
	    if (y < (MAX_SCRATCH_Y - (tc_cy / 8)))
	    {
	       // replace char over cursor
	       ReplaceCharacter(x, y);
	       y++;
	       // draw cursor over new position
	       DrawCursor(x, y);
	    }
	    break;

    case KEY_F1:
       DisplayHelp(HELP_SCRATCH_PAD);
       break;
       
	 case CURSOR_UP: 
	    if (y > 0)
	    {
	       // replace char over cursor
	       ReplaceCharacter(x, y);
	       y--;
	       // draw cursor over new position
	       DrawCursor(x, y);
	    }
	    break;
	    
	 case CURSOR_LEFT:
	    if (x > 0)
	    {
	       // replace char over cursor
	       ReplaceCharacter(x, y);
	       x--;
	       // draw cursor over new position
	       DrawCursor(x, y);
	    }
	    break;
	    
	 case CURSOR_RIGHT:
	    if (x < (MAX_SCRATCH_X - (tc_cx / 8)))
	    {
	       // replace char over cursor
	       ReplaceCharacter(x, y);
	       x++;
	       // draw cursor over new position
	       DrawCursor(x, y);

	    }
	    break;
	 }
	 
      }
      break;
      
      case 32: // space - place current char on scratch pad
	 if (UsingEditChar)
	 {
	    ScratchArray[x][y].Character = CURRENT_CHAR;
	    ScratchArray[x][y].Bank = CurrentBank;
	    blit(CurChar, screen, 0, 0, 
	       SCRATCH_X + (x * 8), SCRATCH_Y + (y * 8), 
	       tc_cx, tc_cy);
	 }
	 else
	 {
	    ScratchArray[x][y].Character = FetchedChar;
	    ScratchArray[x][y].Bank = CurrentBank;
	    blit(CurChar, screen, 0, 0, 
	       SCRATCH_X + (x * 8), SCRATCH_Y + (y * 8), 
	       tc_cx, tc_cy);
	 }

	 // redraw cursor
	 DrawCursor(x, y);

	 break;
	 
      case 'z': // remove char at scratch position
      case 'Z':
	 ScratchArray[x][y].Character = EMPTY_CELL;
	 rectfill(screen, SCRATCH_X+(x*8), SCRATCH_Y+(y*8), 
	    SCRATCH_X+(tc_cx-1)+(x*8), 
	    SCRATCH_Y+(tc_cy-1)+(y*8), 0);
	 DrawCursor(x, y);

	 break;
	 
      case 'c': // clear scratch
      case 'C':
	 ClearScratch();
	 break;
#if 0	 
      case 'g': // get a character from the char set
      case 'G':
      {
	 int CharNo = -1;
	 
	 ReplaceCharacter(x, y, CurChar);	 

//	 CharNo = SelectChar(/*must add pointer to current graph layout here */);
	 if (CharNo != -1)
	 {
	    // if the user has selected a char then draw with that
	    FetchedChar = CharNo;
	    UsingEditChar = 0;
	 }
	 
	 // redraw cursor
	 rect(screen, SCRATCH_X+(x*8), SCRATCH_Y+(y*8), 
	    SCRATCH_X+7+(x*8), SCRATCH_Y+7+(y*8), 2);

		// JERRY
		wait_for_button_release();
	 break;
      }
#endif 
      case 9: // tab - revert back to edit mode
	 exit = 1;
	 break;
      }
      
	wait_for_button_release(); // JERRY 11/20/97
   } while (!exit);
   
   // replace char over cursor
   ReplaceCharacter(x, y);
   DisplayState(STATE_EDIT);
}




// dynamically update the scratch pad if it contains any
// currently being edited characters
void UpdateScratch(BITMAP * CurChar)
{
   int x, y;
   
   for (x = 0; x < MAX_SCRATCH_X; x ++)
   {
      for (y = 0; y < MAX_SCRATCH_Y; y ++)
      {
	 if (ScratchArray[x][y].Character == CURRENT_CHAR)
	 {
	    blit(CurChar, screen, 0, 0, 
	       SCRATCH_X + (x * 8), SCRATCH_Y + (y * 8), tc_cx, tc_cy);
	    
	 }
      }
   }
}




// Makes previous character not get updated next time the user
// does an edit and if there are any of the new character in
// the scratch pad then these are marked to be auto updated
void SelectNewCurrenCharacter(int NewChar)
{
   static int PreviousChar = -1; // remember previous
   int x, y;
   
   for (x = 0; x < MAX_SCRATCH_X; x ++)
   {
      for (y = 0; y < MAX_SCRATCH_Y; y ++)
      {
	 // convert dynamically updating chars to previous static char
	 if (ScratchArray[x][y].Character == CURRENT_CHAR)
	 {
	    // if there is no previous character
	    if (PreviousChar != -1)
	    {
	       // replace with previous character
	       ScratchArray[x][y].Character = PreviousChar;
	    }
	 }
	 else
	 // convert new char to make it update dynamically
	 if ((ScratchArray[x][y].Character == NewChar) &&
	    (ScratchArray[x][y].Bank == CurrentBank))
	 {
	    ScratchArray[x][y].Character = CURRENT_CHAR;
	 }
      }
   }
   
   PreviousChar = NewChar;
}





#include <stdio.h>
#include <conio.h>
#include <ctype.h>
#include <string.h>

#include "allegro.h"
#include "help.h"


struct HELPLINE {
   char HelpLine[21];
};
   
struct HELPTEXT {
   char HelpTitle[21];
   int NumLines; // max 20;
   struct HELPLINE * HelpLines;
};


struct HELPLINE Help_CharEdit_Lines[] = {
   { "L    Load Font"},
   { "AltS Save Font"},
   { "SPC  Draw Pixel" },
   { "Z    Remove Pixel" },
   { "G    Get Char" },
   { "P    Put Char" },
   { "C    Select Colour" },
   { "< >  Change Palette" },
   { "PUPD Change Bank" },
   { "+ -  Zoom" },
   { "T    Bitmap Tools" },
   { "TAB  Scratch Mode" },
   { "F5   Map Editor" },
   { "F6   Text Editor " },
   { "F9   Save PCX" },
   { "F10  Load PCX" },
   { "F12  Screen Snapshot" },
   { "ESC  Quit AGE" }
};
      

struct HELPTEXT Help_CharEdit = {
   "Character Edit",
   18,
   Help_CharEdit_Lines
};
   

struct HELPLINE Help_ColourSelect_Lines[] = {
   { "< >  Change Palette" },
   { "SPC  Sel. Foreground" },
   { "Z    Sel. Background" },
   { "ESC  Quit Selection" }
};


struct HELPTEXT Help_ColourSelect = {
   "Colour Select",
   4,
   Help_ColourSelect_Lines
};


struct HELPLINE Help_SelectChar_Lines[] = {
   { "SPC  Select" },
   { "ESC  Quit Selection" }
};


struct HELPTEXT Help_SelectChar = {
   "Character Select",
   2,
   Help_SelectChar_Lines
};


struct HELPLINE Help_ScratchPad_Lines[] = {
   { "SPC  Place Char" },
   { "Z    Remove Char" },
   { "C    Clear Scratch" },
   { "TAB  Edit Mode"}
};


struct HELPTEXT Help_ScratchPad = {
   "Scratch Pad",
   4,
   Help_ScratchPad_Lines
};


struct HELPLINE Help_MapEdit_Lines[] = {
   { "AltS Save Map" },
   { "U    Scroll Up" },
   { "D    Scroll Down" },
   { "S    Stretch Map" },
   { "R    Reset Map" },
   { "+ -  Change Tile" },
   { "P    Toggle Pellet" },
   { "PUPD Change Map" },
   { "B    View Base" },
   { "O    View Overlay" },
   { "H    View Power-ups" },
   { "G    Get Tile" },
   { "< >  Change Palette" },
   { "3    320x200 Res"},
   { "6    640x480 Res"},
   { "F12  Snapshot"},
   { "ESC  Quit Map Editor" }
};


struct HELPTEXT Help_MapEdit = {
   "Map Editor",
   18,
   Help_MapEdit_Lines
};


struct HELPLINE Help_TextEdit_Lines[] = {
   { "AltS Save Text" },
   { "AltL Reload Text" },
   { "PUPD Change Category"},
   { "< >  Change Palette"},
   { "Ins  Insert Space"},
   { "Home Start of Text"},
   { "End  End of Text"},
   { "Del  Delete Text" },
   { "CUp  Previous String"},
   { "CDn  Next String"},
   { "F12  Snapshot"},
   { "ESC  Quit Text Edit" }
};


struct HELPTEXT Help_TextEdit = {
   "Text Editor",
   12,
   Help_TextEdit_Lines
};



// must correspond to enum in help.h
struct HELPTEXT * HelpText[] = {
   &Help_CharEdit,
   &Help_SelectChar,
   &Help_ScratchPad,
   &Help_ColourSelect,
   &Help_MapEdit,
   &Help_TextEdit
};
                              

void DisplayHelp(HELP_AREA HelpArea)
{
   int BoxY = HelpText[HelpArea]->NumLines * 8 + 14;
   int ScreenY = (200-BoxY) / 2;
   int ScreenCY = BoxY + ScreenY - 1;
   int i;
   
   // save background
   BITMAP * Background = create_bitmap(170,BoxY);
   blit(screen, Background, 75,ScreenY, 0,0, 170,BoxY);

   // display border
   rectfill(screen, 75,ScreenY,244,ScreenCY, 0);
   rect(screen, 75,ScreenY,244,ScreenCY, 1);
   
   // title
   textout_centre(screen, font, HelpText[HelpArea]->HelpTitle, 160,ScreenY + 2, 2);
 
   // display help
   for (i = 0; i < HelpText[HelpArea]->NumLines; i ++)
   {
      textout(screen, font, HelpText[HelpArea]->HelpLines[i].HelpLine, 77,
              ScreenY + 12 + (i * 8), 1);
   }
   
   // wait for key press before exiting
   getch();
   
   // restore screen image
   blit(Background, screen, 0,0, 75,ScreenY, 170,BoxY);
   destroy_bitmap(Background);
}
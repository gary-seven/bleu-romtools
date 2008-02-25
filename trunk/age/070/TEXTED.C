// text editing tools
// January, 1998 jerry@mail.csh.rit.edu
//   http://www.csh.rit.edu/~jerry
//
// special thanks to M.C.Silvius (msilvius@caiw.nl) for pacman
//  variant name info & motivation. If his pacman name editor, pacname3.exe
//  i would never have gotten around to writing this module.  :)
// "Hey!  how dare he make a program that edits the strings in a pacman
//  ROM before I add that functionality to AGE!?!!?"   ;)
//  Gotta love competition... ;)


#include <stdio.h>
#include <conio.h>
#include <string.h> // for memset()
#include "allegro.h"
#include "drivers.h"
#include "age.h"
#include "fileio.h"
#include "palette.h"
#include "tools.h"
#include "maped.h"
#include "snap.h"
#include "help.h"

int catno;
int strno;
int charno; // charon
BITMAP * display = NULL;
BITMAP * TempChar = NULL;



int Texted_Display_String(unsigned long spos, unsigned long length, int x, int y)
{
#if 0
    int chn;
    int count;
    struct GFXLAYOUT * gl = Driver.GfxInfo[GetGfxBank()].GfxLayout;
    //clear_to_color(TempChar, PALETTEBASE);

    for (count=0; count < length ; count++)
    {
	// first get the character number
	chn = dataroms[(long)count+spos];
    
	// then copy that bitmap over...
	GetImageFromSet(gl, chn, TempChar);
	ShiftColor(TempChar);
    
	// and blit it to the appropriate location
	blit(TempChar, display, 0,0, x+(count*gl->width), y,
	     gl->width, gl->height);
    }
    return (x+(count*gl->width));
#endif
return 0;
}


void Texted_Display(void)
{
#if 0
    char text[250];
    struct ROMDATA * rd = Driver.RomData;
    struct GFXLAYOUT * gl = Driver.GfxInfo[GetGfxBank()].GfxLayout;
    int count;
    int sxpos, expos;
    int ypos;
    char * src;

    clear(display);
    textout_centre(display, font, title_text, 160, 1, PAL_LTGREY);

    sprintf(text, "%d/%d", catno+1, rd->numCategories);
    textout(display, font, text, 10, 20, PAL_GREEN);
    textout_centre(display, font, rd->TextData[catno].category_name,
	    160, 20, PAL_YELLOW);

    for (count=0 ; count<rd->TextData[catno].numStrings ; count++)
    {
	src = rd->TextData[catno].strings[count].stringname;
	if (strlen(src) >12) src[12] = '\0';

	ypos = 40+(count*(gl->height+2));

	sprintf(text, "%12s %c",src, (count == strno)?'<':'[');
	textout(display, font, text, 10, ypos,
	        (count == strno)?PAL_LTGREY:PAL_GREY);

	sxpos = 10+text_length(font, text);
	expos = Texted_Display_String(
		    rd->TextData[catno].strings[count].startpos,
		    rd->TextData[catno].strings[count].length,
		    sxpos, ypos);

	sprintf(text,"%c", (count == strno)?'>':']');
	textout(display, font, text, expos+3, ypos,
	        (count == strno)?PAL_LTGREY:PAL_GREY);

	if (count == strno)
	{
	    // display the cursor
	    hline(display,
		    sxpos+(charno * gl->width), ypos+gl->height,
		    sxpos+((charno+1) * gl->width)-1,
		    (timer_counter & 0x04)?PAL_YELLOW:PAL_BLACK);

	}
    }

    blit(display, screen, 0,0, 0,0, 320,200);
#endif
}


char Delete_Lookup(void)
{
#if 0
    struct ROMDATA * rd = Driver.RomData;
    int count=0;

    while (rd->TextData[catno].lu[count].ascii != 0)
        count++;

    return (rd->TextData[catno].lu[count].rom);
#endif
return 0;
}

char Texted_Lookup(char ascii)
{
#if 0
    struct ROMDATA * rd = Driver.RomData;
    int count=0;

    while (rd->TextData[catno].lu[count].ascii != 0)
    {
	if (rd->TextData[catno].lu[count].ascii == ascii)
	    return (rd->TextData[catno].lu[count].rom);
        count++;
    }
    return 0;
#endif
return 0;
}


void Texted_Delete(long startpos, long length)
{
#if 0
    int count = 0;
    dataroms[startpos] = Delete_Lookup();
    if (length == 0)
	return;
    while(count < length)
    {
	if (count < length-1)
	    dataroms[startpos+count] = dataroms[startpos+count+1];
	else
	    dataroms[startpos+count] = Delete_Lookup();
	count++;
    }
#endif
}

void Texted_Backspace(long startpos, long length)
{
#if 0
    if (charno > 0)
    {
	charno--;
	Texted_Delete(startpos-1, length+1);
    }
#endif
}

void Texted_Insert(long startpos, long length)
{
#if 0
    int count = length-2;
    while (count >= 0)
    {
	dataroms[startpos+count+1] = dataroms[startpos+count];
	count--;
    }
    dataroms[startpos] = Delete_Lookup();
#endif
}

void Edit_Text_Strings(void)
{
#if 0
    int ch;
    char newchar;
    int done = 0;
    struct ROMDATA * rd = Driver.RomData;

    catno = 0;
    strno = 0;
    charno = 0;

    // do the stuff in here...
    while (!done)
    {
	Texted_Display();
	if (kbhit())
	    ch = getch();
	else
	    ch = -1;

	switch (ch){
	case(-1):
	    break;

	case(ESCAPE):
	    if (Dialog_Box("Exit the text editor?",
			   "Strike [esc] again to exit.") == ESCAPE)
		done = 1;
	    break;

	case (BACKSPACE):
	    // delete the character at charno+1
	    //Dialog_Box("foo", "Yowza");
	    Texted_Backspace(
		rd->TextData[catno].strings[strno].startpos+(long)charno,
		rd->TextData[catno].strings[strno].length-charno);
	    break; // not "return;" ... duh

   case ',': // palette --
		if (ModifyPaletteNumber(-1))
         SetColours(Driver.GfxInfo[CurrentBank].GfxLayout,0);
      break;

   case '.': // palette ++
		if (ModifyPaletteNumber( 1))
         SetColours(Driver.GfxInfo[CurrentBank].GfxLayout,0);
      break;
      
	case(0):
	    ch = getch();
	    switch(ch){
          case (KEY_F1):
             DisplayHelp(HELP_TEXT_EDIT);
             break;
               
	    case (KEY_HOME):
	        charno = 0;
		break;

	    case (KEY_END):
	        charno = rd->TextData[catno].strings[strno].length-1;
		break;

	    case (KEY_INSERT):
	        // shift the string to the right, and insert
		// a delete character
		Texted_Insert(
		    rd->TextData[catno].strings[strno].startpos+charno,
		    rd->TextData[catno].strings[strno].length-charno);
		break;

	    case (KEY_DEL):
		Texted_Delete(
		    rd->TextData[catno].strings[strno].startpos+charno,
		    rd->TextData[catno].strings[strno].length-charno);
		break;

	    case (ALT_L): // alt-s for save
	        {
		int ch = Dialog_Box("Really Reload?", "Strike 'y' to reload");
		if (ch == 'y' || ch == 'Y')
		    LoadRomData();
		}
		break;

	    case (ALT_S): // alt-s for save
		SaveRomData();
		(void)Dialog_Box("ROM data saved.",
				 "Hit any key to continue.");
		break;

	    case(KEY_LEFT):
	        if (charno > 0)
		    charno--;
	        break;
	    case(KEY_RIGHT):
	        if (charno < rd->TextData[catno].strings[strno].length-1)
		    charno++;
	        break;

	    case(KEY_UP):
	        if (strno > 0)
		{
		    strno--;
		    charno = 0;
		}
	        break;
	    case(KEY_DOWN):
	        if (strno < rd->TextData[catno].numStrings-1)
		{
		    strno++;
		    charno = 0;
		}
	        break;

	    case(KEY_PGUP):
		if (catno < rd->numCategories-1)
		{
		    catno++;
		    strno = 0;
		    charno = 0;
		}
	        break;
	    case(KEY_PGDN):
		if (catno > 0)
		{
		    catno--;
		    strno = 0;
		    charno = 0;
		}
	        break;

	    case (F12):
		screen_snap();
		break;
	    default:
		break;
	    }
	    break;
	default:
	    // look up the character in the lookup table
	    newchar = Texted_Lookup((char)(ch&0xff));
	    if (newchar)
	    {
		dataroms[rd->TextData[catno].strings[strno].startpos
		         + charno] = newchar;

	        if (charno < rd->TextData[catno].strings[strno].length-1)
		    charno++;
	    }
	    break;
	}
    }
#endif
}

void Edit_Text(void)
{
#if 0
    BITMAP * Background;
    int oldGfxBank;
    struct GFXLAYOUT * gl = Driver.GfxInfo[GetGfxBank()].GfxLayout;

    // look in the driver structure
    if (Driver.RomData == NULL)
    {
	Dialog_Box("Sorry. No ROM data available", "Hit any key to continue");
	return;
    }
    if (Driver.RomData->TextData == NULL)
    {
	Dialog_Box("Sorry. No text data available", "Hit any key to continue");
	return;
    }

    if (!LoadRomData())
    {
	// save the background...
	Background = create_bitmap(320,200);
	display = create_bitmap(320,200);
    
	blit(screen, Background, 0,0, 0,0, 320,200);
	oldGfxBank = GetGfxBank();
	ChangeGfxBank(0); // assume the text is in bank 0
	
	if (TempChar)
	    destroy_bitmap(TempChar);
	TempChar = create_bitmap(gl->width, gl->height);
    
	Edit_Text_Strings();

	if (TempChar)
	    destroy_bitmap(TempChar);

	// now restore the screen
	set_gfx_mode(GFX_VGA,320,200,0,0);
	SetReservedColours();
	SetColours(Driver.GfxInfo[CurrentBank].GfxLayout,0);
	blit(Background, screen, 0,0, 0,0, 320,200);
    
	destroy_bitmap(display);
	destroy_bitmap(Background);
	ChangeGfxBank(oldGfxBank);
    } else {
	Dialog_Box("Unable to load rom data", "Press any key to continue");
    }

    if (dataroms) {
	free(dataroms);
	dataroms = NULL;
    }

    if (editbuf) {
	free(editbuf);
	editbuf = NULL;
    }

    if (overbuf) {
	free(overbuf);
	overbuf = NULL;
    }
#endif
}

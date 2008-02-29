/* Turaco CL GUI Experiment
**
**	main.c
**
**  2008-02-08+  Scott Lawrence
**  http://umlautllama.com
*/

/* This code is considered to be freeware.  Do with it as you wish.
** I claim no responsibility for anything bad that might be a direct
** or indirect result of the usage of this code.
*/

#include <curses.h>
#include <stdio.h>
#include <stdlib.h> /* for exit() */
#include <signal.h> /* for x windows resize signal */
#include <string.h> /* for strncpy() */
#include "tmgui.h"  /* the gui interface */
#include "bits.h"   /* bitmap operations */
#include "filereq.h"


/* gui_menuitems
**	this is the actual list of items used for all of the menus
*/
gui_menuitem gui_menuitems[] = {
	/* main menu - starts at 100 */
	{ 100, 200, 'a', "_Application", "Application related options" },
	{ 101, 300, 'f', "_File", "File loading, saving and exporting" },
	{ 102, 400, 's', "_Sprite", "Sprite retrieval and storage" },
	{ 104, 500, 'b', "_Bitmap", "Bitmap manipulation tools" },
	{ 105, 600, 'c', "_Color", "Color manipulations" },
	{ 106, 700, 't', "_Test", "Internal debugging tests" },
	{ 199, END },

	/* Application menu... */
	{ 200, CMD, 'v', "_Version", "Application version " VERSION },
	{ 201, CMD, 'a', "_Author", "Scott Lawrence  http://umlautllama.com" },
	{ 202, CMD, 'h', "_Help", "Application help" },
	{ 203, CMD, 'q', "_Quit", "Exit from from the application" },
	{ 299, END },

	/* File IO menu */
	{ 300, 320, 'l', "_Load", "Load in sprite data" },
	{ 301, CMD, 'r', "_Revert", "Revert to disk data" },
	{ 302, CMD, 's', "_Save", "Save out to disk" },
	{ 303, 330, 'a', "Save _As", "Save out to disk with a different name" },
	{ 304, END },

	{ 320, CMD, 'p', "_PCX", "Load from PCX image file" },
	{ 321, CMD, 'r', "_ROM", "Load from ROM data file" },
	{ 322, END },

	{ 330, CMD, 'p', "_PCX", "Save as PCX image file" },
	{ 331, CMD, 'r', "_ROM", "Save as ROM data file" },
	{ 332, END },

	{ 399, END },

	/* Sprite menu */
	{ 400, CMD, 'b', "_Bank", "Select Bank in ROM" },
	{ 401, CMD, 'g', "_Get", "Get sprite from ROM" },
	{ 402, CMD, 'p', "_Put", "Put sprite into ROM" },
	{ 403, CMD, 'e', "_Edit", "Edit current sprite" }, 
	{ 404, CMD, 'c', "_Copy", "Copy the current sprite aside" },
	{ 405, CMD, 'p', "_Paste", "Paste the spare to the current position" },
	{ 499, END },

	/* Bitmap menu */
	{ 500, CMD, 'x', "_X Flip", "Horizontal Flip" },
	{ 501, CMD, 'y', "_Y Flip", "Vertical Flip" },
	{ 502, CMD, 'r', "_Rotate", "Rotate 90 clockwise" },
	{ 503, CMD, 'c', "_Clear", "Clear the bitmap field" },
	{ 504, CMD, 's', "_Swap", "Swap with the spare field" },
	{ 505, CMD, 't', "_To Swap", "Copy this bitmap to swap" },
	{ 506, 520, 'm', "_Move", "Slide the image around" },
	{ 507, CMD, 'u', "_Undo", "Undoes the last manipulation" },
	{ 510, END },

	{ 520, CMD, 'l', "_Left", "Slide the image to the left" },
	{ 521, CMD, 'r', "_Right", "Slide the image to the right" },
	{ 522, CMD, 'u', "_Up", "Slide the image up" },
	{ 523, CMD, 'd', "_Down", "Slide the image down" },
	{ 530, END },

	{ 599, END },

	/* color */
	{ 600, CMD, 's', "_Swap", "Swaps palette with bitmap colors" },
	{ 601, CMD, 'f', "_Fill", "Fills bitmap color with palette color" },
	{ 602, CMD, 'p', "_Pick", "Pick up color in bitmap editor" },
	{ 699, END },

	/* debug test */
	{ 700, CMD, 'p', "_Popup", "Popup requestor" },
	{ 701, CMD, 'f', "_Files", "File requestors" },
	{ 799, END },

	/* end of the entire list -- id=999 */
	{ 999, END }
};


/* this is for the confirmation popup */
char * noyes[] = { "No",
		   "Yes, quit.", NULL };

/* for the test items */
char * test_items[] = {
	"Item One",
	"Numero dos",
	"drei",
	"4444444",
	"5ive",
	NULL
};

struct gui_filereq frq;


/* menu_callback
**	the callback for when a user selects an item
*/
void menu_callback( gui_handle * gui, int item_id )
{
	switch( item_id ) {
		/* application */
		case( 203 ):
			if( gui_popup_requestor( gui, "Really quit?",
						noyes, 0 ))
				gui->quit = 1;
			break;

		/* files */
		case( 320 ):
			if( gui_popup_requestor( gui, "Lose changes?", 
						noyes, 0 ))
				/* Load from PCX here */
				;
			break;
		case( 321 ):
			if( gui_popup_requestor( gui, "Lose changes?", 
						noyes, 0 ))
				;
				/* Load from ROM here */
			break;
		case( 330 ):
			/* save as PCX here */
			break;
		case( 331 ):
			/* save as ROM here */
			break;


		/* edit */
		case( 400 ):
			gui_set_status( gui, COLOR_STATUS_GOOD, " Good" );
			break;
		case( 401 ):
			gui_set_status( gui, COLOR_STATUS_WARNING, " Warning" );
			break;
		case( 402 ):
			gui_set_status( gui, COLOR_STATUS_BAD, " Bad" );
			break;


		/* sprite */
		case( 500 ): bits_flip_horiz( gui ); break;
		case( 501 ): bits_flip_vert( gui ); break;
		case( 502 ): bits_rotate_90( gui ); break;
		case( 503 ): bits_clear( gui ); break;
		case( 504 ): bits_swap( gui ); break;
		case( 505 ): bits_copy_to_swap( gui ); break;
		case( 507 ): bits_swap_undo( gui ); break;
		case( 520 ): bits_shift( gui, -1, 0 ); break;
		case( 521 ): bits_shift( gui, +1, 0 ); break;
		case( 522 ): bits_shift( gui, 0, -1 ); break;
		case( 523 ): bits_shift( gui, 0, +1 ); break;


		/* color */
		case( 600 ): bits_swap_colors( gui ); break;
		case( 601 ): bits_fill_all( gui ); break;
		case( 602 ):
			gui->pal.cursor = bits_getp( gui->main, 
					gui->bited.cx, gui->bited.cy );
			break;


		/* test gui elements */
		case( 700 ):
			/* our popup requestor */
			gui_popup_requestor( gui, "Requestor", test_items, 0);
			break;

		case( 701 ):
			/* our file requestor */
			snprintf( frq.title, LEN_TITLE, "Select your file..." );
			snprintf( frq.path, LEN_PATH, "/" );
			snprintf( frq.accept, LEN_BUTTON, "Open" );
			snprintf( frq.cancel, LEN_BUTTON, "Cancel" );
			snprintf( frq.extensions, LEN_EXT, ".jpg" );
			(void) gui_file_requestor( gui, &frq );

			/* and the other way to call it */
			(void) gui_file_requestor_p( gui, &frq,
				"Select another", ".",
				"Yup!", NULL, ".c .pcx .jpg" );
		break;
	}
}


/* main
*/
int main( int argc, char ** argv )
{
	gui_handle *gui = gui_startup( gui_menuitems, menu_callback );
	if( gui )
	{
		/* for the demo, we'll set up an 8 color palette */
		gui_initialize_palette( gui, 8 );

		/* for the demo, we'll set up an 16x16 sprite */
		gui_initialize_bited( gui, 16, 16 );

		/* and setup the second display */
		gui_initialize_second( gui, 16, 16 );

		gui_runloop( gui );
		gui_shutdown( gui );
	}
	return( 0 );
}

// if you want the Snoop feature to be enabled, use:
//   #define SNOOP_ENABLE
// otherwise
//   #undef SNOOP_ENABLE
// just be aware that most of the editing,saving and sizing
// features WILL NOT WORK!
#undef SNOOP_ENABLE

// modes - passed into the display_info function
enum { MODE_EDIT, MODE_TILE };

extern BYTE * dataroms;

void ShiftColor(BITMAP * bp);
void PutTileUnderCursor(int newvalue);
unsigned char GetTileUnderCursor(void);
void MapCursor(BITMAP * playfield, int x, int y); //draw cursors where needed
void EditMap(void);

// these map editing things are available to the maped routines...
extern int mapno;		// which map is being edited
extern BYTE * dataroms;		// the rom data space
extern char * editbuf;		// the edit buffer space (base map data)
extern char * overbuf;		// the edit buffer space (overlay map data)
extern int display_base;        // display the base map (zero if no)
extern int display_overlay;     // display the overlay (zero if no)
extern int display_hardcoded;   // display the hardcoded (zero if no)
extern int display_stretch;	// stretch the display?
extern signed long snoop_offset; // for snooping... offset from structures


// from the 256 colour palette of mode 13 the first colours are
// used by this application
#define PALETTEBASE (16)

#define PAL_BLACK 	(0)
#define PAL_GREY 	(1)
#define PAL_LTGREY 	(2)
#define PAL_DKGREY 	(3)
#define PAL_RED		(4)
#define PAL_YELLOW	(5)
#define PAL_GREEN	(6)
#define PAL_CYAN	(7)
#define PAL_BLUE	(8)


void InitialisePalette(void);
void SetReservedColours(void);
void SelectColour(void);
int GetLeftMouseColour(void);
int GetRightMouseColour(void);
void SetColours(struct GFXLAYOUT * gl, int display_palette);
int ModifyPaletteNumber(int No);
void SetLeftMouseColour(int colour);
void SetRightMouseColour(int colour);

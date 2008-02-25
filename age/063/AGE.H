
typedef unsigned char BYTE;

extern char title_text[];

#define CURSOR_UP	(72)
#define CURSOR_DOWN	(80)
#define CURSOR_LEFT	(75)
#define CURSOR_RIGHT	(77)

#define ALT_CURSOR_UP		(152)
#define ALT_CURSOR_DOWN		(160)
#define ALT_CURSOR_LEFT		(155)
#define ALT_CURSOR_RIGHT	(157)

#define CTRL_CURSOR_UP		(141)
#define CTRL_CURSOR_DOWN	(145)
#define CTRL_CURSOR_LEFT	(115)
#define CTRL_CURSOR_RIGHT	(116)

#define ESCAPE		(27)

#define PAGE_UP		(73)
#define PAGE_DOWN	(81)
#define BACKSPACE	(8)

#define HOME		(71)
#define ALT_KYP_PLUS	(78)
#define ALT_KYP_MINUS	(74)
#define CTRL_KYP_PLUS   (144)
#define CTRL_KYP_MINUS  (142)

#define ALT_S		(31)
#define ALT_L		(38)

#define F12             (134)

#define FONT_TOP	(135)
#define FONT_TITLE_POS (FONT_TOP-9)

extern int ConvertMouseDirection(int MouseDir);

unsigned long timer_counter;

extern BYTE * ROMSet;
extern BYTE * DecodedSet;
extern struct GAMEINFO * Drivers[];
extern int DriverNumber;
extern int CurrentBank;

// exported functions
void BlitChar(BITMAP * bmp, int x, int y, int width, int height, int No);
void DrawChar(int x, int y, int width, int height, int charno);
int SelectChar(struct GFXLAYOUT *gl);
int EnlargeChar(int No, struct GFXLAYOUT * gl);
int GetCharFromSet(struct GFXLAYOUT * gl);
void DisplayFont(struct GFXLAYOUT *gl, int ColourChange);
void InitialisePalette(void);
int Dialog_Box(char * topline, char * bottomline);
int GetGfxBank(void);
void ChangeGfxBank(int bank);
void ClearZoomWindow(void);
void ZoomChar(void);

int ReadFromDriverIniFile(char *filename);
void DisplayError(const char * pFormatStr, ...);
int LoadRoms(void);
int SaveRoms(void);
int LoadSelector(void);
void ListRoms(void);
void ListFull(void);
void ListHtml(void);
void ListSupported(void);
void MakePatch(void);
void ApplyPatch(void);
void WriteAllPalettes(void);
void FreeDriverList(void);

// error types
enum { E_LOAD, E_SAVE, E_SAVEFWRITE };

int LoadRomData(void);
void SaveRomData(void);

// Initial load selector postion
extern int InitialSelPos;

// how many elements in an array?
#define ELEMENTS(x) (sizeof(x) / sizeof(x[0]))

// the driver filename
extern char INIDriverName[];

// The path where the roms are stored
extern char ROMPath[];
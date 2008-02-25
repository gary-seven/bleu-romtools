void DisplayError(int ErrorName, char * ROMName);
int LoadRoms(void);
void SaveRoms(void);
int LoadSelector(void);
void ListRoms(void);
void ListFull(void);
void ListHtml(void);
void ListSupported(void);
char * ShortNameFromIndex(int Index, char * ShortName, int ShortNameSize);
int IndexFromShortName(char * GameName);
void MakePatch(int GameIndex);
void ApplyPatch(int GameIndex);

// error types
enum { E_LOAD, E_SAVE, E_SAVEFWRITE };

int LoadRomData(void);
void SaveRomData(void);

// Initial load selector postion
extern int InitialSelPos;

// how many elements in an array?
#define ELEMENTS(x) (sizeof(x) / sizeof(x[0]))

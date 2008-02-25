#include <stdio.h>
#include <conio.h>
#include <ctype.h>
#include <string.h>
#include <sys/stat.h>

#include "allegro.h"

#include "drivers.h"
#include "age.h"
#include "mouse.h"
#include "maped.h"
#include "palette.h"
#include "fileio.h"
#include "snap.h"
#include "ini.h"


#define MAX_SELECTIONS	(14)

const char * BackupDir = "AGEBAK";
const char * PatchDir = "AGEPATCH";

// initialised properly from ini file
int InitialSelPos = -1;

void DisplayError(int ErrorName, char * ROMName)
{
   BITMAP * Background = create_bitmap(170,22);
   
   // copy screen image
   blit(screen, Background, 75,89, 0,0, 170,22);
   
   // display error
   rectfill(screen, 75,89,244,110, 0);
   rect(screen, 75,89,244,110, 1);
   
   if (ErrorName == E_LOAD)
      textout_centre(screen, font, "Load Error", 160,91, 2);
   else if (ErrorName == E_SAVE)
      textout_centre(screen, font, "Save Error", 160,91, 2);
   else
      textout_centre(screen, font, "Save/Write Error", 160,91, 2);
      
   textout_centre(screen, font, ROMName, 160, 101, 1);
   
   // wait for key press
   getch();
   
   // restore screen image
   blit(Background, screen, 0,0, 75,89, 170,22);
   destroy_bitmap(Background);
}

long DetermineDataSize(int nroms, struct GFXROMINFO * gri)
{
    long totalsize = 0l;
    int romc;
    // first figure out how much ram is needed
    for (romc = 0; romc < nroms ; romc++)
    {
	totalsize += gri[romc].Size;
    }
    return totalsize;
}

// returns the current number of drivers
int CountDrivers(void)
{
   int i = 0;

   while (Drivers[i] != 0)
      i ++;

   return i;
}


char * ShortNameFromIndex(int Index, char * GameName, int GameNameSize)
{
   // work out the 8 char name for this driver. A bit of a hack really
   // take the name from the prepended directory name added to the rom
   // names
   int j = 0;

   strncpy(GameName, Drivers[Index]->GfxRoms[0].ROMName, GameNameSize);

   do
   {
      if (GameName[j] == '\\')
      {
         GameName[j] = 0;
         break;
      }
      j ++;
   }
   while (j < GameNameSize);

   return GameName;
}



// returns -1 if the specified game is supported else the game index
int IndexFromShortName(char * GameName)
{
   int i;
   int NumDrivers = CountDrivers();
   char RomName[100];

   for (i = 0; i < NumDrivers; i ++)
   {
      ShortNameFromIndex(i, RomName, ELEMENTS(RomName));

      // got the short rom name now check if it is the one specified
      // in the argument
      if (stricmp(RomName, GameName) == 0)
         return i;
   }

   return -1;
}



int LoadDataFromFiles(int nroms, struct GFXROMINFO * gri, BYTE ** loadmem)
{
    int romc;
    FILE * fp;

    // malloc the ram
    if (*loadmem)
    {
	free(*loadmem);
	*loadmem = NULL;
    }
    *loadmem = (char *)malloc(DetermineDataSize(nroms, gri));
    if (*loadmem == NULL)
    	return 1;

    // load the data into the dataroms
    for (romc = 0; romc < nroms ; romc++)
    {
	if ((fp = fopen(gri[romc].ROMName, "rb")) == NULL)
	{
	    // file not found
	    DisplayError(E_LOAD, gri[romc].ROMName);
	    return 1;
	}
    
	if (fread((*loadmem+gri[romc].LoadAddress), 1,
		  gri[romc].Size, fp) != gri[romc].Size)
	{
	    // bad load
	    DisplayError(E_LOAD, gri[romc].ROMName);
	    fclose(fp);
	    return 1;
	}
    
	fclose(fp);
    }
    return 0;
}


int LoadRomData(void)
{
    // load the data into the internal data space
    struct ROMDATA * rd = Drivers[DriverNumber]->RomData;
    return LoadDataFromFiles(rd->numDataRoms, rd->DataRoms, &dataroms);
}

int LoadRoms(void)
{
    // load the graphis into the internal data space
    struct GFXROMINFO * pRomInfo = Drivers[DriverNumber]->GfxRoms;
    return LoadDataFromFiles(Drivers[DriverNumber]->NumGfxRoms,
			     pRomInfo, &ROMSet);
}



/* sort the games in to alphabetical order for the sake of output */
void SortGames(int * GameIndex)
{
   /* first fill array with current order */
   int i = 0, j, k;

   while (Drivers[i])
   {
      GameIndex[i] = i;
      i++;
   }

   i--;
   
   for (k = 0; k < i; k++)
   {
      for (j = 0; j < i - k; j++)
      {
	 if (strcmp(Drivers[GameIndex[j]]->GameName, 
		    Drivers[GameIndex[j+1]]->GameName) > 0)
	 {
	    /* swap */
	    int tmp = GameIndex[j];
	    GameIndex[j] = GameIndex[j+1];
	    GameIndex[j+1] = tmp;
	 }
      }
   }
}


void DrawSelections(int * Index, int NumDrivers, int TopIndex)
{
   int i = TopIndex;
   int pos = 0;

   // clear list first
   rectfill(screen, 80,35,239,174, 0);
   
   // draw new list
   while (i < NumDrivers && i < (MAX_SELECTIONS + TopIndex))
   {
      textout(screen, font, Drivers[Index[i]]->GameName, 80, 35 + (pos * 10), 1);      
      i++;
      pos++;
   }
}


int LoadSelector(void)
{
   BITMAP * Background = create_bitmap(320,200);
   int Selection = 0;
   int key, exit = 0;
   int MouseDir = 0, MouseButt = 0;
   int NumDrivers = CountDrivers();
   int * Index = (int *) malloc(NumDrivers * sizeof(int)); // this is important
   int TopIndex = 0;
   char romsetname[10];
   int i = 0;
   
   if (!Index) return -1;
   
   // make a lookup index of all game name in alphabetical order
   SortGames(Index);

   // find initial position in index
   if (InitialSelPos != -1)
   {
      do
      {
         if (Index[i] == InitialSelPos)
            TopIndex = i;

         i++;
      }
      while ((TopIndex != InitialSelPos) && (i < NumDrivers));
   }

   // copy image of the screeen
   blit(screen, Background, 0,0, 0,0, 320,200);
   
   // draw load selector box containing current drivers
   rectfill(screen, 78,23,241,176, PAL_BLACK);
   rect(screen, 78,23,241,176, PAL_GREY);

   // now the window at the bottom for the romname  -JERR
   rect(screen, 78, 179, 241, 192, PAL_GREY);
   rectfill(screen, 79,180,240,191, PAL_BLACK);

   textout_centre(screen, font, "Load ROMs", 160, 25, 2);
   DrawSelections(Index, NumDrivers, TopIndex);
      
   // box the first entry for selection
   rect(screen, 79, 34, 240, 43, 2);
   
   // allow selection
   do
   {
       // fill the text... -JERR
      rectfill(screen, 79,180,240,191, PAL_BLACK);

      textout_centre(screen, font, ShortNameFromIndex(Index[TopIndex + Selection],
         romsetname, ELEMENTS(romsetname)), 160, 182, PAL_GREEN);
      
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

      // first check for initial letter of driver name
      if (toupper(key) >= 'A' && toupper(key) <= 'Z') 
      {
	 
	 int i = 0;
	 int Match = 0;
	 while (!Match && (i < NumDrivers))
	 {
	    if (toupper(key) == toupper(Drivers[Index[i]]->GameName[0]))
	    {
	       // found first match
	       Match = 1;

	       // remove old selection
	       rect(screen, 79, 34 + (Selection * 10), 
		  240, 43 + (Selection * 10), 0);

	       TopIndex = i;
	       Selection = 0;
	       
	       // selection is now at the top of the list box
	       DrawSelections(Index, NumDrivers, TopIndex);
	       rect(screen, 79, 34 + (Selection * 10), 
		     240, 43 + (Selection * 10), 2);
	    }
	    
	    // not found try the next index
	    i++;
	 }
      }
      
      switch(key)
      {
      case 0:
      {
	 // cursor key
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
	 case CURSOR_UP:
	    // up
	    if (Selection == 0)
	    {
	       // can we scroll up?
	       if (TopIndex > 0)
	       {
		  TopIndex --;
		  DrawSelections(Index, NumDrivers, TopIndex);
		  rect(screen, 79, 34 + (Selection * 10), 
		     240, 43 + (Selection * 10), 2);
	       }
	    }
	    if (Selection > 0)
	    {
	       // remove old selection
	       rect(screen, 79, 34 + (Selection * 10), 
		  240, 43 + (Selection * 10), 0);
	       Selection --;
	       //draw new
	       rect(screen, 79, 34 + (Selection * 10), 
		  240, 43 + (Selection * 10), 2);
	    }
	    break;  
	    	    
	 case CURSOR_DOWN:
	    // down
	    if (Selection == MAX_SELECTIONS - 1)
	    {
	       // can we scroll down?
	       if ((TopIndex + MAX_SELECTIONS) < NumDrivers)
	       {
		  TopIndex++;

		  DrawSelections(Index, NumDrivers, TopIndex);
		  rect(screen, 79, 34 + (Selection * 10), 
		     240, 43 + (Selection * 10), 2);
	       }
	    }
	    
	    if ((Selection < MAX_SELECTIONS -1) && 
	       (TopIndex + Selection < (NumDrivers -1)))
	    {
	       // remove old selection
	       rect(screen, 79, 34 + (Selection * 10), 
		  240, 43 + (Selection * 10), 0);
	       Selection ++;
	       //draw new
	       rect(screen, 79, 34 + (Selection * 10), 
		  240, 43 + (Selection * 10), 2);
	    }
	    break;  
	    
	 case PAGE_DOWN:
	    if ((TopIndex + Selection + MAX_SELECTIONS) < NumDrivers)
	    {
	       TopIndex += MAX_SELECTIONS;
		  
	       DrawSelections(Index, NumDrivers, TopIndex);
	       rect(screen, 79, 34 + (Selection * 10), 
		     240, 43 + (Selection * 10), 2);
	    }
	    break;

	 case PAGE_UP:
	    if (((TopIndex + Selection) - MAX_SELECTIONS) >= 0)
	    {
	       TopIndex -= MAX_SELECTIONS;
	       if (TopIndex < 0)
	       {
		  // remove old selection
		  rect(screen, 79, 34 + (Selection * 10), 
		     240, 43 + (Selection * 10), 0);

		  Selection += TopIndex; // subtraction - topindex is negative
		  TopIndex = 0;
	       }
		  
	       DrawSelections(Index, NumDrivers, TopIndex);
	       rect(screen, 79, 34 + (Selection * 10), 
		     240, 43 + (Selection * 10), 2);
	    }
	    break;

	 case (F12):
	    screen_snap();
	    break;
	 }
      }
      break;

      case 27: // escape to abort
	 exit = 1;
	 Selection = -1;
	 break;
	 
      case 32: // space
      case 13: // enter
	 // space to select
	 Selection = Index[TopIndex + Selection];
    InitialSelPos = Selection;
    Save_INI();
	 exit = 1;
	 break;
      }
      
   } while(!exit);
   
   
   // restore screen
   blit(Background, screen, 0,0, 0,0, 320,200);
   destroy_bitmap(Background);
   free (Index);

   return Selection;
}

// Copies a file. filename includes relative path
//
// returns 1 for sucess or 0 for failure
int CopyROM(char * from, char * to, long size, int CommandLine)
{
	FILE * fp_from;
	FILE * fp_to;
	char Buffer[1024];
	long tot = 0;
   int i = 0;

	if ((fp_from = fopen(from, "rb")) == NULL)
	{
      if (!CommandLine)
         DisplayError(E_SAVE, "Couldn't open ROM");
		return 0;
	}
	
	if ((fp_to = fopen(to, "wb")) == NULL)
	{
      if (!CommandLine)
         DisplayError(E_SAVE, "Couldn't create ROM");
		return 0;
	}
	
	do
	{
		i = fread(Buffer, 1, sizeof(Buffer), fp_from);

		if (i == 0 && tot != size)
		{
         if (!CommandLine)
            DisplayError(E_SAVE, "Couldn't copy ROM");
			fclose(fp_from);
			fclose(fp_to);
			return 0;
		}

		if (i > 0)
			fwrite(Buffer, 1, i, fp_to);

		tot += i;

	} while (i > 0);

	fclose(fp_from);
	fclose(fp_to);
	return 1;
}


void MakeBackupPath(const char * DirName, char * Buffer, char * ROMPath, int AppendFilename)
{
	char Filename[100];
	
	int i;
	
	strcpy(Buffer, ROMPath);

	// search backwards to extract the filename from the path
	for (i = strlen(Buffer) - 1; i > 0; i --)
	{
		if (Buffer[i] == '\\')
		{
			Buffer[i] = '\0';
			break;
		}
	}

	// copy the filename to temp storage
	if (i == 0)
		strcpy(Filename, Buffer);
	else
		strcpy(Filename, &Buffer[i + 1]);
		
	
	// append the backup path to the current path
	if (i == 0)
   {
		strcpy(Buffer, DirName);
   }
	else
   {
		strcat(Buffer, "\\");
      strcat(Buffer, DirName);
   }

	// if caller also wants filename then append that too
	if (AppendFilename)
	{
		strcat(Buffer, "\\");
		strcat(Buffer, Filename);
	}
}


// To avoid original roms being overwritten a subdirectory called agebak
// is created and a copy of all of the original roms is store there.
// If agebak already exists then nothing is copied to avoid overwriting
// the back ups of the originals.
//
// Returns 1 for success or 0 for failure
int MakeBackups(int nroms, struct GFXROMINFO * pRomInfo)
{
	char ROMPath[100];
	FILE * fp;
	int i;

	MakeBackupPath(BackupDir, ROMPath, pRomInfo[0].ROMName, 1);
	
	// are the roms already backed up? (can we open one for read)
	if ((fp = fopen(ROMPath, "rb")) != NULL)
	{
		fclose(fp);
		return 1;
	}

	// at this point we need to make backups

	// first create directory
	MakeBackupPath(BackupDir, ROMPath, pRomInfo[0].ROMName, 0);
	mkdir(ROMPath, S_IRUSR | S_IWUSR);

	// back up the roms
   for (i = 0; i < nroms; i ++)
   {
		MakeBackupPath(BackupDir, ROMPath, pRomInfo[i].ROMName, 1);
		if (!CopyROM(pRomInfo[i].ROMName, ROMPath, pRomInfo[i].Size, 0))
			return 0;
	}

	return 1;
}


// lists all of the current drivers and the graphics roms required for each driver
void ListRoms(void)
{
    int NumDrivers = CountDrivers();
    int * Index = (int *) malloc(NumDrivers * sizeof(int)); // this is important
    int i,r;

    if (!Index)
    {
	printf("Memory allocation error.\n");
	return;
    }

    SortGames(Index);

    for (i = 0; i < NumDrivers; i ++)
    {
	    printf("%s\n", Drivers[Index[i]]->GameName);
	    for (r = 0; r < Drivers[Index[i]]->NumGfxRoms; r ++)
	    {
		printf("\t%s\n", Drivers[Index[i]]->GfxRoms[r].ROMName);
	    }

	    if (Drivers[Index[i]]->RomData)
	    for (r = 0; r < Drivers[Index[i]]->RomData->numDataRoms; r ++)
	    {
		printf("\t%s\n", Drivers[Index[i]]->RomData->DataRoms[r].ROMName);
	    }

	    printf("\n");
    }

    free(index);
}

void ListHtml(void)
{
    // display all roms a la mame -list
    int NumDrivers = CountDrivers();
    int * Index = (int *) malloc(NumDrivers * sizeof(int)); // this is important
    int i;
    int online = 42;
    char buf[15];

    if (!Index)
    {
	printf("Memory allocation error.\n");
	return;
    }
    
    SortGames(Index);

    printf("<!table generated by \"AGE.EXE -listhtml\">\n");
    printf("\n<table border = 0>\n");
    printf("\n<tr>\n");
    printf("<td colspan=7 bgcolor=\"444444\"><center>AGE currently supports the following games:</center></td>\n</tr>\n");
    for (i = 0; i < NumDrivers; i ++)
    {
	online++;
	if (online > 6)
	{
	    online = 0;
	    printf("</tr>\n<tr>\n");
	}
	printf("<td bgcolor=\"333333\"> <center> &nbsp %s &nbsp </center> </td>\n", 
          ShortNameFromIndex(Index[i], buf, ELEMENTS(buf)));
    }
    printf("</tr>\n<tr><td colspan=7 bgcolor=\"444444\">\n");
    printf("<center>Total ROM sets supported:  %d</center>\n", CountDrivers());
    printf("</td>\n</tr>\n</table>\n");
    
    free(index);
}

void ListSupported(void)
{
    // display all roms a la mame -list
    int NumDrivers = CountDrivers();
    int * Index = (int *) malloc(NumDrivers * sizeof(int)); // this is important
    int i;
    int online = 42;
    char buf[15];

    if (!Index)
    {
	printf("Memory allocation error.\n");
	return;
    }
    
    SortGames(Index);
    
    printf("\nAGE currently supports the following games:\n");
    for (i = 0; i < NumDrivers; i ++)
    {
	online++;
	if (online > 6)
	{
	    online = 0;
	    printf("\n  ");
	}
   printf("%8s  ", ShortNameFromIndex(Index[i], buf, ELEMENTS(buf)));
    }
    printf("\n\nTotal ROM sets supported:  %d\n\n", CountDrivers());
    
    free(index);
}

void ListFull(void)
{
    // display all roms a la mame -list
    int NumDrivers = CountDrivers();
    int * Index = (int *) malloc(NumDrivers * sizeof(int)); // this is important
    int i;
    char buf[15];
    
    if (!Index)
    {
	printf("Memory allocation error.\n");
	return;
    }

    SortGames(Index);
    
    printf("\nAGE currently supports the following games:\n");
    for (i = 0; i < NumDrivers; i ++)
    {
       printf(" %-10s  \"%s\"\n", ShortNameFromIndex(Index[i], buf, ELEMENTS(buf)),
              Drivers[Index[i]]->GameName);
    }
    
    free(index);
}




void SaveDataToFiles(int nroms, struct GFXROMINFO * gri, BYTE ** loadmem)
{
   FILE * fp = NULL;
   int i = 0;
   
   // try and make the back ups
   if (!MakeBackups(nroms, gri))
      return;

   for (i = 0; i < nroms; i ++)
   {
      if ((fp = fopen(gri[i].ROMName, "wb")) == NULL)
      {
         // cant open file for write
         DisplayError(E_SAVE, gri[i].ROMName);
         return;
      }

      if (fwrite((*loadmem+gri[i].LoadAddress), 1, gri[i].Size, fp)
          != gri[i].Size)
      {
         // bad save
         DisplayError(E_SAVE, gri[i].ROMName);
         fclose(fp);
         return;
      }

      fclose(fp);
   }
}



void SaveRomData(void)
{
    // load the data into the internal data space
    struct ROMDATA * rd = Drivers[DriverNumber]->RomData;
    SaveDataToFiles(rd->numDataRoms, rd->DataRoms, &dataroms);
}

void SaveRoms(void)
{
    // load the graphis into the internal data space
    struct GFXROMINFO * pRomInfo = Drivers[DriverNumber]->GfxRoms;
    SaveDataToFiles(Drivers[DriverNumber]->NumGfxRoms, pRomInfo, &ROMSet);
}



void WritePatchChanges(long StartSeq, long SeqLen, FILE * fpModifiedRom, FILE * fpPatchRom)
{
   int i;
         
   // write value of j as 24bit number
   char * p = (char *)&StartSeq;
   fputc(p[2], fpPatchRom);
   fputc(p[1], fpPatchRom);
   fputc(p[0], fpPatchRom);

   // write value of SeqLen as 16bit number
   p = (char *)&SeqLen;
   fputc(p[1], fpPatchRom);
   fputc(p[0], fpPatchRom);

   // rewind fpModifiedRom to start of change
   fseek(fpModifiedRom, StartSeq, SEEK_SET);

   // copy bytes
   for (i = 0; i < SeqLen; i ++)
   {
      BYTE ch = fgetc(fpModifiedRom);
      fputc(ch, fpPatchRom);
   }

   // make fpModifiedRom in the same position as when we entered this function
   fgetc(fpModifiedRom);
}


void DoComparison(char * ROMName, long Size)
{
   char BackupPath[100] = {0};
   char PatchPath[100] = {0};
   FILE * fpModifiedRom = NULL;
   FILE * fpBackupRom = NULL;
   FILE * fpPatchRom = NULL;
   long StartSeq = -1, SeqLen = 0, j;
   int HeaderWritten = 0;

   // open the backup ROM (do this first as it is the most likely to fail)
   MakeBackupPath(BackupDir, BackupPath, ROMName, 1);
   if ((fpBackupRom = fopen(BackupPath, "rb")) == NULL)
   {
      // just be quiet about this as it only means that the user hasn't edited
      // either the maps or the graphics therefore there is no backup for them
      // and no patch needed anyway
      return;
   }

   // now open the modified rom
   if ((fpModifiedRom = fopen(ROMName, "rb")) == NULL)
   {
      printf("ERROR: Failed to open modified rom \"%s\"\n skipping", ROMName);
      fclose(fpBackupRom);
      return;
   }

   // on to the comparison and creating the IPS file
   // Compare Roms
   StartSeq = -1;

   for (j = 0; j < Size; j ++)
   {
      BYTE BackupCh = fgetc(fpBackupRom);
      BYTE ModifiedCh = fgetc(fpModifiedRom);

      // do bytes match?
      if (BackupCh != ModifiedCh)
      {
         // set up our start flag if it is not already done
         if (StartSeq == -1)
         {
            StartSeq = j;
            SeqLen = 0;
            if (!HeaderWritten)
            {
               // create patch file
               MakeBackupPath(PatchDir, PatchPath,ROMName, 1);
               if ((fpPatchRom = fopen(PatchPath, "wb")) == NULL)
               {
                  // cant open file for write
                  printf("ERROR: Failed to create patch file: \"%s\" skipping\n", PatchPath);
                  fclose(fpBackupRom);
                  fclose(fpModifiedRom);
                  return;
               }

               fwrite("PATCH", 1, 5, fpPatchRom);
               HeaderWritten = 1;
            }
         }

         SeqLen ++;
         // don't allow to overflow 2 bytes
         if (SeqLen == 65535)
         {
            WritePatchChanges(StartSeq, SeqLen, fpModifiedRom, fpPatchRom);
            StartSeq = -1;
            SeqLen = 0;
         }
      }
      else
      {
         // bytes match - do we need to write any patch file
         if (StartSeq != -1)
         {
            WritePatchChanges(StartSeq, SeqLen, fpModifiedRom, fpPatchRom);
            StartSeq = -1;
            SeqLen = 0;
         }
      }
   }

   // do we need to write trailer?
   if (HeaderWritten)
   {
      // have we written all of the changes
      if (StartSeq != -1)
      {
         WritePatchChanges(StartSeq, SeqLen, fpModifiedRom, fpPatchRom);
         StartSeq = -1;
         SeqLen = 0;
      }
      fwrite("EOF", 1, 3, fpPatchRom);
      HeaderWritten = 0;
      fclose(fpPatchRom);
   }

   fclose(fpBackupRom);
}



// Make IPS patch file for specified game and put them in the AGEPATCH
// directory. This is only called from Command line so we can use printf
// for errors
void MakePatch(int GameIndex)
{
   char PatchPath[100] = {0};
   int i = 0;
   
	// create patch directory
	MakeBackupPath(PatchDir, PatchPath, Drivers[GameIndex]->GfxRoms[0].ROMName, 0);
	mkdir(PatchPath, S_IRUSR | S_IWUSR);
   
   // first do graphics roms
   for (i = 0; i < Drivers[GameIndex]->NumGfxRoms; i ++)
   {
      DoComparison(Drivers[GameIndex]->GfxRoms[i].ROMName,
                   Drivers[GameIndex]->GfxRoms[i].Size);
   }

   // are there any map roms to create patch files for?
   if (Drivers[GameIndex]->RomData != 0)
   {
      for (i = 0; i < Drivers[GameIndex]->RomData->numDataRoms; i ++)
      {
         DoComparison(Drivers[GameIndex]->RomData->DataRoms[i].ROMName,
                      Drivers[GameIndex]->RomData->DataRoms[i].Size);
      }
   }
}



void MergeFiles(char * ROMName, long size)
{
   char BackupPath[100] = {0};
   char PatchPath[100] = {0};
   FILE * fpNewRom = NULL;
   FILE * fpBackupRom = NULL;
   FILE * fpPatchRom = NULL;
   char Header[] = "PATCH";
   long Offset = 0;
   long NumBytes = 0;
   long i;
   long BackupPos = 0;
   BYTE ch;
   
   // check for rom name in backup directory
   MakeBackupPath(BackupDir, BackupPath,ROMName, 1);
      
   // if not there then put it there
   if ((fpBackupRom = fopen(BackupPath, "rb")) == NULL)
   {
      CopyROM(ROMName, BackupPath, size, 1);
      if ((fpBackupRom = fopen(BackupPath, "rb")) == NULL)
      {
         // something has gone wrong with the backup abort this
         // rom merge.
         return;
      }
   }

   // at this point we should have a valid fpBackupRom

   MakeBackupPath(PatchDir, PatchPath,ROMName, 1);
   if ((fpPatchRom = fopen(PatchPath, "rb")) == NULL)
   {
      // Oh no! there is no patch file for this rom, just copy
      // backup file over original

      fclose(fpBackupRom);
      CopyROM(BackupPath, ROMName, size, 1);
      return;
   }

   // open the new rom file
   if ((fpNewRom = fopen(ROMName, "wb")) == NULL)
   {
      // abort
      fclose(fpBackupRom);
      fclose(fpPatchRom);
      return;
   }

   // check header
   for (i = 0; i < 5; i ++)
   {
      BYTE ch = fgetc(fpPatchRom);
      if (ch != Header[i])
      {
         printf("Invalid Patch file \"%s\"", PatchPath);
         return;
      }
   } 

   // right thats all of the tinkering about done now for the merge
   for(;;)
   {
      Offset = 0;
      
      // get the next 3 bytes for the offset
      for (i = 0; i < 3; i ++)
      {
         ch = fgetc(fpPatchRom);
         Offset *= 256;
         Offset += ch;
      }

      // end of file?
      if (Offset == ('E' * 65536) + ('O' * 256) + 'F')
         break;

      // now copy all of the bytes from the Backup to the new roms
      // up until the offset

      fseek(fpBackupRom, BackupPos, SEEK_SET);
      for (i = BackupPos; i < Offset; i ++)
      {
         ch = fgetc(fpBackupRom);
         fputc(ch, fpNewRom);
      }

      // how many bytes to merge?
      ch = fgetc(fpPatchRom);
      NumBytes = (256 * ch) + fgetc(fpPatchRom);

      for (i = 0; i < NumBytes; i ++)
      {
         ch = fgetc(fpPatchRom);
         fputc(ch, fpNewRom);
      }
   
      BackupPos = Offset + NumBytes;
   }

   // now copy any remaining bytes from the backup file
   fseek(fpBackupRom, BackupPos, SEEK_SET);
   for (i = BackupPos; i < size; i ++)
   {
      ch = fgetc(fpBackupRom);
      fputc(ch, fpNewRom);
   }
   
   fclose(fpNewRom);
   fclose(fpBackupRom);
   fclose(fpPatchRom);
}


// Patch IPS file with original roms
void ApplyPatch(int GameIndex)
{
   int i = 0;
      
   // first do graphics roms
   for (i = 0; i < Drivers[GameIndex]->NumGfxRoms; i ++)
   {
      MergeFiles(Drivers[GameIndex]->GfxRoms[i].ROMName,
                 Drivers[GameIndex]->GfxRoms[i].Size);
   }

   // are there any map roms to create patch files for?
   if (Drivers[GameIndex]->RomData != 0)
   {
      for (i = 0; i < Drivers[GameIndex]->RomData->numDataRoms; i ++)
      {
         MergeFiles(Drivers[GameIndex]->RomData->DataRoms[i].ROMName,
                    Drivers[GameIndex]->RomData->DataRoms[i].Size);
      }
   }
}


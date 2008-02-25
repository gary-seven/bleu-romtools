#include <stdio.h>
#include <stdlib.h>
#include <conio.h>
#include <ctype.h>
#include <string.h>
#include <stdarg.h>
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

/* initialised properly from ini file */
char ROMPath[100] = {0};
char InitGameName[100] = {0};

// when the specified rom is located its path is stored here.
char ROMPathFound[100] = {0};

/* store the name of the driver INI file */
static char INIDriverName[50];
static char ROMDirName[10];

// used for storing a sorted list of drivers from the
// drivers directory
#define MAXNAME (21) // 20 + null
struct INIFileInfo {
   char FullName [MAXNAME];
   char Filename [MAXNAME];
};

struct INIFileInfo * AllDrivers = NULL;
static int TotalDrivers = 0;
static int * GameIndex = NULL;

void DriverCallback(char * Name, int Attrib, int Param)
{
   char * pFullName = NULL;

   // first check validity of ini file by getting full game name
   push_config_state();
   set_config_file(Name);
   pFullName = get_config_string("General", "Description", "None");

   if (strcmp(pFullName, "None") == 0) // bad ini file - ignore
   {
      pop_config_state();
      return;
   }
   
   // make more storage
   AllDrivers = realloc(AllDrivers, (TotalDrivers + 1) * sizeof(struct INIFileInfo));
   if (AllDrivers == NULL)
   {
      pop_config_state();
      return; // not much we can do
   }

   strncpy (AllDrivers[TotalDrivers].Filename, get_filename(Name), MAXNAME -1);
   AllDrivers[TotalDrivers].Filename[MAXNAME-1] = 0;
   strncpy (AllDrivers[TotalDrivers].FullName, pFullName, MAXNAME -1);
   AllDrivers[TotalDrivers].FullName[MAXNAME-1] = 0;

   TotalDrivers ++;
   pop_config_state();
}


/* sort the games in to alphabetical order for the sake of output */
void SortArray(void)
{
	int i, j, k;

	/* first fill array with current order */
	for (i = 0; i < TotalDrivers; i ++)
	{
		GameIndex[i] = i;
	}

	i--;

	for (k = 0; k < i; k++)
	{
		for (j = 0; j < i - k; j++)
		{
			if (strcmp(AllDrivers[GameIndex[j]].FullName, 
					   AllDrivers[GameIndex[j+1]].FullName) > 0)
			{
				/* swap */
				int tmp = GameIndex[j];
				GameIndex[j] = GameIndex[j+1];
				GameIndex[j+1] = tmp;
			}
		}
	}
}


// Call this function wheneven a list of sorted drivers is
// required i.e. the -list functions or the load selector.
// If the function has already been called then it will
// return straight away
void BuildDriverList(void)
{
   if (AllDrivers != NULL)
      return;

   // fill array
   for_each_file("drivers/*.ini", 0, DriverCallback, 0);

   // sort array
   GameIndex = (int *) malloc(TotalDrivers * sizeof(int));
   if (GameIndex == NULL)
   {
      DisplayError("Memory allocation error");
      return;
   }

   SortArray();
}


void DisplayError(const char * pFormatStr, ...)
{
   char     FormatStrBuffer[100] = { 0 };
   va_list  msgArgs;

   /* Convert the parameters to an actual message and write it into
    * FormatStrBuffer. */
   va_start (msgArgs, pFormatStr);
   vsprintf (FormatStrBuffer, pFormatStr, msgArgs);
   va_end (msgArgs);

   // Is this message before the user interface has started up?
   if (ErrorMode == COMMAND_MODE)
   {
      printf("%s\n", FormatStrBuffer);
   }
   else
   {
      Dialog_Box("Error", FormatStrBuffer);
   }
}


// returns the first part of a filename (no extension)
char * GetShortName(char * Buffer, char * Filename)
{
   int i = 0;
   
   // fill buffer and ensure there is no path info
   strcpy(Buffer, get_filename(Filename));

   // scan buffer to wipe out extension (splat '.' with null)
   for(;;)
   {
      if (Buffer[i] == 0) // end of buffer
         break;

      if (Buffer[i] == '.') // splat extension
      {
         Buffer[i] = 0;
         break;
      }

      i++;
   }

   return Buffer;
}



long DetermineDataSize(int nroms, struct GFXROMINFO * gri)
{
    long totalsize = 0l;
    int romc;
    /* first figure out how much ram is needed */
    for (romc = 0; romc < nroms ; romc++)
    {
		totalsize += gri[romc].Size;
    }
    return totalsize;
}



int ScanROMPath(char * Path)
{
   int i = 0;

   // no path?
   if (Path[0] == '\0')
      return -1;

   // find end position
   while ((Path[i] != '\0') && (Path[i] != ';'))
      i ++;
   
   return i;
}

int FindRomPath(char * ROMName)
{
   int Terminator = 0;
   int Pos = 0;
   char tmpFullPath[100];
   
   for (;;)
   {
      Terminator = ScanROMPath(&ROMPath[Pos]);
      if (Terminator == -1)
         return 1;

      strncpy(tmpFullPath, &ROMPath[Pos], Terminator);
      tmpFullPath[Terminator] = 0;
      put_backslash(tmpFullPath);
      strcat(tmpFullPath, ROMName);

      // found it - return success
      if (file_exists(tmpFullPath, 0, NULL) != 0)
      {
         strncpy(ROMPathFound, &ROMPath[Pos], Terminator);
         ROMPathFound[Terminator] = 0;
         return 0;
      }
         
      if (ROMPath[Pos + Terminator] == ';')
         Terminator ++;

      Pos += Terminator;
   }
}



// load the specfied roms
// returns 0 for failure or non-zero for success
int LoadDataFromFiles(int nroms, struct GFXROMINFO * gri, BYTE ** loadmem)
{
   int romc;
   FILE * fp;
   char ROMDirWithName[50];
   
   /* malloc the ram */
   if (*loadmem)
   {
      free(*loadmem);
      *loadmem = NULL;
   }
   *loadmem = (char *)malloc(DetermineDataSize(nroms, gri));
   if (*loadmem == NULL)
      return 0;

   // can we find the roms?
   sprintf(ROMDirWithName, "%s/%s", ROMDirName, gri[0].ROMName);
   if (FindRomPath(ROMDirWithName) != 0)
   {
      DisplayError("ROM (%s) not found on ROMPath (%s)",
                   gri[0].ROMName, ROMPath);
      return 0;
   }
   
   /* load the data into the dataroms */
   for (romc = 0; romc < nroms ; romc++)
   {
      char FullPath[120];

      // build full path including filename
      strcpy(FullPath, ROMPathFound);
      put_backslash(FullPath);
      strcat(FullPath, ROMDirName);
      put_backslash(FullPath);
      strcat(FullPath, gri[romc].ROMName);

      if ((fp = fopen(FullPath, "rb")) == NULL)
      {
         /* file not found */
         DisplayError("File not found");
         return 0;
      }

      if (fread((*loadmem+gri[romc].LoadAddress), 1,
                gri[romc].Size, fp) != gri[romc].Size)
      {
         /* bad load */
         DisplayError("Load error");
         fclose(fp);
         return 0;
      }

      fclose(fp);
   }
   return 1;
}


// returns 0 for failure or non-zero for success
int LoadRomData(void)
{
    /* load the data into the internal data space */
    return LoadDataFromFiles(Driver.numDataRoms, Driver.DataRoms, &dataroms);
}

// returns 0 for failure or non-zero for success
int LoadRoms(void)
{
    /* load the graphics into the internal data space */
    return LoadDataFromFiles(Driver.NumGfxRoms, Driver.GfxRoms, &ROMSet);
}



void DrawSelections(int TopIndex)
{
	int i = TopIndex;
	int pos = 0;

	/* clear list first */
	rectfill(screen, 80,35,239,174, 0);

	/* draw new list */
	while (i < TotalDrivers && i < (MAX_SELECTIONS + TopIndex))
	{
		textout(screen, font, AllDrivers[GameIndex[i]].FullName, 80, 35 + (pos * 10), 1);      
		i++;
		pos++;
	}
}


// allows selection of a game including loading it and
// returns 1 for success or 0 for failure
int LoadSelector(void)
{
	BITMAP * Background = create_bitmap(320,200);
	int Selection = 0;
	int key, exit = 0;
	int MouseDir = 0, MouseButt = 0;
	int TopIndex = 0;

	/* make a lookup index of all game name in alphabetical order */
	BuildDriverList();

	/* find initial position in index */
	if (strcmp(InitGameName, "none") != 0)
	{
      int Exit = 0;
      int i = 0;
      
		do
		{
         char buf[15] = {0};
         
			if (strcmp(GetShortName(buf, AllDrivers[GameIndex[i]].Filename), InitGameName) == 0)
         {
				TopIndex = i;
            Exit = 1;
         }

			i++;
		}
		while (!Exit && i < TotalDrivers);
	}
   
	/* copy image of the screeen */
	blit(screen, Background, 0,0, 0,0, 320,200);

	/* draw load selector box containing current drivers */
	rectfill(screen, 78,23,241,176, PAL_BLACK);
	rect(screen, 78,23,241,176, PAL_GREY);

	/* now the window at the bottom for the romname  -JERR */
	rect(screen, 78, 179, 241, 192, PAL_GREY);
	rectfill(screen, 79,180,240,191, PAL_BLACK);

	textout_centre(screen, font, "Load ROMs", 160, 25, 2);
	DrawSelections(TopIndex);

	/* box the first entry for selection */
	rect(screen, 79, 34, 240, 43, 2);

	/* allow selection */
	do
	{
		rectfill(screen, 79,180,240,191, PAL_BLACK);

		textout_centre(screen, font, AllDrivers[GameIndex[TopIndex + Selection]].Filename,
                     160, 182, PAL_GREEN);

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

		/* first check for initial letter of driver name */
		if (isalnum(key))
		{

			int i = 0;
			int Match = 0;
			while (!Match && (i < TotalDrivers))
			{
				if (toupper(key) == toupper(AllDrivers[GameIndex[i]].FullName[0]))
				{
					/* found first match */
					Match = 1;

					/* remove old selection */
					rect(screen, 79, 34 + (Selection * 10), 
						 240, 43 + (Selection * 10), 0);

					TopIndex = i;
					Selection = 0;

					/* selection is now at the top of the list box */
					DrawSelections(TopIndex);
					rect(screen, 79, 34 + (Selection * 10), 
						 240, 43 + (Selection * 10), 2);
				}

				/* not found try the next index */
				i++;
			}
		}

		switch(key)
		{
			case 0:
			{
				/* cursor key */
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
						/* up */
						if (Selection == 0)
						{
							/* can we scroll up? */
							if (TopIndex > 0)
							{
								TopIndex --;
								DrawSelections(TopIndex);
								rect(screen, 79, 34 + (Selection * 10), 
									 240, 43 + (Selection * 10), 2);
							}
						}
						if (Selection > 0)
						{
							/* remove old selection */
							rect(screen, 79, 34 + (Selection * 10), 
								 240, 43 + (Selection * 10), 0);
							Selection --;
							/*draw new */
							rect(screen, 79, 34 + (Selection * 10), 
								 240, 43 + (Selection * 10), 2);
						}
						break;  

					case CURSOR_DOWN:
						/* down */
						if (Selection == MAX_SELECTIONS - 1)
						{
							/* can we scroll down? */
							if ((TopIndex + MAX_SELECTIONS) < TotalDrivers)
							{
								TopIndex++;

								DrawSelections(TopIndex);
								rect(screen, 79, 34 + (Selection * 10), 
									 240, 43 + (Selection * 10), 2);
							}
						}

						if ((Selection < MAX_SELECTIONS -1) && 
							(TopIndex + Selection < (TotalDrivers -1)))
						{
							/* remove old selection */
							rect(screen, 79, 34 + (Selection * 10), 
								 240, 43 + (Selection * 10), 0);
							Selection ++;
							/*draw new */
							rect(screen, 79, 34 + (Selection * 10), 
								 240, 43 + (Selection * 10), 2);
						}
						break;  

					case PAGE_DOWN:
						if ((TopIndex + Selection + MAX_SELECTIONS) < TotalDrivers)
						{
							TopIndex += MAX_SELECTIONS;

							DrawSelections(TopIndex);
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
								/* remove old selection */
								rect(screen, 79, 34 + (Selection * 10), 
									 240, 43 + (Selection * 10), 0);

								Selection += TopIndex; /* subtraction - topindex is negative */
								TopIndex = 0;
							}

							DrawSelections(TopIndex);
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

			case 27: /* escape to abort */
				exit = 1;
				Selection = -1;
				break;

			case 32: /* space */
			case 13: /* enter */
         {
            char buf[15] = {0};
				/* space to select */
				Selection = GameIndex[TopIndex + Selection];
            strcpy(InitGameName, GetShortName(buf, AllDrivers[Selection].Filename));
				Save_INI();
				exit = 1;
				break;
         }
		}

	} while(!exit);

	/* restore screen */
	blit(Background, screen, 0,0, 0,0, 320,200);
	destroy_bitmap(Background);

   // any selected?
   if (Selection == -1)
      return 0;

   // attempt to load specified romset
   return ReadFromDriverIniFile(AllDrivers[Selection].Filename);
}


/* Copies a file. filename includes relative path */
/* */
/* returns 1 for sucess or 0 for failure */
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
			DisplayError("Couldn't open ROM for backup");
		return 0;
	}

	if ((fp_to = fopen(to, "wb")) == NULL)
	{
		if (!CommandLine)
			DisplayError("Couldn't create backup");
		return 0;
	}

	do
	{
		i = fread(Buffer, 1, sizeof(Buffer), fp_from);

		if (i == 0 && tot != size)
		{
			if (!CommandLine)
				DisplayError("Couldn't copy ROM");
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


void MakeBackupPath(const char * DirName, char * Buffer, char * Filename, int AppendFilename)
{
   strcpy(Buffer, ROMPathFound); // add the full rom path
   put_backslash(Buffer);
   strcat(Buffer, ROMDirName); // now add the directory name
   put_backslash(Buffer);
   strcat(Buffer, DirName); // now add agebak/agepatch etc.
   
	/* if caller also wants filename then append that too */
	if (AppendFilename)
	{
      put_backslash(Buffer);
		strcat(Buffer, Filename);
	}
}


/* To avoid original roms being overwritten a subdirectory called agebak */
/* is created and a copy of all of the original roms is store there. */
/* If agebak already exists then nothing is copied to avoid overwriting */
/* the back ups of the originals. */
/* */
/* Returns 1 for success or 0 for failure */
int MakeBackups(int nroms, struct GFXROMINFO * pRomInfo)
{
	char BackupPath[100];
   char FromPath[100];
	FILE * fp;
	int i;

	MakeBackupPath(BackupDir, BackupPath, pRomInfo[0].ROMName, 1);

	/* are the roms already backed up? (can we open one for read) */
	if ((fp = fopen(BackupPath, "rb")) != NULL)
	{
		fclose(fp);
		return 1;
	}

	/* at this point we need to make backups */

	/* first create directory */
	MakeBackupPath(BackupDir, BackupPath, pRomInfo[0].ROMName, 0);
	mkdir(BackupPath, S_IRUSR | S_IWUSR);

	/* back up the roms */
	for (i = 0; i < nroms; i ++)
	{
      strcpy(FromPath, ROMPathFound);
      put_backslash(FromPath);
      strcat(FromPath, ROMDirName);
      put_backslash(FromPath);
      strcat(FromPath, pRomInfo[i].ROMName);

		MakeBackupPath(BackupDir, BackupPath, pRomInfo[i].ROMName, 1);
		if (!CopyROM(FromPath, BackupPath, pRomInfo[i].Size, 0))
			return 0;
	}

	return 1;
}

/* lists all of the current drivers and the graphics roms required for each driver */
void ListRoms(void)
{
   int i, j;
   BuildDriverList();

   for (i = 0; i < TotalDrivers; i ++)
   {
      char DriverPath[30];
      sprintf(DriverPath, "drivers/%s", AllDrivers[GameIndex[i]].Filename);
      
      printf("%s\n", AllDrivers[GameIndex[i]].FullName);

      push_config_state();
      set_config_file(DriverPath);
      j = 1;
      for (;;)
      {
         char RomHeading[10];
         char * pRomName = NULL;
         
         // extract info from the ini files
         sprintf(RomHeading, "Rom%d", j);
         pRomName = get_config_string(RomHeading, "name", "None");

         // end of list?
         if (strcmp(pRomName, "None") == 0)
            break;

         printf("\t%s\n", pRomName);
         j++;
      }

      pop_config_state();
      printf("\n");
   }
}


void ListHtml(void)
{
   /* display all roms a la mame -list */
   int i;
   int online = 42;
   char buf[15];
   
   BuildDriverList();

   printf("<!table generated by \"AGE.EXE -listhtml\">\n");
   printf("\n<table border = 0>\n");
   printf("\n<tr>\n");
   printf("<td colspan=7 bgcolor=\"444444\"><center>AGE currently supports the following games:</center></td>\n</tr>\n");
   for (i = 0; i < TotalDrivers; i ++)
   {
      online++;
      if (online > 6)
      {
         online = 0;
         printf("</tr>\n<tr>\n");
      }

      printf("<td bgcolor=\"333333\"> <center> &nbsp %s &nbsp </center> </td>\n",
            GetShortName(buf, AllDrivers[GameIndex[i]].Filename));
   }
   printf("</tr>\n<tr><td colspan=7 bgcolor=\"444444\">\n");
   printf("<center>Total ROM sets supported:  %d</center>\n", TotalDrivers);
   printf("</td>\n</tr>\n</table>\n");

}


void ListSupported(void)
{
   int i;
   int online = 42;
   char buf[15];
   
   BuildDriverList();

   printf("\nAGE currently supports the following games:\n");
   for (i = 0; i < TotalDrivers; i ++)
   {
      online++;
      if (online > 6)
      {
         online = 0;
         printf("\n  ");
      }
      
      printf("%8s  ", GetShortName(buf, AllDrivers[GameIndex[i]].Filename));

   }
   printf("\n\nTotal ROM sets supported:  %d\n\n", TotalDrivers);
}


void ListFull(void)
{
   int i;
   char buf[15];
   
   BuildDriverList();

   printf("\nAGE currently supports the following games:\n");
   for (i = 0; i < TotalDrivers; i ++)
   {
      printf(" %-10s  \"%s\"\n", GetShortName(buf, AllDrivers[GameIndex[i]].Filename),
             AllDrivers[GameIndex[i]].FullName);
   }
}


// Saves the romset out to disk. Attempts to backup originals if
// a backup doesn't already exist.
// return 1 for success or 0 for failure
int SaveDataToFiles(int nroms, struct GFXROMINFO * gri, BYTE ** loadmem)
{
	FILE * fp = NULL;
	int i = 0;
   char FullPath[100] = {0};
   
	/* try and make the back ups */
	if (!MakeBackups(nroms, gri))
		return 0;

	for (i = 0; i < nroms; i ++)
	{
      strcpy(FullPath, ROMPathFound);
      put_backslash(FullPath);
      strcat(FullPath, ROMDirName);
      put_backslash(FullPath);
      strcat(FullPath, gri[i].ROMName);

		if ((fp = fopen(FullPath, "wb")) == NULL)
		{
			/* cant open file for write */
			DisplayError("Failed to open for write");
			return 0;
		}

		if (fwrite((*loadmem+gri[i].LoadAddress), 1, gri[i].Size, fp)
			!= gri[i].Size)
		{
			/* bad save */
			DisplayError("Failed to save");
			fclose(fp);
			return 0;
		}

		fclose(fp);
	}

   return 1;
}


void SaveRomData(void)
{
    SaveDataToFiles(Driver.numDataRoms, Driver.DataRoms, &dataroms);
}


// returns 1 for a successful rom save or 0 for failure
int SaveRoms(void)
{
    return SaveDataToFiles(Driver.NumGfxRoms, Driver.GfxRoms, &ROMSet);
}


void WritePatchChanges(long StartSeq, long SeqLen, FILE * fpModifiedRom, FILE * fpPatchRom)
{
	int i;

	/* write value of j as 24bit number */
	char * p = (char *)&StartSeq;
	fputc(p[2], fpPatchRom);
	fputc(p[1], fpPatchRom);
	fputc(p[0], fpPatchRom);

	/* write value of SeqLen as 16bit number */
	p = (char *)&SeqLen;
	fputc(p[1], fpPatchRom);
	fputc(p[0], fpPatchRom);

	/* rewind fpModifiedRom to start of change */
	fseek(fpModifiedRom, StartSeq, SEEK_SET);

	/* copy bytes */
	for (i = 0; i < SeqLen; i ++)
	{
		BYTE ch = fgetc(fpModifiedRom);
		fputc(ch, fpPatchRom);
	}

	/* make fpModifiedRom in the same position as when we entered this function */
	fgetc(fpModifiedRom);
}


void DoComparison(char * ROMName, long Size)
{
	char BackupPath[100] = {0};
	char PatchPath[100] = {0};
   char FullPath[100] = {0};
	FILE * fpModifiedRom = NULL;
	FILE * fpBackupRom = NULL;
	FILE * fpPatchRom = NULL;
	long StartSeq = -1, SeqLen = 0, j;
	int HeaderWritten = 0;

	/* open the backup ROM (do this first as it is the most likely to fail) */
	MakeBackupPath(BackupDir, BackupPath, ROMName, 1);
	if ((fpBackupRom = fopen(BackupPath, "rb")) == NULL)
	{
		/* just be quiet about this as it only means that the user hasn't edited */
		/* either the maps or the graphics therefore there is no backup for them */
		/* and no patch needed anyway */
		return;
	}

	/* now open the modified rom */
   strcpy(FullPath, ROMPathFound);
   put_backslash(FullPath);
   strcat(FullPath, ROMDirName);
   put_backslash(FullPath);
   strcat(FullPath, ROMName);

	if ((fpModifiedRom = fopen(FullPath, "rb")) == NULL)
	{
		printf("ERROR: Failed to open modified rom \"%s\"\n skipping", ROMName);
		fclose(fpBackupRom);
		return;
	}

	/* on to the comparison and creating the IPS file */
	/* Compare Roms */
	StartSeq = -1;

	for (j = 0; j < Size; j ++)
	{
		BYTE BackupCh = fgetc(fpBackupRom);
		BYTE ModifiedCh = fgetc(fpModifiedRom);

		/* do bytes match? */
		if (BackupCh != ModifiedCh)
		{
			/* set up our start flag if it is not already done */
			if (StartSeq == -1)
			{
				StartSeq = j;
				SeqLen = 0;
				if (!HeaderWritten)
				{
					/* create patch file */
					MakeBackupPath(PatchDir, PatchPath,ROMName, 1);
					if ((fpPatchRom = fopen(PatchPath, "wb")) == NULL)
					{
						/* cant open file for write */
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
			/* don't allow to overflow 2 bytes */
			if (SeqLen == 65535)
			{
				WritePatchChanges(StartSeq, SeqLen, fpModifiedRom, fpPatchRom);
				StartSeq = -1;
				SeqLen = 0;
			}
		}
		else
		{
			/* bytes match - do we need to write any patch file */
			if (StartSeq != -1)
			{
				WritePatchChanges(StartSeq, SeqLen, fpModifiedRom, fpPatchRom);
				StartSeq = -1;
				SeqLen = 0;
			}
		}
	}

	/* do we need to write trailer? */
	if (HeaderWritten)
	{
		/* have we written all of the changes */
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


/* Make IPS patch file for specified game and put them in the AGEPATCH */
/* directory. This is only called from Command line so we can use printf */
/* for errors */
void MakePatch(void)
{
	char PatchPath[100] = {0};
	int i = 0;

	/* create patch directory */
	MakeBackupPath(PatchDir, PatchPath, Driver.GfxRoms[0].ROMName, 0);
	mkdir(PatchPath, S_IRUSR | S_IWUSR);

	/* first do graphics roms */
	for (i = 0; i < Driver.NumGfxRoms; i ++)
	{
      // build full path including filename
		DoComparison(Driver.GfxRoms[i].ROMName,
					 Driver.GfxRoms[i].Size);
	}

	/* are there any map roms to create patch files for? */
	if (Driver.numDataRoms != 0)
	{
		for (i = 0; i < Driver.numDataRoms; i ++)
		{
			DoComparison(Driver.DataRoms[i].ROMName,
				     Driver.DataRoms[i].Size);
		}
	}
}


void MergeFiles(char * ROMName, long size)
{
	char BackupPath[100] = {0};
	char PatchPath[100] = {0};
   char FullPath[100] = {0};
	FILE * fpNewRom = NULL;
	FILE * fpBackupRom = NULL;
	FILE * fpPatchRom = NULL;
	char Header[] = "PATCH";
	long Offset = 0;
	long NumBytes = 0;
	long i;
	long BackupPos = 0;
	BYTE ch;

	/* check for rom name in backup directory */
	MakeBackupPath(BackupDir, BackupPath,ROMName, 1);

   // make full rom path
   strcpy(FullPath, ROMPathFound);
   put_backslash(FullPath);
   strcat(FullPath, ROMDirName);
   put_backslash(FullPath);
   strcat(FullPath, ROMName);
   
	/* if not there then put it there */
	if ((fpBackupRom = fopen(BackupPath, "rb")) == NULL)
	{
		CopyROM(FullPath, BackupPath, size, 1);
		if ((fpBackupRom = fopen(BackupPath, "rb")) == NULL)
		{
			/* something has gone wrong with the backup abort this */
			/* rom merge. */
			return;
		}
	}

	/* at this point we should have a valid fpBackupRom */

	MakeBackupPath(PatchDir, PatchPath,ROMName, 1);
	if ((fpPatchRom = fopen(PatchPath, "rb")) == NULL)
	{
		/* Oh no! there is no patch file for this rom, just copy */
		/* backup file over original */

		fclose(fpBackupRom);
		CopyROM(BackupPath, ROMName, size, 1);
		return;
	}

	/* open the new rom file */
	if ((fpNewRom = fopen(FullPath, "wb")) == NULL)
	{
		/* abort */
		fclose(fpBackupRom);
		fclose(fpPatchRom);
		return;
	}

	/* check header */
	for (i = 0; i < 5; i ++)
	{
		BYTE ch = fgetc(fpPatchRom);
		if (ch != Header[i])
		{
			printf("Invalid Patch file \"%s\"", PatchPath);
			return;
		}
	} 

	/* right thats all of the tinkering about done now for the merge */
	for(;;)
	{
		Offset = 0;

		/* get the next 3 bytes for the offset */
		for (i = 0; i < 3; i ++)
		{
			ch = fgetc(fpPatchRom);
			Offset *= 256;
			Offset += ch;
		}

		/* end of file? */
		if (Offset == ('E' * 65536) + ('O' * 256) + 'F')
			break;

		/* now copy all of the bytes from the Backup to the new roms */
		/* up until the offset */

		fseek(fpBackupRom, BackupPos, SEEK_SET);
		for (i = BackupPos; i < Offset; i ++)
		{
			ch = fgetc(fpBackupRom);
			fputc(ch, fpNewRom);
		}

		/* how many bytes to merge? */
		ch = fgetc(fpPatchRom);
		NumBytes = (256 * ch) + fgetc(fpPatchRom);

		for (i = 0; i < NumBytes; i ++)
		{
			ch = fgetc(fpPatchRom);
			fputc(ch, fpNewRom);
		}

		BackupPos = Offset + NumBytes;
	}

	/* now copy any remaining bytes from the backup file */
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


/* Patch IPS file with original roms */
void ApplyPatch(void)
{
	int i = 0;

	/* first do graphics roms */
	for (i = 0; i < Driver.NumGfxRoms; i ++)
	{
		MergeFiles(Driver.GfxRoms[i].ROMName,
				   Driver.GfxRoms[i].Size);
	}

	/* are there any map roms to create patch files for? */
	if (Driver.numDataRoms != 0) // JERRY -- CHANGED
	{
		for (i = 0; i < Driver.numDataRoms; i ++)
		{
			MergeFiles(Driver.DataRoms[i].ROMName,
				   Driver.DataRoms[i].Size);
		}
	}
}


// Pass in a buffer of the form:
// "12,    34, 56"
// and the return value is an index to the start of the next
// alphanumeric char i.e. in the above case the return value is 7.
int NextNumber(char * p)
{
   int i = 0;

   // remove any leading white space
   while (p[i] != '\0' && !isalnum(p[i]))
      i++;

   // find the first non alphanumeric character
   while (p[i] != '\0' && isalnum(p[i]))
      i++;

   // remove any trailing white space
   while (p[i] != '\0' && !isalnum(p[i]))
      i++;

   return i;
}

/* default palette - used if one doesn't exist in the ini driver */
static RGB DefaultColours[] =
{
	{0x00,0x00,0x00}, {0x3f,0x00,0x00}, {0x00,0x3f,0x00}, {0x00,0x00,0x3f},
	{0x3f,0x3f,0x00}, {0x3f,0x00,0x3f}, {0x00,0x3f,0x3f}, {0x3f,0x3f,0x3f},
	{0x08,0x08,0x08}, {0x10,0x10,0x10}, {0x18,0x18,0x18}, {0x20,0x20,0x20},
	{0x28,0x28,0x28}, {0x30,0x30,0x30}, {0x38,0x38,0x38}, {0x3f,0x3f,0x3f}
};
static struct PALETTE DefaultPalette = { 16, DefaultColours };


// Read from a specified ini file
// returns 0 for failure or non-zero for success
static int RealReadFromDriverIniFile(char *filename);

// this is a wrapper so that the ini file stuff can be pushed and poped easily
int ReadFromDriverIniFile(char *filename)
{
   int retval = 0;
   int i;
   char * fname = get_filename(filename);
   
   /* setup INI Driver name */
   sprintf(INIDriverName, "drivers/%s", fname);

   /* set up ROMDirName - copy name with the extension */
   i = 0;
   while ((fname[i] != '\0') && (fname[i] != '.'))
   {
      ROMDirName[i] = fname[i];
      i ++;
   }
   ROMDirName[i] = 0;
   
   push_config_state();
   set_config_file(INIDriverName);

   retval = RealReadFromDriverIniFile(filename);
   
   pop_config_state();

   return retval;
}


static int LoadPaletteFromBuffer(char * pBuffer, int NumCols, int k, int Index);

// returns an index given the number of colours - I should think there is a better
// way of doing this but I didn't have time to think of one
static int IndexFromNumCols(int NumCols)
{
   switch(NumCols)
   {
   case 32:
      return 4;
   case 16:
      return 3;
   case 8:
      return 2;
   case 4:
      return 1;
   case 2:
   default:
      return 0;
   }
}


// load in the data rom info...
void ReadDataRomInfo(void)
{
    char tsect[255];
    char *temp;
    char **argv;
    int argc;

    Driver.numDataRoms = 0;
    Driver.DataRoms = NULL;
    temp = tsect;

    do {
        sprintf(tsect, "Rom%d", Driver.numDataRoms+1);
	temp = get_config_string("DataRoms", tsect, "None");
	if (strcmp(temp, "None"))
	{
	    // there was a rom, parse the information for it...
	    Driver.DataRoms = (struct GFXROMINFO *)realloc(Driver.DataRoms, 
		    (Driver.numDataRoms + 1) * sizeof(struct GFXROMINFO));
	    if (Driver.DataRoms == NULL)
	    	break;

	    argv = get_config_argv("DataRoms", tsect, &argc);

	    Driver.DataRoms[Driver.numDataRoms].LoadAddress = atol(argv[0]);
	    Driver.DataRoms[Driver.numDataRoms].Size = atol(argv[1]);
	    Driver.DataRoms[Driver.numDataRoms].ROMName = 
	    		(char *)malloc (strlen(argv[2])+1);
	    strcpy(Driver.DataRoms[Driver.numDataRoms].ROMName, argv[2]);

	    Driver.numDataRoms++;
	}
    } while (strcmp(temp, "None"));

    if (Driver.DataRoms == NULL)
    	Driver.numDataRoms = 0;

    // this memory is freed before a new driver is loaded and before
    // AGE exits
}


// load in the graphics rom info...
void ReadGfxRomInfo(void)
{
   char tsect[255];
   char *temp;
   char **argv;
   int argc;

   Driver.NumGfxRoms = 0;
   Driver.GfxRoms = NULL;
   temp = tsect;

   do
   {
      sprintf(tsect, "Rom%d", Driver.NumGfxRoms+1);
      temp = get_config_string("GraphicsRoms", tsect, "None");
      if (strcmp(temp, "None"))
      {
         // there was a rom, parse the information for it...
         Driver.GfxRoms = (struct GFXROMINFO *)realloc(Driver.GfxRoms, 
            (Driver.NumGfxRoms + 1) * sizeof(struct GFXROMINFO));
         if (Driver.GfxRoms == NULL)
            break;

         argv = get_config_argv("GraphicsRoms", tsect, &argc);

         Driver.GfxRoms[Driver.NumGfxRoms].LoadAddress = atol(argv[0]);
         Driver.GfxRoms[Driver.NumGfxRoms].Size = atol(argv[1]);
         Driver.GfxRoms[Driver.NumGfxRoms].ROMName = 
            (char *)malloc (strlen(argv[2])+1);
         strcpy(Driver.GfxRoms[Driver.NumGfxRoms].ROMName, argv[2]);

         Driver.NumGfxRoms++;
      }
   } while (strcmp(temp, "None"));

   if (Driver.GfxRoms == NULL)
      Driver.NumGfxRoms = 0;

    // this memory is freed before a new driver is loaded and before
    // AGE exits
}



static int RealReadFromDriverIniFile(char *filename)
{
   char * pBuffer = NULL;
	int i, j, k;
   int PlanesUsed[MAX_COLOUR_PLANES] = {0};
   
	pBuffer = get_config_string("General", "Description", "None");
	if (strcmp(pBuffer, "None") == 0)
	{
		DisplayError("Can't open .ini file");
		return 0;
	}

   strcpy(Driver.GameName, pBuffer);

	if ((Driver.NumGfxBanks = get_config_int("Layout", "GfxDecodes", 0)) == 0)
	{ DisplayError("INI file error 'GfxDecodes'"); return 0; }

   // if this fail the orientation will be 0 - which is OK.
   Driver.Orientation = get_config_int("Layout", "Orientation", 0);
   
   Driver.GfxInfo = malloc(Driver.NumGfxBanks * sizeof(struct GFXINFO));
	if (Driver.GfxInfo == NULL)
	{ DisplayError("Malloc failed"); return 0; }

	for (i = 0; i < Driver.NumGfxBanks; i++)
	{
		char GfxBank[10];

      sprintf(GfxBank, "Decode%d", i+1);

		if ((Driver.GfxInfo[i].StartAddress = get_config_int(GfxBank, "start", -1)) == -1)
		{ DisplayError("INI file error 'start'"); return 0; }

		Driver.GfxInfo[i].GfxLayout = malloc(sizeof(struct GFXLAYOUT));
		if (Driver.GfxInfo[i].GfxLayout == NULL)
		{ DisplayError("Malloc failed"); return 0; }

		Driver.GfxInfo[i].GfxLayout->width = get_config_int(GfxBank, "width", 0);
		Driver.GfxInfo[i].GfxLayout->height = get_config_int(GfxBank, "height", 0);
		Driver.GfxInfo[i].GfxLayout->total = get_config_int(GfxBank, "total", 0);
		Driver.GfxInfo[i].GfxLayout->planes = get_config_int(GfxBank, "planes", 0);

      // mark the fact that this driver is using a decode method with n planes
      PlanesUsed[Driver.GfxInfo[i].GfxLayout->planes - 1] = 1;
      
		pBuffer = get_config_string(GfxBank, "planeoffsets", "");

      for (j = 0, k = 0; j < Driver.GfxInfo[i].GfxLayout->planes; j++)
		{
         int retval;
			if (sscanf(&pBuffer[k], " %d ", &(Driver.GfxInfo[i].GfxLayout->planeoffset[j])) == -1)
            { DisplayError("INI file error 'planeoffsets'"); return 0; }
         if ((retval = NextNumber(&pBuffer[k])) == 0)
            { DisplayError("INI file error 'planeoffsets'"); return 0; }
         k+=retval;
		}

		pBuffer = get_config_string(GfxBank, "xoffsets", "");
		for (j = 0, k = 0; j < Driver.GfxInfo[i].GfxLayout->width; j++)
		{
         int retval;
			if (sscanf(&pBuffer[k], " %d ", &(Driver.GfxInfo[i].GfxLayout->xoffset[j])) == -1)
            { DisplayError("INI file error 'xoffsets'"); return 0; }
         if ((retval = NextNumber(&pBuffer[k])) == 0)
            { DisplayError("INI file error 'xoffsets'"); return 0; }
         k+=retval;
		}

		pBuffer = get_config_string(GfxBank, "yoffsets", "");
		for (j = 0, k = 0; j < Driver.GfxInfo[i].GfxLayout->height; j++)
		{
         int retval;
			if (sscanf(&pBuffer[k], " %d ", &(Driver.GfxInfo[i].GfxLayout->yoffset[j])) == -1)
            { DisplayError("INI file error 'yoffsets'"); return 0; }
         if ((retval = NextNumber(&pBuffer[k])) == 0)
            { DisplayError("INI file error 'yoffsets'"); return 0; }
         k+=retval;
		}

		Driver.GfxInfo[i].GfxLayout->charincrement = get_config_int(GfxBank, "charincrement", 0);
	}

   ReadGfxRomInfo();
   ReadDataRomInfo();

   // initialise palette number
   ModifyPaletteNumber(0);

   // create enough default palettes. Note that these may get overwritten if
   // real ones exist.
   for (i = 0; i < MAX_COLOUR_PLANES; i ++)
   {
      // only define defaults to planes used.
      if (!PlanesUsed[i])
      {
         // set no palettes for this plane
         Driver.NumPalettes[i] = 0;
         continue;
      }

      // this has to dynamic instead of simply pointing to the default palette so that
      // it can be realloc'ed if needs be
      Driver.NumPalettes[i] = 1;
      Driver.Palettes[i] = malloc(sizeof(struct PALETTE));
      Driver.Palettes[i][0].NumColours = (1 << (i + 1));
      Driver.Palettes[i][0].Colours = malloc(Driver.Palettes[i][0].NumColours * sizeof(RGB));
      memcpy(Driver.Palettes[i][0].Colours,
             DefaultPalette.Colours,
             Driver.Palettes[i][0].NumColours * sizeof(RGB));
      
   }
   
   // read in all of the palettes that exist in the ini file
   i = 0;
   for (;;)
   {
      char PaletteNum[12];
      int NumCols = 0;
      int Index;
      
      sprintf(PaletteNum, "Palette%d", i + 1);
      i++;

      pBuffer = get_config_string("Palette", PaletteNum, "None");
      if (strcmp(pBuffer, "None") == 0)
      {
         // last palette therefor drop out
         break;
      }

      // any errors on the palette read we will just drop out in our current state
      
      // get the number of colours
      if (sscanf(pBuffer, " %d ", &NumCols) != 1) break;
      if ((k = NextNumber(pBuffer)) == 0) break;

      // memory allocations (if needed)
      Index = IndexFromNumCols(NumCols);
      
      if (PlanesUsed[Index] && (Driver.NumPalettes[Index] == 1))
      {
         // we have already malloced a default palette here so overwrite it

         // ensure we don't try and overwrite it again
         PlanesUsed[Index] = 0;
      }
      else
      {
         Driver.Palettes[Index][Driver.NumPalettes[Index]].NumColours = NumCols;
         Driver.Palettes[Index][Driver.NumPalettes[Index]].Colours = malloc(NumCols * sizeof(RGB));
         Driver.NumPalettes[Index] ++;
      }

      if (!LoadPaletteFromBuffer(pBuffer, NumCols, k, Index))
      {
         // failed to load colours
         break;
      }
   }

   printf("\n\n");
   return LoadRoms();
}



// Given a string it copies the colours into the colour array
// returns 1 for success, 0 for failure
static int LoadPaletteFromBuffer(char * pBuffer, int NumCols, int k, int Index)
{
   int j;
   
   // now read in the colour info
   for (j = 0; j < NumCols; j++)
   {
      int retval;

      if (sscanf(&pBuffer[k], " %d ",
                 (int *)&(Driver.Palettes[Index][Driver.NumPalettes[Index]-1].Colours[j].r)) != 1)
         return 0;
      if ((retval = NextNumber(&pBuffer[k])) == 0)
         return 0;
      k+=retval;

      if (sscanf(&pBuffer[k], " %d ",
                 (int *)&(Driver.Palettes[Index][Driver.NumPalettes[Index]-1].Colours[j].g)) != 1)
         return 0;
      if ((retval = NextNumber(&pBuffer[k])) == 0)
         return 0;
      k+=retval;

      if (sscanf(&pBuffer[k], " %d ",
                 (int *)&(Driver.Palettes[Index][Driver.NumPalettes[Index]-1].Colours[j].b)) != 1)
         return 0;
      if ((retval = NextNumber(&pBuffer[k])) == 0)
         return 0;
      k+=retval;
   }

   return 1;
}

      
      
      


// Writes all of the palettes to the INI file in the form
// [Palette]
// Palette1 = 4, 0,63,0, 63,63,63, 0,63,63, 63,0,0
//
// where 4 is the number of colours and the other numbers
// are the rgb of each of the colours.
void WriteAllPalettes()
{
   char Colours[256];
   char Title[20];
	int i, j, PalCount, PalNum = 1;
   int NumColours;

   // select ini file
   push_config_state();
   set_config_file(INIDriverName);

   // cycle though all palette plane sizes
   for (PalCount = 0; PalCount < MAX_COLOUR_PLANES; PalCount++)
   {
      int NumPalettes = Driver.NumPalettes[PalCount];

      for (j = 0; j < NumPalettes; j ++)
      {
         char Buf[20] = {0};
         NumColours = Driver.Palettes[PalCount][j].NumColours;
         Colours[0] = 0;

         // first store the 
         sprintf(Buf, "%d, ", NumColours);
         strcat(Colours, Buf);

         for (i = 0; i < NumColours; i ++)
         {
            RGB temp = Driver.Palettes[PalCount][j].Colours[i];

            if (i < NumColours -1)
               sprintf(Buf, "%d,%d,%d, ", temp.r,temp.g,temp.b);
            else
               sprintf(Buf, "%d,%d,%d", temp.r,temp.g,temp.b);               

            strcat(Colours, Buf);
         }

         sprintf(Title, "Palette%d", PalNum);
         set_config_string("Palette", Title, Colours);
         PalNum++;
      }
   }   
   pop_config_state();
}


// frees the memory used by BuildDriverList. This memory is in use
// for the duration of AGE's runtime.
void FreeDriverList(void)
{
   if (AllDrivers != NULL)
   {
      free (AllDrivers);
      AllDrivers = NULL;
   }

   if (GameIndex != NULL)
   {
      free (GameIndex);
      GameIndex = NULL;
   }
}

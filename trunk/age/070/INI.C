// ini feature utilities
// February, 1998 jerry@mail.csh.rit.edu
//   http://www.csh.rit.edu/~jerry
//


// NOTE: if any other functions need to use any other .ini files,
//       be sure to use the push_config_state() and pop_config_state()
//       function calls!!!

#include <stdio.h>
#include <conio.h>
#include <string.h>
#include "allegro.h"
#include "drivers.h"
#include "fileio.h"
#include "ini.h"


#define CONFIG_FILE "age.ini"
#define CONFIG_INFO "This file was created from AGE."
#define CONFIG_AUTHOR "AGE"

#define NEWINI_MAJOR 0
#define NEWINI_MINOR 7

int ini_version_major = 0;
int ini_version_minor = 0;

extern int display_stretch;		// stretch the map display
extern int increment;			// pcx capture increment
extern char InitGameName[];       // as used by load selector

void Init_INI(void)
{
//   WriteConfigString(CONFIG_FILE, "System", "Info", "Fred");
   
   // initialize the system - create the ini file if necessary
   if (!exists(CONFIG_FILE))
   {
      set_config_file(CONFIG_FILE);
      set_config_int("System", "Version_Major", NEWINI_MAJOR);
      set_config_int("System", "Version_Minor", NEWINI_MINOR);
      set_config_string("System", "Info", CONFIG_INFO);
      set_config_string("System", "Author", CONFIG_AUTHOR);
      Save_INI();
   } else {
      set_config_file(CONFIG_FILE);
      Load_INI();
   }

   ini_version_major = get_config_int("System", "Version_Major", ini_version_major);
   ini_version_minor = get_config_int("System", "Version_Minor", ini_version_minor);

   if (ini_version_major > NEWINI_MAJOR)
   {
      printf("ERROR: The \"%s\" file is newer than AGE.\n"
             "       There might be problems when running AGE!\n"
             "       Winging it!   Press any key to continue...\n",
             CONFIG_FILE);
      (void)getch();
   }
}


void DeInit_INI(void)
{
   // any de-initialization goes here..
}

void Load_INI(void)
{
   // load in all internal bits from the .ini file!
   display_stretch = get_config_int("User", "Maped_Stretch", display_stretch);
   increment = get_config_int("User", "PCX_Increment", increment);
   strcpy(InitGameName, get_config_string("User", "Default_Driver", "none"));

   // read in the path for the ROMs
   strcpy(ROMPath, get_config_string("User", "ROMPath", "."));
}

void Save_INI(void)
{
   // save out all internal bits to be stored
   set_config_int("User", "Maped_Stretch", display_stretch);
   set_config_int("User", "PCX_Increment", increment);

   set_config_string("User", "Default_Driver", InitGameName);
}




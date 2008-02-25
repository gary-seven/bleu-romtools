/* params.h
 *
 * $Id: params.h,v 1.19 2003/05/29 02:50:00 jerry Exp $
 */


#ifndef __PARAMS_H__
#define __PARAMS_H__

#include "gfxlib.h"	/* for "FileFormat" */

#define PARAMSUSAGE \
    "    == COMMON ==\n" \
    "\t-drv DriverName          Select the driver to use\n" \
    "\t-ini AbsoluteDriverPath  Use this specific driver\n" \
    "\t-inf (IMG|ROM)           Select the input format\n"  \
    "\t-bnk BankNumber          Which bank to use\n" \
    "\t-pal PaletteNumber       Which palette to use\n" \
    "\t-key KeyFileName         The palette key filename\n" \
    "\t-rom InputDirectory      Absolute ROM input directory\n" \
    "\t-ff  (PPM|PCX)           Image format to use (PCX is default)\n" \
    "\t-tms TilemapSetName	Import/Export tilemaps along with banks\n" \
    "\t-dmp			Dump out the driver structure\n" \
    "\n" \
    "    == IMG to ROM ==   (-inf IMG)\n"\
    "\t-rod OutputDirectory     Absolute ROM output directory\n" \
    "\n" \
    "    == ROM to IMG ==   (-inf ROM)\n"\
    "\t-dbf BankImageFile       Force this filename for the bank image\n" \
    "\t-chk CheckerboardFile    Force this filename for the checkerboard\n" \
    "\t-wid NumberSpritesWide   How many sprites across\n" \
    "\n"


typedef struct {
    /* common */
    char * drv;	/* driver name */
    char * ini;	/* driver absolute name */
    char * inf; /* input file style IMG or ROM */
    int    bnk;	/* bank number */
    int    pal;	/* palette number */
    char * key;	/* key file name */
    char * rom;	/* rom input directory */

    FileFormat ff; /* which image format to use */
    char * ffs;    /* temporary location for the above, and extension */

    /* IMG to ROM */
    char * rod;	/* rom output directory */

    /* ROM to IMG */
    int eightbit; /* user palette range override for 8 bit color */
    char * dbf;	/* bank output IMG filename */
    char * chk;	/* checkerboard filename */
    int    wid;	/* number of sprites wide we're doing */

    /* tilemap */
    char * tms;  /* tilemap set name */
    int    tmsn; /* number of tilemap set: 0 for none, -1 for error */

    /* oneshot */
    int h;	/* help */
    int v;	/* version */

    /* env vars */
    char * tdrivers;  /* TDRIVERS environment variable */
    char * troms;     /* TROMS    environment variable */

    /* debug stuff */
    int dmp;	/* dump out the driver */
} UserParams;


int params_IsDir( char * path );



/* params_Parse
 *
 *  checks the environment variables and command line and
 *  sets the appropriate parameter options as such 
 *  This needs to be freed later.
 */
UserParams *
params_Parse( UserParams * pup, int argc, char **argv );



/* params_Verify
 *
 * check validity of parameters
 * returns 0 on OK.
 */
int params_Verify( UserParams * pup );


#endif


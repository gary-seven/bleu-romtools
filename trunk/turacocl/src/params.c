/* params.c
 *
 * $Id: params.c,v 1.21 2003/05/29 02:50:00 jerry Exp $
 */

#include <stdio.h>
#include <stdlib.h>  /* malloc */
#include <string.h>  /* strcmp */
#include <sys/types.h> /* opendir */
#include <dirent.h>    /* opendir */
#include "params.h"
#include "errors.h"
#include "gfxlib.h"

/* _params_NewUP
 *
 *  generates a new user params structure
 */ 
UserParams *
_params_NewUP( void )
{
    UserParams * pup = (UserParams *)malloc( sizeof( UserParams ));

    if( pup )
    {
	/* zero it all */

	/* common */
	pup->drv = pup->ini =
	pup->inf = pup->key = pup->rom = NULL;
	pup->bnk = pup->pal = 1;

	/* imageFormat */
	pup->ffs = NULL;
	pup->ff  = ff_PCX;

	/* tilemap set */
	pup->tms  = NULL;
	pup->tmsn = 0;

	/* IMG to ROM */
	pup->rod = NULL;

	/* ROM to IMG */
	pup->wid = 32;
	pup->dbf = pup->chk = NULL;

	/* oneshot */
	pup->v = pup->h = 0;

	/* env vars */
	pup->tdrivers = pup->troms = NULL;

	/* debug stuff */
	pup->dmp = 0;
    }

    return( pup );
}


/* params_ParseArgv  
 * 
 * parse the arg vector, generating a UserParams structure.
 * Need to free it later.
 */
UserParams *
params_ParseArgv( UserParams * pup, int argc, char ** argv )
{
    int ac;

    /* if no params were passed in, create one */
    if (!pup)
	pup = _params_NewUP();

    if (pup)
    {
	/* we need to move over the arg vector to find stuff */
	for( ac=1 ; ac<argc ; ac++ )
	{
	    /* common arguments */
	         if (!strcmp( argv[ac], "-drv" ))  pup->drv = argv[++ac];
	    else if (!strcmp( argv[ac], "-ini" ))  pup->ini = argv[++ac];
	    else if (!strcmp( argv[ac], "-inf" ))  pup->inf = argv[++ac];
	    else if (!strcmp( argv[ac], "-bnk" ))  pup->bnk = atoi(argv[++ac]);
	    else if (!strcmp( argv[ac], "-pal" ))  pup->pal = atoi(argv[++ac]);
	    else if (!strcmp( argv[ac], "-key" ))  pup->key = argv[++ac];
	    else if (!strcmp( argv[ac], "-rom" ))  pup->rom = argv[++ac];
	    else if (!strcmp( argv[ac], "-trom"))  pup->rom = pup->troms;

	    else if (!strcmp( argv[ac], "-ff" ))   pup->ffs = argv[++ac];

	    else if (!strcmp( argv[ac], "-tms" ))  pup->tms = argv[++ac];

	    /* misc one-shot */
	    else if (!strcmp( argv[ac], "-h"   ))  pup->h = 1;
	    else if (!strcmp( argv[ac], "-v"   ))  pup->v = 1;

	    /* debug stuff */
	    else if (!strcmp( argv[ac], "-dmp" ))  pup->dmp = 1;

	    /* IMG to ROM specific*/
	    else if (!strcmp( argv[ac], "-rod" ))  pup->rod = argv[++ac];

	    /* ROM to IMG specific*/
	    else if (!strcmp( argv[ac], "-dbf" ))  pup->dbf = argv[++ac];
	    else if (!strcmp( argv[ac], "-chk" ))  pup->chk = argv[++ac];
	    else if (!strcmp( argv[ac], "-wid" ))  pup->wid = atoi(argv[++ac]);
	}
    }

    return( pup );
}


/* params_IsDir
 * 
 *  returns 0 if the passed in path is not a dir or not writable
 */
int
params_IsDir( char * path )
{
    DIR * td = opendir( path );

    if( td )
    {
	closedir( td );
	return( 1 );
    }
    return( 0 );
}


/* params_ParseEnvVars
 *
 *  checks the environment variables and sets the appropriate
 *  parameter options as such with cheese and pickles
 */
UserParams *
params_ParseEnvVars( UserParams * pup )
{
    char * r;

    /* if no params were passed in, create one */
    if (!pup)
	pup = _params_NewUP();

    if (pup)
    {
	r = getenv("TDRIVERS");   if (r)  pup->tdrivers = r;
	r = getenv("TROMS");      if (r)  pup->troms = r; 
    }

    return( pup );
}


/* params_Parse
 *
 *  checks the environment variables and command line and 
 *  sets the appropriate parameter options as such with 
 *  cheese and pickles
 */
UserParams *
params_Parse( UserParams * pup, int argc, char **argv )
{
    pup = params_ParseEnvVars( pup );
    pup = params_ParseArgv( pup, argc, argv );

    return( pup );
}



/* params_Verify
 *
 * check validity of parameters
 * returns 0 on OK.
 */
int
params_Verify( UserParams * pup )
{
    if( !pup ) return( ERR_NO_PARAMS_STRUCT );

    if( !pup->tdrivers )
    {
	fprintf( stderr, "Error: TDRIVERS environment variable not defined!\n");
	return( ERR_NO_TDRIVERS );
    }

    if( (!pup->inf) || (strcmp(pup->inf, "ROM") && strcmp(pup->inf, "IMG") ) )
    {
	fprintf( stderr, "Error: You must specify either -inf ROM or -inf IMG!\n");
	return( ERR_CONVERSION_UNKNOWN );
    }

    /* adjust the file format selector */
    if( pup->ffs )
    {
	if( !strcmp( pup->ffs, "PPM" ) )
	{
	    pup->ff  = ff_PPM;
	} else {
	    pup->ff  = ff_PCX;
	}
    }

    if( pup->ff == ff_PPM )
    {
	pup->ffs = "pnm";
    } else {
	pup->ffs = "pcx";
    }

    /* just in case */
    if( pup->wid < 1 )
	pup->wid = 1;

    return( ERR_NONE );
}

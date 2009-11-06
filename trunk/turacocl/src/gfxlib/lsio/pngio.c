/*
 *  pngio
 *
 *   simple save/loader for png data
 *
 * $Id: PNG.c,v 1.10 2003/04/08 14:57:44 jerry Exp $
 *
 * $Log: PNG.c,v $
 *
 */

#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include "image.h"
#include <png.h>
#include "pngio.h"
#include "ls.h"
#include "io_util.h"

/* ref: http://zarb.org/~gc/html/libpng.html */


IMAGE *
PNG_Load (
	char * filename
)
{
    FILE * in_fp;
    IMAGE * newimg;

    if (filename == NULL)
    {
	printf("Nothing to save.\n");
	return NULL;
    }

    in_fp = fopen(filename, "r");
    if (in_fp == NULL)
    {
	printf ("%s: Unable to open file\n", filename);
	return NULL;
    }
    
    /* insert loader code here. */
    newimg = Image_Create(320,240,8);
    if (newimg == NULL)
    {
        printf ("%s: Unable to load file.  Memory error\n", filename);
        return NULL;
    }

	/*
    newimg->width = w;
    newimg->height = h;
    newimg->bits = bits;
	*/


    fclose (in_fp);
    return newimg;
}



void
PNG_Save (
	char * filename, 
	IMAGE * tosave
)
{
    FILE * out_fp;

    if (tosave == NULL || filename == NULL)
    {
	printf("Nothing to save.\n");
	return;
    }

    if (tosave->width == 0 || tosave->height == 0)
    {
	printf("%dx%d: Illegal image dimensions\n", 
		    tosave->width, tosave->height);
	return;
    }

    out_fp = fopen(filename, "w");
    if (!out_fp)
    {
	printf ("%s: Unable to open file\n", filename);
	return;
    }

	/* saver */

    fclose(out_fp);    
}


/*////////////////////////////////////////////////////////////////////////////*/

FileFormat
PNG_SupportedFile(
	char * filename
)
{
	FILE *fp;
	unsigned char buf[8];
	int ret;

	/* do some basic checks first. */
	if (!filename) return ff_UNKNOWN;

	fp = fopen(filename, "rb");
	if (!fp) return ff_UNKNOWN;

	/* now do the libpng checks */
	ret = fread(buf, 1, 8, fp);
	fclose(fp);

	if (ret != 8) return ff_UNKNOWN;

	if( !png_check_sig(buf, 8) ) return ff_UNKNOWN;

	return ff_PNG;
}


/*////////////////////////////////////////////////////////////////////////////*/

int
PNG_Query(
        void
)
{
    return( QUERY_TRUECOLOR );
}



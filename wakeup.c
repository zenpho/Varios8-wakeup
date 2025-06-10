/*
Roland VariOS hardware supports a `VPI-01 for VariOS.BIN` the full Roland
VSynth VA (Virtual Analogue) synth engine, albeit monotimbral single part.

Patches with PCM SAMPLED or VARIPHRASE waveforms appear silent on VariOS
(unless you use my `wakeup` tool here).

This tool `wakeup` MODIFIES IN PLACE the binary data of the patch filename
given as its argument. It is used to 'wake up' .P00 patches that are silent.
It simply sets byte 273 and byte 306 (two sound sources set to VA mode).

Usage is typically with a batch process for all .P00 files in a 'PRJ' folder e.g.

$ cd /path/to/wakeup
$ for i in path/to/library.prj/Patches/*.P00 do; echo "$i"; ./wakeup "$i"; done;
*/
#include <stdlib.h> /* for malloc() free() etc */
#include <stdio.h>  /* for printf() etc */
#include <string.h> /* for strdup() strcat() etc */

/* filenames from commandline */
char   *command       = NULL;
char   *srcFname      = NULL; 
 
/* file handling */
size_t srcSizeInBytes = 0;
FILE*  srcFile        = NULL;
FILE*  dstFile        = NULL;

void cleanup()
{
	if( srcFile ){ fclose( srcFile ); }
}

void exitWithError(int err)
{
	printf("ERROR: ");
	if( 1 == err )
	{ 
		printf("usage: %s <filename> ]\n", command);
		printf("modifies filename in place");
	}
	if( 2 == err ){ printf("could not open file %s", srcFname); }

	cleanup();

	printf("\n");
	exit( err );
}

void clearBitsAt(FILE* file, size_t pos, int mask ) // clear bits in the mask
{
	if( file == NULL )         return;
	if( pos > srcSizeInBytes ) return;
	if( pos < 0 )              return;

	fseek( file, pos, SEEK_SET ); // seek to position, for reading
	int v = fgetc( file );        // read value
	fseek( file, pos, SEEK_SET ); // seek to position, before write
	fputc( v & ~mask, file );     // overwrite with masked value
}

void setBitsAt(FILE* file, size_t pos, int mask ) // set bits in the mask
{
	if( file == NULL )         return;
	if( pos > srcSizeInBytes ) return;
	if( pos < 0 )              return;

	fseek( file, pos, SEEK_SET ); // seek to position, for reading
	int v = fgetc( file );        // read value
	fseek( file, pos, SEEK_SET ); // seek to position, before write
	fputc( v | mask, file );      // overwrite with masked value
}


int main(int argc, char* argv[])
{
	command = argv[0]; // name of our executable

	if (argc < 2 || argc > 2) // abort if argument count is invalid
	{
		exitWithError( 1 );
	}
	else if (argc == 2) // try to get filename argument
	{
		srcFname = argv[1];
	}

	srcFile = fopen( srcFname, "r+" ); // open file for writing
	if( srcFile == NULL ){ exitWithError( 2 ); } // error checking

	fseek( srcFile, 0L, SEEK_END ); // determine filesize
	srcSizeInBytes = ftell( srcFile );
	printf("OK opened %s for reading %zu bytes\n", srcFname, srcSizeInBytes);

	clearBitsAt( srcFile, 273, 12 );  // update in place
	clearBitsAt( srcFile, 306, 192 ); // update in place

	cleanup();

	return 0;
}

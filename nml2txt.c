/* [isd] Insane Software Designers - arnvid@isd.no
 * ===============================================
 * nml2txt.c - converts nml playlist files to txt
 *
 * Written for 
 *      Native Instruments - Traktor DJ Studio
 *
 * $Id$
 *
 */

#include <expat.h>
#include <stdlib.h>
#include <unistd.h>
#include <signal.h>
#include <fcntl.h>
#include <string.h>
#include <errno.h>
#include <stdio.h>
#include <stdarg.h>
#include <ctype.h>
#include <sys/types.h>
#include <sys/dir.h>
#include <sys/stat.h>


#define BUFFSIZE        8192

int  songno;
char buff[BUFFSIZE];

static void start(void *data, const char *el, const char **attr)
{
  int i;
  char *artist;
  char *title;

  artist = malloc(512);
  title = malloc(512);

  if (strcmp(el, "PLAYLIST") == 0)
  {
    for (i = 0; attr[i]; i += 2)
    {
      if (strcmp(attr[i], "TITLE") == 0)
      {
        printf("\nMix Title: %s\n\n", attr[i + 1]);
      }
    }
  }
  if (strcmp(el, "ENTRY") == 0)
  {
    songno++;
    for (i = 0; attr[i]; i += 2)
    {
      if (strcmp(attr[i], "ARTIST") == 0)
      {
        strncpy(artist, attr[i + 1], 500);
      }
      if (strcmp(attr[i], "TITLE") == 0)
      {
        strncpy(title, attr[i + 1], 500);
      }
    }
    if (songno < 10) 
    { 
      printf(" %d: %s - %s\n", songno, artist, title);
    } 
    else
    {
      printf("%d: %s - %s\n", songno, artist, title);
    }
  }
  if (title) free(title);
  if (artist) free(artist);
}


static void end(void *data, const char *el)
{
}
  

static void _usage ()
{
  printf ("usage : nml2txt in_file\n");
  printf ("[isd] insane software designers 1989 - 2004\n");
  exit (1);
};

 
int main(int argc, char ** argv, char ** envp) 
{
  XML_Parser p = XML_ParserCreate(NULL);
  FILE *f1;
  char *infile=0;
  songno = 0;

  printf ("[isd] nml2txt - v1.1 - part of djtools\n");
  if (! p) 
  {
    printf("Couldn't allocate memory for parser\n");
    exit(-1);
  }
  if (!(argc == 2 )) _usage();

  infile = argv[1];

  f1 = fopen(infile,"r");
  if (!f1) {
    printf("[nml2txt] unable to create file %s.\n", infile);
    exit(1);
  }

  XML_SetElementHandler(p, start, end);

  for (;;) {
    int done;
    int len;

    len = fread(buff, 1, BUFFSIZE, f1);
    if (ferror(f1)) {
      printf("Read error\n");
      exit(-1);
    }
    done = feof(f1);

    if (XML_Parse(p, buff, len, done) == XML_STATUS_ERROR) {
      fprintf(stderr, "Parse error at line %d:\n%s\n",
              XML_GetCurrentLineNumber(p),
              XML_ErrorString(XML_GetErrorCode(p)));
      exit(-1);
    }

    if (done)
      break;
  }

  printf("\n");
  printf("Total songs in track list: %d\n",songno);
  printf("\n");
  fclose(f1);
  XML_ParserFree(p);
  exit(0);
}


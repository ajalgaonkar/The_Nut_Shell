
/*
 *  Support routines for the nut shell
 */

#include <stdlib.h>
#include <string.h>
#include "funcs.h"


/*
 *
 *  FatalError(sb):  print message and exit.
 *
 */

void FatalError(char *sb)
{
    int cc;
    cc = printf("Fatal Error: %s\n", sb);
    exit(1);
}

/*
 *
 *  getsb(sb) -- returns a new sb of length strlen(sb)
 * 
 */

char *getsb(sb)

char *sb;

{
  char *newsb;

  if (sb == NULL) return(sb);
  newsb = calloc(1, ((int) strlen(sb)) + 1);
  if (newsb == NULL) 
      FatalError("Out of memory in getsb.");
  strcpy(newsb,sb);
  return(newsb);
}


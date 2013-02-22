
/*
 *  Support routines for the nut shell
 */

#include <stdlib.h>
#include <string.h>
#include "funcs.h"
#include <unistd.h>
#include <stdio.h>
#include <errno.h>
#include <dirent.h>
#include <limits.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <pwd.h>
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


/* USER ajalgaonkar DEFINED SECTION */
void identify_word(char *wrd)
{
    printf("Enter in word identify!!  %s \n",wrd);
    if(wrd==NULL)
    printf("Error: No word entered\n");
    if(strcmp(wrd,"dir") == 0)
        get_dir(wrd);
    if(strcmp(wrd,"bye") == 0)
        exit_flag = 1;
    if(strcmp(wrd,"cd")==0){
        struct passwd *pw = getpwuid(getuid());
        const char *homedir = pw->pw_dir;
        if(chdir(homedir))
            printf("Now on Home Dir!\n");
    }
}


void get_dir(char *wrd)
{
    char cwd[1024];
    DIR *d;
    struct stat sb1;

        printf("The directory will be listed...\n");
        if (getcwd(cwd, sizeof(cwd)) != NULL)
        {
            printf("Current working dir: %s\n", cwd);
            d = opendir(cwd);
            while (1)
            {
                struct dirent * entry;
                const char * d_name;
                entry = readdir (d);
                if (! entry)
                {
                    break;
                }
                d_name = entry->d_name;
                if((stat(d_name,&sb1)==0)&&(S_ISREG(sb1.st_mode) && sb1.st_mode & 0111))
                printf ("%s*\n",d_name);
                else if(S_ISDIR(sb1.st_mode))
                printf ("%s/\n",d_name);
                else
                printf ("%s\n",d_name);
            }
            printf("\n");
        }
}
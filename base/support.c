
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
    printf("Enter in word identify!!  %s\n",wrd);
    if(wrd==NULL)
    printf("Error: No word entered\n");
    if(strcmp(wrd,"dir") == 0)
        get_dir(wrd);
    if(strcmp(wrd,"bye") == 0)
        exit_flag = 1;
    if(strcmp(wrd,"cd")==0)
    {
        struct passwd *pw = getpwuid(getuid());
        const char *homedir = pw->pw_dir;
        if(chdir(homedir) == 0)
            printf("Now on '%s'\n",homedir);
    }
    if(strncmp(wrd,"~",1) == 0)
    {
        printf("Enter Expansion");
        char *expansion = tilde_expansion(wrd);
        printf("Directory is: %s\n",expansion);
    }
}


void command_with_arg(char *cmd,char *arg)
{
    printf("Enter int command arg identify!! %s %s \n", cmd, arg);    
}

char *tilde_expansion(char *tilde)
{
    int len = strlen(tilde);
    int i =0;
    printf("\n\nExpansion for USER %s\n\n", tilde);
    if(len == 1)
    {
        return(Home_Dir);
    }
    else{
       
            tilde++;
            char *temp = tilde;
            struct passwd *pw = getpwnam(temp);
            char *user_dir = pw->pw_dir;
            return(user_dir);
        printf("\n\n %s \n\n",temp);
    }

}









void get_dir(char *wrd)
{
    char cwd[1024];
    DIR *d;
    int print_count = 0;
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
                    printf ("%25s* ",d_name);
                else if(S_ISDIR(sb1.st_mode))
                    printf ("%25s/ ",d_name);
                else if(S_ISFIFO(sb1.st_mode))
                    printf("%25s| ",d_name);
                else if(S_ISLNK(sb1.st_mode))
                    printf("%25s@ ",d_name);
                else if(S_ISSOCK(sb1.st_mode))
                    printf("%25s= ",d_name);
                else
                printf ("%25s ",d_name);
                print_count++;
                if(print_count == 5){
                printf("\n");
                print_count =0;
                }
            }
            printf("\n");
        }
}
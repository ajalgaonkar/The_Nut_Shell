
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
	char *args[] = {"command", (char *) 0 };  //Empty args
	
    printf("Enter in word identify!!  %s\n",wrd);
    if(wrd==NULL)
    printf("Error: No word entered\n");
    if(strcmp(wrd,"dir") == 0)
        get_dir(wrd);
    else if(strcmp(wrd,"bye") == 0)
        exit_flag = 1;
    else if(strcmp(wrd,"cd")==0)
    {
        struct passwd *pw = getpwuid(getuid());
        const char *homedir = pw->pw_dir;
        if(chdir(homedir) == 0)
            printf("Now on '%s'\n",homedir);
    }
    else if(strncmp(wrd,"~",1) == 0)
    {
        printf("Enter Expansion");
        char *expansion = tilde_expansion(wrd);
        printf("Directory is: %s\n",expansion);
    }
	
	else if(findAndExecute(wrd,args,NULL,NULL))
		printf("%s: command not found \n",wrd);
}


void command_with_arg(char *cmd,char *arg)
{
    printf("Enter int command arg identify!! %s %s \n", cmd, arg);
    if(strcmp(cmd,"cd")==0)
    {
        char *path;
        path = get_path(arg);
        if(chdir(path) !=0)
        printf("\nInvalid Path\n");
    }
}

char *get_path(char *arg)
{
    if(strncmp(arg,"~",1) == 0)
    {
        return(tilde_expansion(arg));
    }
    else
    return(arg);
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
            char *temp2 = tilde;
            char user[20];
            int i = 0;
            char *path;
            char *path2 = strpbrk(temp,"/");

            if(path2 != NULL)
            {
                printf("\nContains / \n");

                if(strncmp(temp,"/",1) != 0)
                {

                    while(strncmp(temp2,"/",1) != 0)
                    {
                        user[i] = *temp2;
                        temp2++;
                        i++;
                    }
                    i =0;
                    struct passwd *pw = getpwnam(user);
                    if(pw == NULL){
                        printf("Invalid User Name!!\n");
                        return(".");
                    }
                    char *user_dir = pw->pw_dir;
                    printf("\n%s\n",temp);
                    path = strcat(user_dir,path2);
                    printf("\n..%s..\n",path);
                    return(path);
                }
                else
                return(strcat(Home_Dir,path2));
            }
            else
            {
                struct passwd *pw = getpwnam(temp);
                if(pw == NULL){
                    printf("Invalid User Name!!\n");
                    return(".");
                }
                char *user_dir = pw->pw_dir;
                printf("\n..%s..\n",user_dir);
                return(user_dir);
            }
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



//// Varun functions

//Returns  0 if file found and is executable
//Returns -1 if file is not found 
//Returns -2 if file is but not executable
int checkIfExecutable(char *filePath)
{
	struct stat sb;
	if (stat(filePath, &sb) == 0) //File present
	{
		if(S_ISREG(sb.st_mode) && sb.st_mode & 0111)
		{
			printf("%s is executable\n", filePath);
			return 0;
		}
		return -2;
	}
	return -1;
}


//Returns  0 if Dir is found
//Returns -1 if path is not found
//Returns -2 if path is found but not a dir
int checkIfDir(char *dirPath)
{
	struct stat sb;
	if (stat(dirPath, &sb) == 0) //Dir present
	{
		if(S_ISDIR(sb.st_mode))
		{
			//printf("%s is a directory\n", dirPath);
			return 0;
		}
		return -2;
	}
	return -1;
}


int set_read(int* rpipe)
{
    dup2(rpipe[0], STDIN_FILENO);
    close(rpipe[0]); // we have a copy already, so close it
    close(rpipe[1]); // not using this end
	return 0;
}
  
int set_write(int* wpipe)
{
    dup2(wpipe[1], STDOUT_FILENO);
    close(wpipe[0]); // not using this end
    close(wpipe[1]); // we have a copy already, so close it
	return 0;
}


int execFile(char * cmd, char * args[], int* rpipe, int* wpipe)
{
	pid_t cpid;
	
	cpid = fork();
	
	if(cpid < 0)
	{
		printf("\n Fork Failed \n");
		return -1;
	}
	else if(cpid == 0) 					// Child
	{
		if(rpipe) // there's a pipe from the previous process
            set_read(rpipe);
        // else you may want to redirect input from somewhere else for the start
        
		if(wpipe) // there's a pipe to the next process
            set_write(wpipe);
        // else you may want to redirect out to somewhere else for the end
		
		//ExecV file
		execv(cmd,args);
	}
	else								// Parent
	{
		//wait();							// Wait for fpid, command to complete
		return 0;
	}
	
	return 0;
	
}

//Find the path of the command in PATH env variable and execute
//
int findAndExecute(char *cmd, char*args[], int* rpipe, int* wpipe)
{
	char *PATH = "/bin:/usr/bin";
	char paths[10][256];
	
	char cfp[256]; 							// full path of cmd
	int pn=0,i,r1;
	
	//Parse PATH and get individual paths
	strcpy(paths[0],"/usr/bin");
	strcpy(paths[1],"/bin");
	strcpy(paths[2],"../../bin");
	pn=3;
	
	for(i=0;i<pn;i++)
	{
		strcat(cfp,paths[i]);
		strcat(cfp,"/");
		strcat(cfp,cmd);
		if(checkIfExecutable(cfp) == 0)
		{
			//printf("File %s is executable \n",cfp);
			break;
		}
		//else
		//	printf("%s cannot be found or is not an executable\n",cfp);
			
		strcpy(cfp,"");
	}
	if(strlen(cfp)==0)
	{
		printf("%s: command not found \n",cmd);
		return -1;
	}
	printf("Executing command: %s \n",cfp);
	
	execFile(cfp,args,rpipe,wpipe);
	
	return 0;
}




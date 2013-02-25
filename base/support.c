
/*
 *  Support routines for the nut shell
 */


#include <string.h>
#include "linkedlist.h"
#include "funcs.h"
#include <unistd.h>
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
 *		Juned LinkedList Implementation
 */ 
Node *node_head_env = NULL; 								// Juned : Global Pointer to linkedlist head
Node *node_head_aliases = NULL;

int findAndExecute(char *cmd, char*args[], int* rpipe, int* wpipe);

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
    else if( strcmp(wrd,"printenv") == 0 )						// Juned : Call to print the Environment Variables
    { 
    	linkedlist_print(&node_head_env); 							// print the Env linkedlist
    }
    else if( strcmp(wrd,"printenv") == 0 )						// Juned : Call to print the Aliases
    { 
    	linkedlist_print(&node_head_aliases); 							// print the Aliases linkedlist
    }
	else if(findAndExecute(wrd,args,NULL,NULL))
		printf("%s: command not found \n",wrd);
}


void command_with_arg(char *cmd,char *arg)
{
	char *args[3];
	args[0] = cmd;
	args[1] = arg;
	args[2] = (char *)0;
	
    printf("Enter int command arg identify!! %s %s \n", args[0], args[1]);
    if(strcmp(cmd,"cd")==0)
    {
        char *path;
        path = get_path(arg);
        if(chdir(path) !=0)
        printf("\nInvalid Path\n");
    }
    else if( strcmp(cmd,"unset")==0 )
    {
    	if(arg == NULL)
		{    
			printf("\nPlease enter a Variable Name to unset\n");
			return;
		}
		printf("Trying to Delete....\n");
		printf("%s\n", linkedlist_delete_node(&node_head_env, arg)); //pop the linkedlist's head
		 	
    }
	else if(findAndExecute(cmd,args,NULL,NULL))
		printf("%s: command not found \n",cmd);
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
	int i=0;
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
		exit(0);
	}
	else								// Parent
	{
		//wait(0);							// Wait for fpid, command to complete
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


// Juned Functions 

// Returns the lenght of the Linked_List
int linkedlist_len(Node *node_head)
{
    Node *curr = node_head;
    int len = 0;
     
    while(curr)
    {
        ++len;
        curr = curr -> next;
    }
    return len;
}
 

// Inserts Key,Value into the Linked_List 
void linkedlist_insert(Node **node_head, linkedlist_data key, linkedlist_data value)
{
	if(linkedlist_find(node_head, key)) {
		Node *node_curr = *node_head;
	
		do {
			if(strcmp(node_curr -> key, key) == 0) {
				node_curr -> value = value;
				return;
			}
			node_curr = node_curr -> next;
		}while(node_curr);
	}
	else {
	    Node *node_new = malloc(sizeof(Node));
	     
	    node_new -> key = key;
	    node_new -> value = value;
	    node_new -> next = *node_head;
	    *node_head = node_new;
	}
}
 
// Removes the Node of the specified key
linkedlist_data linkedlist_delete_node(Node **node_head, linkedlist_data key)
{
	linkedlist_data result = "No Variable Found !";
	Node *node_curr = *node_head;
	Node *node_prev = NULL;
	
	while(node_curr) {
		if(strcmp(node_curr -> key, key) == 0) {
			result = node_curr -> value;
			if(node_prev == NULL) { 
				*node_head = node_curr -> next;
			}
			else {
				node_prev -> next = node_curr -> next;
			}
			free(node_curr);
			return result;
		}
		node_prev = node_curr;
		node_curr = node_curr -> next;
	}
	return result;
}
 
// Prints the Linked_List
void linkedlist_print(Node **node_head)
{
    Node *node_curr = *node_head;
     
    if(!node_curr)
        puts("No Environment Variables present.");
    else
    {
        while(node_curr)
        {
            printf("%s = %s\n", node_curr -> key, node_curr -> value); //set for integers, modifiable
            node_curr = node_curr -> next;
        }
        putchar('\n');
    }
}
 
// Empties the Linked_List
void linkedlist_clear(Node **node_head)
{ 
    while(*node_head) {
   		Node *node_prev = *node_head;
        *node_head = node_prev -> next;
        free(node_prev);
    }
}
 
// Snocs the Node at the end.
void linkedlist_snoc(Node **node_head, linkedlist_data key, linkedlist_data value)
{
    Node *node_curr = *node_head;
     
    if(!node_curr)
        linkedlist_insert(node_head, key, value);
    else
    {
        //find the last node
        while(node_curr -> next)
            node_curr = node_curr -> next;
        //build the node after it
        linkedlist_insert(&(node_curr -> next), key, value);
    }
}
 
// Returns 1 if Node with k key is present in the Linked_List
int linkedlist_find(Node **node_head, linkedlist_data k)
{
    Node *node_curr = *node_head;
     
    while(node_curr)
    {
        if(strcmp(node_curr -> key, k) == 0) //set for numbers, modifiable
            return 1;
        else
            node_curr = node_curr -> next;
    }
    return 0;
}




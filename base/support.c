
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
 *	 LinkedList Implementation
 */ 
Node *node_head_env = NULL; 
Node *node_head_aliases = NULL;


struct commandLine cmdLine;
struct command * curCmd;

/*
 *  FatalError(sb):  print message and exit.
 */
void FatalError(char *sb)
{
    int cc;
    cc = printf("Fatal Error: %s\n", sb);
    exit(1);
}



/*
 *  getsb(sb) -- returns a new sb of length strlen(sb)
 */
char *getsb(char *sb)
{
  char *newsb;

  if (sb == NULL) return(sb);
  newsb = calloc(1, ((int) strlen(sb)) + 1);
  if (newsb == NULL) 
      FatalError("Out of memory in getsb.");
  strcpy(newsb,sb);
  return(newsb);
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
    //printf("\nExpansion for USER %s\n", tilde);
    if(len == 1)
    {
        return(Home_Dir);
    }
    else{
       
            tilde++;
            char *temp = tilde;
            char *temp2 = tilde;
            char user[20];
	    char temp1[1000];
	    char temp3[1000];
	    char *path_final;
            int i = 0;
            char *path;
            char *path2 = strpbrk(temp,"/");

            if(path2 != NULL)
            {
                //printf("\nContains / \n");

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
                    //printf("\n%s\n",temp);
		    strcpy(temp1,user_dir);
		    strcpy(temp3,path2);
                    path_final = strcat(temp1,temp3);
                    //printf("\n..%s..\n",path_final);
                    return(path_final);
                }
                else{
		strcpy(temp1,Home_Dir);
		strcpy(temp3,path2);
                return(strcat(temp1,temp3));
		}
            }
            else
            {
                struct passwd *pw = getpwnam(temp);
                if(pw == NULL){
                    printf("Invalid User Name!!\n");
                    return(".");
                }
                char *user_dir = pw->pw_dir;
                //printf("\n..%s..\n",user_dir);
                return(user_dir);
            }
       // printf("\n\n %s \n\n",temp);
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


/* -------------------------------- Command Execution Functions ---------------------------*/
//Insert argument at the end of list
void insertArgToList(char *argument, struct command *cmd)
{
	argNode *newArg = (argNode *)malloc(sizeof(argNode));
	argNode *iNode = cmd->argList_head;
	
	if(argument)
	{
		newArg->argv = (char *)malloc(sizeof(char)*strlen(argument));
		strcpy(newArg->argv,argument);
	}
	else
		newArg->argv = NULL;
	
	if(!iNode) //No nodes in list
	{
		newArg->next = NULL;
		cmd->argList_head=newArg;
	}
	else
	{
		while(iNode->next)
			iNode = iNode->next;
		newArg->next = NULL;
		iNode->next = newArg;
	}
	cmd->argc++;
}

//Insert command to command list
void insertCmdToList(struct command *newCmd)
{
	struct command *iCmd;
	if(!cmdLine.cmdListHead)     //Empty List
	{
		newCmd->nextCmd=NULL;
		cmdLine.cmdListHead = newCmd;
	}
	else
	{
		newCmd->nextCmd=NULL;
		iCmd = cmdLine.cmdListHead;
		while(iCmd->nextCmd)
		{
			iCmd = iCmd->nextCmd;
		}
		iCmd->nextCmd = newCmd;
	}
	cmdLine.commandCnt++;
}

/* 
 * Creates a command with out arguments
 * If command has arguments, it would be created in addArgToCurCmd() and 
 * cmd itself is updated here
 */

int insertCommand(char *cmd)
{
	//Perform Tilde Expansion and alias resolution on command
	int i;
	struct command * newCmd;
	argNode *newArg;
	
	if(cmd)
	{
		if(!curCmd) 								//Command with no arguments are created here
		{
			newCmd = (struct command *)malloc(sizeof(struct command));
			newCmd->argc = 0;
			newCmd->cmd = cmd;
			newCmd->argList_head = NULL;
			
			insertArgToList(cmd,newCmd);
			insertArgToList(NULL,newCmd);
			
			insertCmdToList(newCmd);
		}
		else										//Commands with arguments already parsed comes here
		{
			curCmd->cmd = cmd;
			insertArgToList(NULL,curCmd);
			
			//Insert command to front of arg list. arg[0]
			newArg = (argNode *)malloc(sizeof(argNode));
			newArg->argv = (char *)malloc(sizeof(char)*strlen(cmd));
			strcpy(newArg->argv,cmd);
			newArg->next=curCmd->argList_head;
			curCmd->argList_head=newArg;
			curCmd->argc++;
		}
		
		curCmd = NULL;
		
		return 0;
	}
	return -1;
}

/*
 * Adds an argument to the current command if one exists
 * If current command doesnt exist, will create a NULL cmd,
 * which is updated later in insertCommand()
 */
void addArgToCurCmd(char *arg)
{
	if(curCmd)
	{
		insertArgToList(arg,curCmd);
	}
	else if(!curCmd) 
	{
		curCmd = (struct command *)malloc(sizeof(struct command));
		curCmd->cmd=NULL;
		curCmd->argc = 0;
		curCmd->argList_head = NULL;
		insertCmdToList(curCmd);
		
		insertArgToList(arg,curCmd);
	}
}

// Sets a file name for input redirection
void setInputRedir(char *fn)
{
	cmdLine.inFile = fn;	
}

// Sets a file name for output redirection
void setOutputRedir(char *fn, int mode)
{
	cmdLine.outFile = fn;
	cmdLine.outWriteMode = mode;
}

// Sets a file name for error redirection
void setErrRedir(char *fn, int mode)
{
	cmdLine.errFile = fn;
	cmdLine.errWriteMode = mode;
}

//Converts the linked kist of arguments to string array
char ** charListToArray(argNode *head, int count) 
{
	argNode *tnode;
	char **argArray;
	int i=0,n;
	
	argArray = (char **) malloc(count * sizeof(char*));
	tnode = head;
	while(tnode)
	{
		if(tnode->argv)
		{
			
			argArray[i] = (char *) malloc(strlen(tnode->argv)*sizeof(char));
			strcpy(argArray[i],tnode->argv);
			
		}
		else
		{
			argArray[i]=NULL;
		}
		tnode = tnode->next;
		i++;
	}
	n=i;
	
	return argArray;
}

// Prints the parsed command line stored in the structure
void printCmdline()
{
	int i=0,j=0;
	argNode *tnode=NULL;
	struct command *tcmd;
	char **argArray;
	
	printf("\nNumber of commands: %d\n",cmdLine.commandCnt);
	tcmd = cmdLine.cmdListHead;
	
	while(tcmd)
	{
		printf("\nCMD%d: \"%s\" ARG#=%d\n",i+1,tcmd->cmd,tcmd->argc-2);
		tnode = tcmd->argList_head;
		j=0;
		argArray = charListToArray(tnode,tcmd->argc);
		for(j=0;j<tcmd->argc;j++)
		{
			printf("\tARG%d: \"%s\" \n",j,argArray[j]);
		}
		/*
		while(tnode->next)
		{
			printf("\tARG%d: \"%s\" \n",j,tnode->argv);
			j++;
			tnode = tnode->next;
		}
		*/
		tcmd = tcmd->nextCmd;
		i++;
	}
	printf("\n");
	
	if(cmdLine.inFile)
		printf("Input Stream for first command will be read from file: %s\n",cmdLine.inFile);
		
	if(cmdLine.outFile && cmdLine.outWriteMode==0)
		printf("Output Stream of command line will be Written to file: %s\n",cmdLine.outFile);
	else if(cmdLine.outFile)
		printf("Output Stream of command line will be Appended to file: %s\n",cmdLine.outFile);
		
	if(cmdLine.errFile && cmdLine.errWriteMode==0)
		printf("Error Stream of command line will be Written to file: %s\n",cmdLine.errFile);
	else if(cmdLine.errFile)
		printf("Error Stream of command line will be Appended to file: %s\n",cmdLine.errFile);

}

//Resets the data structure and frees the Command Line memory
void clearCmdLine()
{
	argNode *tnode,*fnode;
	struct command *tcmd,*fcmd;
	
	tcmd = cmdLine.cmdListHead;
	while(tcmd)
	{
		tnode = tcmd->argList_head;
		while(tnode)
		{
			fnode = tnode;
			tnode = tnode->next;
			free(fnode);
		}
		fcmd = tcmd;
		tcmd = tcmd->nextCmd;
		free(fcmd);
	}
	cmdLine.cmdListHead = NULL;
	cmdLine.commandCnt = 0;
	cmdLine.inFile = NULL;
	cmdLine.outFile = NULL;
	cmdLine.errFile = NULL;
}


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
			//printf("%s is executable\n", filePath);
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

//Fork and Exec the specified command with its read and write FDs defined
int execCmd(char * cmd, char **args, int* rpipe, int* wpipe, int infd, int outfd, int errfd)
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
        else if(infd>0)
		{
			dup2(infd, STDIN_FILENO);
		}
        
		if(wpipe) // there's a pipe to the next process
            set_write(wpipe);
        else if(outfd>0)
		{
			dup2(outfd, STDOUT_FILENO);
		}
		
		if(errfd>0)
			dup2(errfd, STDERR_FILENO);

		//ExecV file
		execv(cmd,args);
		exit(0);
	}
	else								// Parent
	{
		return 0;
	}
	return 0;
}

//Find the path of the command in PATH env variable and execute
//
int findAndExecCmd(char *Ocmd, char **args, int* rpipe, int* wpipe, int infd, int outfd, int errfd)
{
	char *PATH = "/bin:/usr/bin";
	char paths[10][256];
	char *path;
	char cfp[256]; 							// full path of cmd
	int pn=0,i,builtin=1;
	
	char *cmd = alias_find(&node_head_aliases, Ocmd);				// Chec if the command is not a builtin and is an alias
	
	if(cmd==NULL)
		printf("Error: No word entered\n");
    else if(strcmp(cmd,"dir") == 0)
        get_dir(cmd);
    else if(strcmp(cmd,"bye") == 0)
    {	
    	//printf("Thank You for using Nutty Shell. Good Bye !\n");
        exit(0);
    }
    else if(strcmp(cmd,"cd")==0)
    {
		if(args[1])
		{
			path = get_path(args[1]);
			if(chdir(path) !=0)
				printf("\nInvalid Path\n");
		}
		else
		{
			struct passwd *pw = getpwuid(getuid());
			const char *homedir = pw->pw_dir;
			if(chdir(homedir) == 0)
				printf("Now on '%s'\n",homedir);
		}			
    }
    else if(strncmp(cmd,"~",1) == 0)
    {
        //printf("Enter Expansion");
        char *expansion = tilde_expansion(cmd);
        printf("Directory is: %s\n",expansion);
    }
    else if( strcmp(cmd,"printenv") == 0 )								// Call to print the Environment Variables
    { 
    	linkedlist_print(&node_head_env); 								// print the Env linkedlist
    }
    else if( strcmp(cmd,"unset")==0 )
    {
    	if(args[1] == NULL)
		{    
			printf("\nPlease enter a Variable Name to unset\n");
			return 0;
		}
		printf("Trying to Delete....\n");
		printf("%s\n", linkedlist_delete_node(&node_head_env, args[1])); 		// Deleete the linkedlist's head
		 	
    }
    else if( strcmp(cmd,"unalias")==0 )
    {
    	if(args[1] == NULL)
		{    
			printf("\nPlease enter a word to unset\n");
			return 0;
		}
		printf("Trying to Delete....\n");
		printf("%s\n", linkedlist_delete_node(&node_head_aliases, args[1])); 	// Delete the linkedlist's head
		 	
    }
    else if( strcmp(cmd,"setenv") == 0 )
    {
    	if(args[1] != NULL && args[2] != NULL)
    	{
    		linkedlist_insert(&node_head_env, args[1],args[2]); 				// Insert the Variable,Word Environment Variable Pair
    	}
    	else
    		printf("\nInvalid command to set Environment Variables.\n");
    }
    else if( strcmp(cmd,"alias") == 0 )
    {
    	if(args[1] != NULL && args[2] != NULL )
    	{
    		linkedlist_insert(&node_head_aliases, args[1],args[2]); 			// Insert the Word,String Aliases Pair
    	}
    	else
    		linkedlist_print(&node_head_aliases); 							// print the Aliases linkedlist
    		//printf("\nInvalid command to set Aliases.\n");
    }
	else
	{
		builtin=0;
	}
	
	if(builtin)
	{
		return 0;
	}
	
	if(0)	//isPath(cmd)
	{
		if(checkIfExecutable(cmd)==0)
			execCmd(cmd,args,rpipe,wpipe,infd,outfd,errfd);
		else
			printf("File is not executable: %s\n",cmd);
	}
	else		//Find the cmd in PATH variable
	{
		//TODO: Parse PATH and get individual paths
		strcpy(paths[0],"/usr/bin");
		strcpy(paths[1],"/bin");
		strcpy(paths[2],"../../bin");
		pn=3;
		
		strcpy(cfp,"");
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
				//printf("%s cannot be found or is not an executable\n",cfp);
				
			strcpy(cfp,"");
		}
		if(strlen(cfp)<1)
		{
			printf("Command not found: %s \n",cmd);
			return -1;
		}
		//printf("Executing command: %s \n",cfp);
		execCmd(cfp,args,rpipe,wpipe,infd,outfd,errfd);
	}
	
	return 0;	
}

void execCmdLine()
{

	int rpipe[2], wpipe[2];
	int i;
	struct command *tcmd;
	argNode *tnode;
	int infd=1,outfd=-1,errfd=-1;
	
	// Open files for redirection if specified
	if(cmdLine.inFile)	//Check if Input 
	{
		infd = open(cmdLine.inFile, O_RDONLY);
		if(infd<0)
			printf("Error Opening File %s for Read",cmdLine.inFile);
	}		
			
	if(cmdLine.outFile)
	{
		if(cmdLine.outWriteMode)
		{
			outfd = open(cmdLine.outFile, O_WRONLY|O_CREAT, O_APPEND);
			if(outfd<0)
				printf("Error opening File %s in Append mode\n",cmdLine.outFile);
		}
		else
		{
			outfd = open(cmdLine.outFile, O_WRONLY|O_CREAT);
			if(outfd<0)
				printf("Error Opening File %s in Write mode\n",cmdLine.outFile);
		}
	}
	
	if(cmdLine.errFile)
	{
		if(cmdLine.errWriteMode)
		{
			errfd = open(cmdLine.errFile, O_WRONLY|O_CREAT, O_APPEND);
			if(errfd<0)
				printf("Error opening File %s in Append mode\n",cmdLine.errFile);
		}
		else
		{
			errfd = open(cmdLine.errFile, O_WRONLY|O_CREAT);
			if(errfd<0)
				printf("Error Opening File %s in Write mode\n",cmdLine.errFile);
		}
	}
	
	//Basic Case, single command
	if(cmdLine.commandCnt==1 ) 
	{
		tcmd = cmdLine.cmdListHead;
		findAndExecCmd(tcmd->cmd,charListToArray(tcmd->argList_head,tcmd->argc),NULL,NULL,infd,outfd,errfd);
		cmdLine.inFile = NULL;
		cmdLine.outFile = NULL;
		//wait();
	}
	
	//Multiple commands needing pipe
	if(cmdLine.commandCnt>1)
	{
		// create the first output pipe
		pipe(wpipe);
		
		tcmd = cmdLine.cmdListHead;
		// first child takes input from somewhere else
		findAndExecCmd(tcmd->cmd,charListToArray(tcmd->argList_head,tcmd->argc),NULL,wpipe,infd,outfd,errfd);
		
		tcmd = tcmd->nextCmd;
		cmdLine.inFile = NULL;
		// output pipe becomes input for the next process.
		rpipe[0] = wpipe[0];
		rpipe[1] = wpipe[1];

		// chain all but the first and last children
		while(tcmd->nextCmd)
		{
			pipe(wpipe); // make the next output pipe
			findAndExecCmd(tcmd->cmd,charListToArray(tcmd->argList_head,tcmd->argc),rpipe, wpipe,infd,outfd,errfd);
			close(rpipe[0]); // both ends are attached, close them on parent
			close(rpipe[1]);
			rpipe[0] = wpipe[0]; // output pipe becomes input pipe
			rpipe[1] = wpipe[1];
			tcmd = tcmd->nextCmd;
		}

		// fork the last one, its output goes somewhere else
		findAndExecCmd(tcmd->cmd,charListToArray(tcmd->argList_head,tcmd->argc),rpipe,NULL,infd,outfd,errfd);
		close(rpipe[0]);
		close(rpipe[1]);
		//
		for(i = 0; i < cmdLine.commandCnt; i++)
		{
			//wait(); 	//Wait for all commands to complete
		}
		
	}
	
	//sleep(1);
}

void processCmdLine()
{
	//printCmdline();
	execCmdLine();
	clearCmdLine();
	//sleep(1);
}

//---------------------------------Command Functions end -----------------------------//

// Linked List Functions 

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
	printf("Successfully Inserted: %s\n", value);
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
        puts("No Variables present.");
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

// Returns alias value if Node with k key is present in the Linked_List of Alias
linkedlist_data alias_find(Node **node_head, linkedlist_data k)
{
    Node *node_curr = *node_head;
     
    while(node_curr)
    {
        if(strcmp(node_curr -> key, k) == 0) //set for numbers, modifiable
        {
            return node_curr -> value;
        }
        else
            node_curr = node_curr -> next;
    }
    return k;
}

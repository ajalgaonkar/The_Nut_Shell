#include <stdio.h>
#include <unistd.h>
#include <string.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
 
#ifndef dstructuresh
#define dstructuresh

/*---------- Structures and Methods to manage command line ----------- */

/*
 * Structure to maintain list of arguments of a command
 */
typedef struct arg_node argNode; 
struct arg_node 
{
    char *argv;
    argNode *next;
};

/*
 * Structure for storing a sub command 
 * A basic command with no redirections and pipes attached
 * Consists of command name and argument list and arg count
 * Also has nextCmd pointer to maintain the list of commands
 */
struct command {
	char *cmd;
	int argc;
	argNode *argList_head;
	struct command *nextCmd;
};

/*
 * Structure stores all the sub commands in the command line in the order specified 
 * and any redirection to or from files. I/O of subcommands are pipelined in the order
 */
struct commandLine {
	int commandCnt;					// Total number of sub commands in the command line
	struct command *cmdListHead;		// commands in the command line and its arguments
	
	char * inFile;		//File name if input needs to be redirected
	char * outFile;		//File name if output redirection is specified
	char * errFile;		//File name if error stream 
	int outWriteMode;  	// 0 for Write 1 for Append
	int errWriteMode;  	// 0 for Write 1 for Append
};

extern struct commandLine cmdLine;
extern struct command * curCmd;

extern int insertCommand(char *cmd);
extern void addArgToCurCmd(char *arg);
extern void setInputRedir(char *);
extern void setOutputRedir(char *, int mode);
extern void setErrRedir(char *, int mode);
extern void printCmdline();
extern void execCmdLine();
extern int findAndExecCmd(char *cmd, char **args, int *rpipe, int* wpipe, int infd, int outfd, int errfd);


extern int exit_flag;
extern char *Home_Dir;

extern char *getsb(char *sb);

extern void identify_word(char *wrd);
extern void get_dir(char *wrd);
extern void command_with_arg(char *cmd,char *arg);
extern char *tilde_expansion(char *tilde);
extern char *get_path(char *arg);

extern int initialize();

#endif



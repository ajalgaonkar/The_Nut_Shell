#include <stdio.h>
#include <unistd.h>
#include <string.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
 
#ifndef dstructuresh
#define dstructuresh

/*---------- Varun ----------- */
#define MAXARGS 16
#define MAXCMDS 16

/*
 * Structure for storing a sub command - a basic command with no redirections and pipes attached
 * consists of command name and argument list
 */
struct command {
	char *cmd;
	int argc;
	char *argv[MAXARGS];
};


/*
 * Structure stores all the sub commands in the command line in the order specified 
 * and any redirection to or from files. I/O of subcommands are pipelined in the order
 */
struct commandLine {
	int commandCnt;					// Total number of sub commands in the command line
	struct command *commands[MAXCMDS];		// commands in the command line and its arguments
	
	char * inFile;		//File name if input needs to be redirected
	char * outFile;		//File name if output redirection is specified
	char * errFile;		//File name if error stream 
	int outWriteMode;  	// 0 for Write 1 for Append
	int errWriteMode;  	// 0 for Write 1 for Append
};

extern struct commandLine cmdLine;
extern struct command * curCmd;

extern int insertCommand(char *cmd,char *args[],int argc);
extern void addArgToCurCmd(char *arg);
extern void setInputRedir(char *);
extern void setOutputRedir(char *, int mode);
extern void setErrRedir(char *, int mode);
extern void printCmdline();
extern void processCmdLine();
extern void execCmdLine();
extern int findAndExecCmd(char *cmd, char*args[], int *rpipe, int* wpipe);
/*---------- varun decl end ------------------*/


extern int exit_flag;
extern char *Home_Dir;

extern char *getsb(char *sb);

extern void identify_word(char *wrd);
extern void get_dir(char *wrd);
extern void command_with_arg(char *cmd,char *arg);
extern char *tilde_expansion(char *tilde);
extern char *get_path(char *arg);

extern int initialize();
extern void testFunc();

#endif



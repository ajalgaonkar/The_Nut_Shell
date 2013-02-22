
/*
 *    Nut.y	 --  YACC code for Nut Shell 
 *
 *
 */
%{
#include <stdio.h> 
#include <sys/types.h>
#include "nutty.h"    /* declarations of external variables */
#include "funcs.h"    /* declarations of functions declared in support.c */

    int YaccDebugFlag = 1;
    void YaccDebug();
%}
%union  {
	char ***plist;		/* pipe cmd list	*/
	char **args;		/* cmd/argument list 	*/
	char *sb;		/* string	 	*/
	int  i;			/* integer		*/
	}
%token BYE 
%token <sb> CD
%token <sb> DDOT
%token <sb> DOT
%token <sb> EOL
%token GREATER
%token GTAMP
%token GTGT
%token <sb> HISTORY
%token <sb> INTEGER
%token LESS
%token <sb> LIST
%token MINUS
%token PATH
%token PIPE
%token PROMPT
%token SEMI
%token <sb> SET
%token <sb> SLASH
%token <sb> STRING
%token <sb> TILDE
%token <sb> TILDEWORD
%token TWOGT
%token UNKNOWN
%token <sb> WORD
%start command 
%type <args> 	command2
%type <i>	command
%type <plist> 	pipe_list
%type <args> 	pipe
%type <sb>	input
%type <sb>	output
%type <args>	arg_list
%type <sb>	arg
%%
command :	command2 EOL
			{
			    YaccDebug("command2 EOL");
			    YYACCEPT;
			}
	|	command2 pipe_list EOL
			{
			    YaccDebug("command2 pipe_list EOL");
			    YYACCEPT;			    
			}
	|	command2 input EOL
			{
			    YaccDebug("command2 input EOL");
			    YYACCEPT;			    
			}
	|	command2 output EOL
			{
			    YaccDebug("command2 output EOL");
			    YYACCEPT;			    
			}
	|	command2 pipe_list input EOL
			{
			    YaccDebug("command2 pipe_list input EOL");
			    YYACCEPT;			    
			}
	|	command2 pipe_list output EOL
			{
			    YaccDebug("command2 pipe_list output EOL");
			    YYACCEPT;
			}
	|	command2 input output EOL
			{
			    YaccDebug("command2 input output EOL");
			    YYACCEPT;
			}
	|	command2 pipe_list input output EOL
			{
			    YaccDebug("command2 pipe_list input output EOL");
			    YYACCEPT;
			}
	|	error EOL
			{
			    YaccDebug("error EOL");
			    YYABORT;
			}
	|	EOL
			{
			    YaccDebug("EOL");
			    YYACCEPT;
			}
	;
command2:      	WORD
			{
			    /* put code here to distinguish builtins */
			    /* from builtin commands */
			    identify_word($1);
			    YaccDebug("WORD");
			}
	|	WORD arg_list
			{
			    /* put code here to distinguish builtins */
			    /* from builtin commands */
			    YaccDebug("WORD arg_list");
			}
	;
pipe	:	PIPE command2
			{
			    YaccDebug("PIPE command2");
			}
	;
pipe_list: 	pipe
			{
			    YaccDebug("pipe");
			}
	|	pipe pipe_list
			{
			    YaccDebug("pipe pipe_list");
			}
	;
input	: 	LESS WORD
			{
			    YaccDebug("LESS WORD");
			}
	;
output:		output2
			{
			    YaccDebug("output2");
			}
	|	output2 error_redir
			{
			    YaccDebug("output2 error_redir");
			}
	|	error_redir
			{
			    YaccDebug("error_redir");
			}
	;
output2	:	GREATER WORD
			{
			    YaccDebug("GREATER WORD");
			}
	|       GTGT WORD 
			{
			    YaccDebug("GTGT WORD");
    			}
	;
error_redir:	TWOGT WORD
			{
			    YaccDebug("TWOGT WORD");
			}
	|	GTAMP
			{
			    YaccDebug("GTAMP");
			}
	;
arg	: 	INTEGER
			{
			    YaccDebug("INTEGER");
			}
	|	WORD
			{
			    YaccDebug("WORD");
			}
	|	STRING
			{
			    YaccDebug("STRING");
			}
	|	MINUS arg
			{
			    YaccDebug("MINUS arg");
			}
	;
arg_list:	arg
			{
			    YaccDebug("arg");
			}
	|	arg arg_list
			{
			    YaccDebug("arg_list");
			}
	;
%%

yyerror(sb1)

char *sb1;

{
	fprintf(stderr, "Error detected:  %s\n", sb1);
}

void YaccDebug(char *sb)
{
    if (YaccDebugFlag) {
	fprintf(stderr, "-- %s --\n", sb);
    }

}

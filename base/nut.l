%{
/*  
 *    Nut.l      --   use lex to scan for tokens. Return token number
 *		      and token value if the token is valid.
 *
 */
#include <stdio.h>
#include <strings.h>
#include "info.h"
char *getsb();

int LexDebugFlag = 1;
void LexDebug();

/* other include files and global variable declarations */
%}
D	[0-9]
L	[a-zA-Z]
W	[\t ]
BANG	!
I	{D}+
F	[^\"\n\t \'\`;><=|]
%%
{W}*			;
{I}			{
    				LexDebug("INTEGER ");
				yylval.sb = getsb(yytext);
				return(INTEGER);
			}
">"			{
    				LexDebug("GREATER ");    
    				return(GREATER);
			}
">>"			{
    				LexDebug("GTGT " );
    				return(GTGT);
			}
"2>&1"			{
    				LexDebug("GTAMP ");
    				return(GTAMP);
			}
"2>"			{
    				LexDebug("TWOGT ");
    				return(TWOGT);
			}
"<"			{
    				LexDebug("LESS ");
    				return(LESS);
			}
"|"			{
    				LexDebug("PIPE ");
    				return(PIPE);
			}
"-"			{
    				LexDebug("MINUS ");
    				return(MINUS);
			}
"="			{
    				LexDebug("EQUALS ");
    				return(EQUALS);
			}			
\n			|
";"			{
			yylval.sb = getsb(yytext);
			LexDebug("EOL\n");
			return(EOL);
			}
\'[^\n\']*\'		|
\`[^\n\`]*\`		|
\"[^\n\"]*\"		{
			yylval.sb[yyleng - 1] = 0;
			yylval.sb = getsb(yytext+1);
			LexDebug("STRING ");
			return(STRING);
			}
{F}+			{
			yylval.sb = getsb(yytext);
			LexDebug("WORD ");
			return(WORD);
			}
.			{
    			LexDebug("UNKNOWN ");
			return (UNKNOWN); 
			}
%%
/*  
 * some routines for nut.l, don't try to put too many routine heres - it
 * takes longer time to compile a .l file than a .c file.  I recommend
 * a separate file of routines (e.g., support.c).
 *
 */

void LexDebug(char *sb)
{
    if (LexDebugFlag) {
    	//fprintf(stderr, sb);
    }
}

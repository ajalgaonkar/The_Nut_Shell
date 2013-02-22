/*
 * main routine for shell
 */

#include <pwd.h>
#include "info.h"
#include "init.h"
#include "funcs.h"
#include <sys/time.h>
#include <signal.h>
void *malloc();
int exit_flag;
main(argc, argv, envp)

int argc;
char **argv, **envp;

{
	extern int yydebug;
	char c;
	Prompt = "nut> ";

	initialize();

	printf(Prompt);
	while((c = getchar()) != EOF) {
		ungetc(c, stdin);
		yyparse();
		printf(Prompt);
		fflush(stdout);
		if(exit_flag == 1){
			printf("Thankyou for using Nutty Sheel :)!! \n\n");
			break;
		}
	}
	return 0;
}


/*
 *	initialize -- initialize the global vars.
 */

int initialize()

{
    /* Defined by student */
    exit_flag = 0;
    return 0;
}

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
	}

}


/*
 *	initialize -- initialize the global vars.
 */

initialize()

{
    /* Defined by student */
}

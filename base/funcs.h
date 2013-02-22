
/*
 *  Declare all functions called from nut.y here, and
 *  then implement them in support.c.
 */
extern int exit_flag;
extern char *Home_Dir;

extern char *getsb(char *sb);

extern void identify_word(char *wrd);
extern void get_dir(char *wrd);
extern void command_with_arg(char *cmd,char *arg);
extern char *tilde_expansion(char *tilde);

extern int initialize();
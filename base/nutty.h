extern char 	*Prompt, 		/* prompt string 	*/
		*MyHome;		/* Home dir      	*/

extern char	**Path;			/* path for dirlist 	*/

extern char **environ;

struct HistEnt 	{			/* entry in Hist table 	*/
	int  valid;			/* true if valid	*/
	char **cmd;			/* command		*/
	char ***pipes;			/* pipes		*/
	char *in;			/* input redirect	*/
	char *out;			/* output redirect	*/
	};

#define MAXHIST 1000

extern struct HistEnt	Hist[];		/* History Table	*/

extern int 	cHist,			/* Size of History Tbl	*/
		hFirst,			/* First entry in H Tbl */
		Hcount,			/* count of stuff in tbl*/
		hLast;			/* Pointer in Hist Tbl	*/

struct RWcach { 
	char 	*sbHost;		/* host name short form	*/
	int  	load5;			/* 5 min load avg.	*/
	};

extern struct RWcach RWcache[];	/* cache for rwho	*/

struct OCache {
	char 		*sbCmd;		/* command name		*/
	char 		**hplist;	/* list of host/port	*/
	long		time;		/* timestamp for flush	*/
	};

extern struct OCache OrCache[];

#include <stdio.h>

/* A Bison parser, made by GNU Bison 2.3.  */

/* Skeleton interface for Bison's Yacc-like parsers in C

   Copyright (C) 1984, 1989, 1990, 2000, 2001, 2002, 2003, 2004, 2005, 2006
   Free Software Foundation, Inc.

   This program is free software; you can redistribute it and/or modify
   it under the terms of the GNU General Public License as published by
   the Free Software Foundation; either version 2, or (at your option)
   any later version.

   This program is distributed in the hope that it will be useful,
   but WITHOUT ANY WARRANTY; without even the implied warranty of
   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
   GNU General Public License for more details.

   You should have received a copy of the GNU General Public License
   along with this program; if not, write to the Free Software
   Foundation, Inc., 51 Franklin Street, Fifth Floor,
   Boston, MA 02110-1301, USA.  */

/* As a special exception, you may create a larger work that contains
   part or all of the Bison parser skeleton and distribute that work
   under terms of your choice, so long as that work isn't itself a
   parser generator using the skeleton or a modified version thereof
   as a parser skeleton.  Alternatively, if you modify or redistribute
   the parser skeleton itself, you may (at your option) remove this
   special exception, which will cause the skeleton and the resulting
   Bison output files to be licensed under the GNU General Public
   License without this special exception.

   This special exception was added by the Free Software Foundation in
   version 2.2 of Bison.  */

/* Tokens.  */
#ifndef YYTOKENTYPE
# define YYTOKENTYPE
   /* Put the tokens into the symbol table, so that GDB and other debuggers
      know about them.  */
   enum yytokentype {
     BYE = 258,
     CD = 259,
     DDOT = 260,
     DOT = 261,
     EOL = 262,
     GREATER = 263,
     GTAMP = 264,
     GTGT = 265,
     HISTORY = 266,
     INTEGER = 267,
     LESS = 268,
     LIST = 269,
     MINUS = 270,
     PATH = 271,
     PIPE = 272,
     PROMPT = 273,
     SEMI = 274,
     SET = 275,
     SLASH = 276,
     STRING = 277,
     TILDE = 278,
     TILDEWORD = 279,
     TWOGT = 280,
     UNKNOWN = 281,
     WORD = 282
   };
#endif
/* Tokens.  */
#define BYE 258
#define CD 259
#define DDOT 260
#define DOT 261
#define EOL 262
#define GREATER 263
#define GTAMP 264
#define GTGT 265
#define HISTORY 266
#define INTEGER 267
#define LESS 268
#define LIST 269
#define MINUS 270
#define PATH 271
#define PIPE 272
#define PROMPT 273
#define SEMI 274
#define SET 275
#define SLASH 276
#define STRING 277
#define TILDE 278
#define TILDEWORD 279
#define TWOGT 280
#define UNKNOWN 281
#define WORD 282




#if ! defined YYSTYPE && ! defined YYSTYPE_IS_DECLARED
typedef union YYSTYPE
#line 16 "nut.y"
{
	char ***plist;		/* pipe cmd list	*/
	char **args;		/* cmd/argument list 	*/
	char *sb;		/* string	 	*/
	int  i;			/* integer		*/
	}
/* Line 1529 of yacc.c.  */
#line 110 "y.tab.h"
	YYSTYPE;
# define yystype YYSTYPE /* obsolescent; will be withdrawn */
# define YYSTYPE_IS_DECLARED 1
# define YYSTYPE_IS_TRIVIAL 1
#endif

extern YYSTYPE yylval;


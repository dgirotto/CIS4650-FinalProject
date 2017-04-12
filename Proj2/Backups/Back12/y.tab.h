
/* A Bison parser, made by GNU Bison 2.4.1.  */

/* Skeleton interface for Bison's Yacc-like parsers in C
   
      Copyright (C) 1984, 1989, 1990, 2000, 2001, 2002, 2003, 2004, 2005, 2006
   Free Software Foundation, Inc.
   
   This program is free software: you can redistribute it and/or modify
   it under the terms of the GNU General Public License as published by
   the Free Software Foundation, either version 3 of the License, or
   (at your option) any later version.
   
   This program is distributed in the hope that it will be useful,
   but WITHOUT ANY WARRANTY; without even the implied warranty of
   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
   GNU General Public License for more details.
   
   You should have received a copy of the GNU General Public License
   along with this program.  If not, see <http://www.gnu.org/licenses/>.  */

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
     IF = 258,
     ELSE = 259,
     INT = 260,
     RETURN = 261,
     VOID = 262,
     WHILE = 263,
     ASSIGN = 264,
     EQ = 265,
     LT = 266,
     LTEQ = 267,
     GT = 268,
     GTEQ = 269,
     NEQ = 270,
     PLUS = 271,
     MINUS = 272,
     TIMES = 273,
     OVER = 274,
     LPAREN = 275,
     RPAREN = 276,
     LSQ = 277,
     RSQ = 278,
     LCURLY = 279,
     RCURLY = 280,
     COMMA = 281,
     SEMI = 282,
     ID = 283,
     NUM = 284,
     ERROR = 285
   };
#endif
/* Tokens.  */
#define IF 258
#define ELSE 259
#define INT 260
#define RETURN 261
#define VOID 262
#define WHILE 263
#define ASSIGN 264
#define EQ 265
#define LT 266
#define LTEQ 267
#define GT 268
#define GTEQ 269
#define NEQ 270
#define PLUS 271
#define MINUS 272
#define TIMES 273
#define OVER 274
#define LPAREN 275
#define RPAREN 276
#define LSQ 277
#define RSQ 278
#define LCURLY 279
#define RCURLY 280
#define COMMA 281
#define SEMI 282
#define ID 283
#define NUM 284
#define ERROR 285




#if ! defined YYSTYPE && ! defined YYSTYPE_IS_DECLARED
typedef int YYSTYPE;
# define YYSTYPE_IS_TRIVIAL 1
# define yystype YYSTYPE /* obsolescent; will be withdrawn */
# define YYSTYPE_IS_DECLARED 1
#endif

extern YYSTYPE yylval;



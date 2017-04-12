/*
** CIS*4650 A1 - scan.h
** Desc: Header file which helps facilitate scanner.l
** Name: Daniel Girotto
** ID: 0783831
** Date: February 4, 2016
*/

#ifndef _SCAN_H_
#define _SCAN_H_

/* MAXTOKENLEN is the maximum size of a token */
#define MAXTOKENLEN 40

/* tokenString array stores the lexeme of each token */
extern char tokenString[MAXTOKENLEN+1];

/* returns the next token in source file */
TokenType getToken(void);

#endif


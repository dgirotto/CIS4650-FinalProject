/*
** CIS*4650 A1 - util.h
** Desc: Contains the function definitions of the program's utility functions (util.c)
** Name: Daniel Girotto
** ID: 0783831
** Date: February 4, 2016
*/

#ifndef _UTIL_H_
#define _UTIL_H_

/* normalize - makes each of the passed tag's characters upper-case.
(i.e. heLLo becomes HELLO)
*/
char* normalize(char*);

/* checkRelevant - checks if the passed string (tag) matches any
of the "relevant" tags described in "main.c". Returns 1 if the passed
tag is relevant (match is found); 0 otherwise
*/
int checkRelevant(char*);

/* stripTag - receives either an open or close tag in its original form
(<TAG> or </TAG>) and strips the string of its brackets (i.e. <TAG> becomes TAG)
PARAMS:
        toStrip: pointer to the tag to be "stripped"
        type=0: strip open tag of < and >
        type=1: strip close tag of </ and >
*/
char* stripTag(char*,int);

#endif

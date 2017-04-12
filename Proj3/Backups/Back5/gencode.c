#include "y.tab.h"
#include "globals.h"
#include "gencode.h"
#define CODESIZE 100
static int tempCtr=0;

/* allocates memory for a new temporary variable */
char* newTemp(){
	char* temp = malloc(10);
	sprintf(temp,"t%d",tempCtr);
	tempCtr++;
	return temp;
}

void emitCode(char * codestr){
	printf("%s\n",codestr);
}

void genCode(TreeNode * theNode){
	char codestr[CODESIZE];
        while(theNode != NULL){
                switch(theNode->nodekind){
                        case StmtK:
                                switch(theNode->kind.stmt){
                                        case IfK:
                                                break;
                                        case WhileK:
                                                break;
                                        case ReturnK:
                                                break;
                                        case CmpdK:
						genCode(theNode->child[0]);
						genCode(theNode->child[1]);
                                                break;
                                        case AssignK:
						genCode(theNode->child[1]); /* call genCode() on right child */
						theNode->strval = theNode->child[0]->attr.name;
printf("theNode->strval is %s\n",theNode->child[0]->attr.name);
						sprintf(codestr,"%s = %s",theNode->child[0]->attr.name,theNode->child[1]->strval);
						emitCode(codestr);
                                                break;
                                        default:
                                                printf("Invalid Stmt kind!\n");
                                                break;
                                }
                                break;
                        case ExpK:
                                switch(theNode->kind.exp){
                                        case OpK:
						switch(theNode->attr.op){
							case PLUS:
printf("inside PLUS\n");
								genCode(theNode->child[0]);
								genCode(theNode->child[1]);
								theNode->strval = newTemp();
								sprintf(codestr,"%s = %s + %s",theNode->strval,theNode->child[0]->attr.name,theNode->child[1]->attr.name);
								emitCode(codestr);
								break;
							case MINUS:
								genCode(theNode->child[0]);
								genCode(theNode->child[1]);
								theNode->strval = newTemp();
								sprintf(codestr,"%s = %s - %s",theNode->strval,theNode->child[0]->strval,theNode->child[1]->strval);
								break;
							case TIMES:
								genCode(theNode->child[0]);
								genCode(theNode->child[1]);
								theNode->strval = newTemp();
								sprintf(codestr,"%s = %s * %s",theNode->strval,theNode->child[0]->strval,theNode->child[1]->strval);
								break;
							case OVER:
								genCode(theNode->child[0]);
								genCode(theNode->child[1]);
								theNode->strval = newTemp();
								sprintf(codestr,"%s = %s / %s",theNode->strval,theNode->child[0]->strval,theNode->child[1]->strval);
								break;
							default:
								printf("Invalid attr.op\n");
								break;
						}	
                                                break;
                                        case ConstK:
                                                break;
                                        case IdK:
printf("THE ID IS %s\n",theNode->attr.name);

                                                break;
                                        case CallK:
						genCode(theNode->child[0]);
                                                break;
                                        case SubsK:
                                                break;
                                        default:
                                                printf("Invalid Exp kind!\n");
                                                break;
                                }
                                break;
                        case DecK:
                                switch(theNode->kind.dec){
                                        case VarK:
                                                break;
                                        case FunK:
						genCode(theNode->child[1]);
						genCode(theNode->child[2]);
                                                break;
                                        case ParamK:
                                                break;
                                        default:
                                                printf("Invalid Dec kind!\n");
                                                break;
                                }
                                break;
                        default:
                                printf("Invalid node kind: %d \n",theNode->nodekind);
                                break;
                }
                theNode = theNode->sibling;
        }


}

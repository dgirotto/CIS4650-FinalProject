#include "y.tab.h"
#include "globals.h"
#include "gencode.h"

#define CODESIZE 100
static int tempCtr=0, labelCtr=0;

static int codeCtr=0;
static char * theCode[100];

/* allocates memory for a label */
char* genLabel(void){
	char* newL = malloc(10);
	sprintf(newL,"lab%d",labelCtr);
	labelCtr++;
	return newL;
}

/* allocates memory for a new temporary variable */
char* newTemp(void){
	char* temp = malloc(10);
	sprintf(temp,"t%d",tempCtr);
	tempCtr++;
	return temp;
}

void emitCode(char * codestr){
	int i;
	codeCtr++;
	theCode[codeCtr] = codestr;
	for(i=0; i<codeCtr; i++){
		printf("%s\n",theCode[i]);
	}
}

void genCode(TreeNode * theNode, char * label){
	char *lab1=NULL, *lab2=NULL;
	char codestr[CODESIZE];

        while(theNode != NULL){
                switch(theNode->nodekind){
                        case StmtK:
                                switch(theNode->kind.stmt){
                                        case IfK:
						genCode(theNode->child[0],label);
						lab1 = genLabel();

						if(theNode->child[0]->attr.val == 0){ //FIX THIS
							sprintf(codestr,"if_false false goto %s",lab1);
						}
						else{
							sprintf(codestr,"if_false true goto %s",lab1);
						}
						emitCode(codestr);
						genCode(theNode->child[1],label);
						
						if(theNode->child[2] != NULL){
							lab2 = genLabel();
							sprintf(codestr,"goto %s",lab2);
							emitCode(codestr);
						}

						sprintf(codestr,"label %s",lab1);
						emitCode(codestr);

						if(theNode->child[2] != NULL){
							genCode(theNode->child[2],label);
							sprintf(codestr,"label %s",lab2);
							emitCode(codestr);
						}
                                                break;
                                        case WhileK:
						lab1 = genLabel();
						sprintf(codestr,"label %s",lab1);
						emitCode(codestr);
						genCode(theNode->child[0],label);
						lab2 = genLabel();

						if(theNode->child[0]->attr.val == 0){ // FIX THIS
							sprintf(codestr,"if_false false goto %s",lab2);				
						}			
						else{
							sprintf(codestr,"if_false true goto %s",lab2);
						}

						emitCode(codestr);
						genCode(theNode->child[1],lab2);
						sprintf(codestr,"goto %s",lab1);
						emitCode(codestr);
						sprintf(codestr,"label %s",lab2);
						emitCode(codestr);
                                                break;
                                        case ReturnK:
                                                break;
                                        case CmpdK:
						genCode(theNode->child[0],NULL);
						genCode(theNode->child[1],NULL);
                                                break;
                                        case AssignK:
						genCode(theNode->child[1],NULL); /* call genCode() on right child */
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
								genCode(theNode->child[0],NULL);
								genCode(theNode->child[1],NULL);
								theNode->strval = newTemp();
								sprintf(codestr,"%s = %s + %s",theNode->strval,theNode->child[0]->attr.name,theNode->child[1]->attr.name);
								emitCode(codestr);
								break;
							case MINUS:
								genCode(theNode->child[0],NULL);
								genCode(theNode->child[1],NULL);
								theNode->strval = newTemp();
								sprintf(codestr,"%s = %s - %s",theNode->strval,theNode->child[0]->strval,theNode->child[1]->strval);
								break;
							case TIMES:
								genCode(theNode->child[0],NULL);
								genCode(theNode->child[1],NULL);
								theNode->strval = newTemp();
								sprintf(codestr,"%s = %s * %s",theNode->strval,theNode->child[0]->strval,theNode->child[1]->strval);
								break;
							case OVER:
								genCode(theNode->child[0],NULL);
								genCode(theNode->child[1],NULL);
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
						genCode(theNode->child[0],NULL);
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
						genCode(theNode->child[1],NULL);
						genCode(theNode->child[2],NULL);
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

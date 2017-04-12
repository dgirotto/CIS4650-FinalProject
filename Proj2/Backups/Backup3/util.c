#include "globals.h"
#include "util.h"
#include "y.tab.h"

/* macros to increase/decrease indentation */
#define INDENT indentno+=2
#define UNINDENT indentno-=2
static indentno = 0;

static int fFlag=0, vFlag=0, pFlag=0; /* flags for function, variable, parameter */

static char * errors[100];
static int errLineNo[100];
static int errCtr=0;

int hash(char * key){
	int temp=0;
	int i=0;
	while(key[i] != '\0'){
		temp = ((temp << SHIFT) + key[i]) % MAXSIZE;
		i++;
	}
	return temp;
}

void printErrors(void){
	int x;
	if(errCtr>0){
		for(x=0;x<errCtr;x++){
			fprintf(listing,"Error on line (%d) : %s\n",errLineNo[x],errors[x]);
		}
	}
	else{
		fprintf(listing,"No errors to report!\n");
	}
}

void addErrMsg(char* errMsg, int lineno){
	errors[errCtr] = errMsg;
	errLineNo[errCtr] = lineno;
	errCtr++;
}

static void printSpaces(void){
        int i;
        for(i=0;i<indentno;i++){
                fprintf(listing," ");
        }
}

void printTree( TreeNode * tree ){
	int i;
	INDENT;
	while(tree != NULL){
		printSpaces();
		fprintf(listing,"Current node kind: %d\n",tree->nodekind);
		if(tree->nodekind==StmtK){
			switch (tree->kind.stmt){
				case IfK:
          				fprintf(listing,"If\n");
          				break;
				case WhileK:
					fprintf(listing,"While\n");
					break;
				case ReturnK:
					fprintf(listing,"Returns ");
					if(tree->child[0]==NULL){
					   fprintf(listing,"nothing.");
					}
					fprintf(listing,"\n");
					break;
				case CmpdK:
					fprintf(listing,"Compound Statement...\n");
					break;
				case AssignK:
					fprintf(listing,"Assign:\n");
					break;
				default:
					fprintf(listing,"Unknown ExpNode kind\n");
					break;
			}
		}
		else if (tree->nodekind==ExpK){
			switch (tree->kind.exp) {
				case OpK:
					fprintf(listing,"Op: ");
					printToken(tree->attr.op,"\0");
					break;
				case ConstK:
					if(fFlag==1){
						if(tree->attr.val==0){
							fprintf(listing,"Function returns VOID\n");
						}
						else if(tree->attr.val==1){
							fprintf(listing,"Function returns INT\n");
						}
						else{
							fprintf(listing,"CONST OUT OF BOUNDS\n");
						}
						fFlag=0;
					}
                                        else if(pFlag==1){
                                                if(tree->attr.val==0){
                                                        fprintf(listing,"Type: VOID\n");
                                                }
                                                else if(tree->attr.val==1){
                                                        fprintf(listing,"Type: INT\n");
                                                }
                                                else{
                                                        fprintf(listing,"CONST OUT OF BOUNDS\n");
                                                }
                                                pFlag=0;
                                        }

					else if(vFlag==1){
                                                if(tree->attr.val==0){
                                                        fprintf(listing,"Type: VOID\n");
                                                }
                                                else if(tree->attr.val==1){
                                                        fprintf(listing,"Type: INT\n");
                                                }
                                                else{
                                                        fprintf(listing,"CONST OUT OF BOUNDS\n");
                                                }
						vFlag=0;
					}
					else{
						fprintf(listing,"Const: %d\n",tree->attr.val);
					}
					break;
				case IdK:
					fprintf(listing,"Id: %s\n",tree->attr.name);
					break;
				case CallK:
					//fprintf(listing,"Calling %s\n",tree->attr.name);
					break;
				case SubsK:
					// FIX THIS
					fprintf(listing,"Subs:\n");
					break;
				default:
					fprintf(listing,"Unknown ExpNode kind\n");
					break;
			}
		}
		else if (tree->nodekind==DecK){

			printf("**** kind.dec is : %d\n",tree->kind.dec);
			switch(tree->kind.dec){

				case VarK:
					fprintf(listing,"Var: %s\n",tree->attr.name);
					vFlag=1;
					break;
                                case FunK:
                                        fprintf(listing,"Function: %s\n",tree->attr.name);
					if(tree->child[0]->attr.val == 1){
						fprintf(listing,"Returns an INT\n");
					}
					else{ // == 0
						fprintf(listing,"Returns VOID\n");
					}
					fprintf(listing," returns %s",tree->child[0]->attr.name);
					fFlag=1;
                                        break;
                                case ParamK:
                                        fprintf(listing,"Parameter(s): %s\n",tree->attr.name);
					if(tree->attr.val==0){
						fprintf(listing,"None.\n");
					}
					else{
						if(tree->child[1]==NULL){
							fprintf(listing,"Param is an array\n");
						}
						else{
							fprintf(listing,"Param is not an array\n");
						}
					}
					pFlag=1;
                                        break;
				default:
					fprintf(listing,"Unknown DecNode kind\n");
					break;
			}
		}
		else{
			fprintf(listing,"Unknown node kind\n");
		}
		for (i=0;i<MAXCHILDREN;i++){
			printTree(tree->child[i]);
		}
		tree = tree->sibling;
	}
	UNINDENT;
}


/* allocates and makes a new copy of an existing string */
char* copyString(char* toCopy){
	int n;
	char* newString;

	if(toCopy==NULL){
		return NULL;
	}
	n = strlen(toCopy)+1;
	newString = malloc(n);

	if(newString==NULL){
		fprintf(listing,"Out of memory error at line %d\n",lineno);
	}	
	else{
		strcpy(newString,toCopy);
	}
	return newString;
}

/* creates a new declaration node for syntax tree construction */
TreeNode* newDecNode(DecKind kind){
	TreeNode* t = (TreeNode*)malloc(sizeof(TreeNode));
	int i;
	if(t==NULL){
		fprintf(listing,"Out of memory! Error at line: %d\n",lineno);
	}
	else{
		for(i=0;i<MAXCHILDREN;i++){
			t->child[i]=NULL;
		}
		t->sibling = NULL;
		t->nodekind = DecK;
		t->kind.dec = kind;
		t->lineno = lineno;
	}
	return t;
}


/* creates a new expression node for syntax tree construction */
TreeNode* newExpNode(ExpKind kind){
	TreeNode* t = (TreeNode*)malloc(sizeof(TreeNode));
	int i;
	if(t==NULL){
		fprintf(listing,"Out of memory! Error at line: %d\n",lineno);
	}
	else{
		for(i=0;i<MAXCHILDREN;i++){
			t->child[i]=NULL;
		}
		t->sibling = NULL;
		t->nodekind = ExpK;
		t->kind.exp = kind;
		t->lineno = lineno;
	}
	return t;
}

/* creates a new statement node for syntax tree construction */
TreeNode* newStmtNode(StmtKind kind){
	TreeNode* t = (TreeNode*)malloc(sizeof(TreeNode));
	int i;
	if(t==NULL){
		fprintf(listing,"Out of memory! Error at line: %d\n",lineno);
	}
	else{
		for(i=0;i<MAXCHILDREN;i++){
			t->child[i]=NULL; /* set each of the node's children to NULL */
		}
		t->sibling = NULL;
		t->nodekind = StmtK; /* WHERE IS StmtK STORED ??? */
		t->kind.stmt = kind;
		t->lineno = lineno;
	}
	return t;
}

/* prints a token and its lexeme to the listing file */
void printToken( int token, const char* tokenString ){ 
	switch (token){ 
		case IF:
		case ELSE:
		case INT:
		case RETURN:
		case VOID:
		case WHILE:
			fprintf(listing,"reserved word: %s\n",tokenString);
			break;
		case ASSIGN: fprintf(listing,"=\n"); break;
		case EQ: fprintf(listing,"==\n"); break;
		case LT: fprintf(listing,"<\n"); break;
		case LTEQ: fprintf(listing,"<=\n"); break; 
		case GT: fprintf(listing,">\n"); break;
		case GTEQ: fprintf(listing,">=\n"); break;
		case NEQ: fprintf(listing,"!=\n"); break;
		case LPAREN: fprintf(listing,"(\n"); break;
		case RPAREN: fprintf(listing,")\n"); break;
		case LSQ: fprintf(listing,"[\n"); break;
		case RSQ: fprintf(listing,"]\n"); break;
		case LCURLY: fprintf(listing,"{\n"); break;
		case RCURLY: fprintf(listing,"}\n"); break;
		case COMMA: fprintf(listing,",\n"); break;
		case SEMI: fprintf(listing,";\n"); break;
		case PLUS: fprintf(listing,"+\n"); break;
		case MINUS: fprintf(listing,"-\n"); break;
		case TIMES: fprintf(listing,"*\n"); break;
		case OVER: fprintf(listing,"/\n"); break;
		case 0: fprintf(listing,"EOF\n"); break;
		case NUM: fprintf(listing,"NUM, val= %s\n",tokenString); break;
		case ID: fprintf(listing,"ID, name= %s\n",tokenString); break;
		case ERROR: fprintf(listing,"ERROR: %s\n",tokenString); break;
		default: /* should never happen */
			fprintf(listing,"Unknown token: %d\n",token);
	}
}




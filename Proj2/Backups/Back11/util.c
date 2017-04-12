
#include "globals.h"
#include "util.h"
#include "y.tab.h"

/* macros to increase/decrease indentation */
#define INDENT indentno+=2
#define UNINDENT indentno-=2
static indentno = 0;

static int fFlag=0, vFlag=0, pFlag=0; /* flags for function, variable, parameter */
static int f_flag=0;
static int scopeDepth = 0; // 0 = global 
static int varType; // 0 = void, 1 = int, 2 = int []

static char * errors[100];
static int errLineNo[100];
static int errCtr=0;

static Entry * hashTable[MAXSIZE]; /* the hash table which stores each of the symbols */

void postorder(TreeNode* theNode){
	while(theNode != NULL){
		printf("the nodekind : %d\n",theNode->nodekind);
		switch(theNode->nodekind){
			case StmtK:
				switch(theNode->kind.stmt){
					case IfK:
						printf("IfK");
						break;
						
					case WhileK:
						printf("WhileK");
						break;
						
					case ReturnK:
						printf("ReturnK");
						break;
						
					case CmpdK:
						printf("CmpdK");
						if(f_flag){
							f_flag=0;
						}
						else{
							scopeDepth++;
						}
						postorder(theNode->child[0]);
						postorder(theNode->child[1]);
						deleteScope(scopeDepth);
						scopeDepth--;
						break;
						
					case AssignK:
						printf("AssignK");
						break;
						
					default:
						printf("Invalid Stmt kind!\n");
						break;
				}
				break;
			case ExpK:
				switch(theNode->kind.exp){
					case OpK:
						printf("OpK");
						break;
						
					case ConstK:
						printf("ConstK");
						break;
						
					case IdK:
						printf("IdK");
						if(lookup(theNode->attr.name,scopeDepth) == NULL){
							insert(theNode->attr.name,theNode,scopeDepth);
						}
						else{
							printf("%s already exists in hash table!\n",theNode->attr.name);
							//report error
						}
						break;
						
					case CallK:
						printf("CallK");
						if(lookup(theNode->attr.name,scopeDepth) == NULL){
							insert(theNode->attr.name,theNode,scopeDepth);
						}
						else{
							printf("%s already exists in hash table!\n",theNode->attr.name);
							//report error
						}
						break;
						
					case SubsK:
						printf("SubsK");
						break;
						
					default:
						printf("Invalid Exp kind!\n");
						break;
				
				}
				break;
			case DecK:
				switch(theNode->kind.dec){
					case VarK:
printf("MADE IT HEREEEEEE\n");						
						// check simple var vs array var
						// and update the symbol table
						if(lookup(theNode->attr.name,scopeDepth) == NULL){
                                                        if(theNode->child[1] != NULL){ //parameter is an array
                                                                theNode->type=2;
								theNode->size = theNode->child[1]->attr.val;
printf("Variable is an array of size %d\n",theNode->child[1]->attr.val);
                                                        }
                                                        else{
                                                                theNode->type=1;
                                                        }
							insert(theNode->attr.name,theNode,scopeDepth);
						}
						else{
							printf("%s already exists in hash table!\n",theNode->attr.name);
							//report error
						}
						break;
						
					case FunK:
						printf("FunK");
						f_flag=1;
						//add function name and definition to symbol table (use theNode->type to store its type)
						//simply access theNode->child[0] to get the function's type
						theNode->type = theNode->child[0]->attr.val;
						if(lookup(theNode->attr.name,scopeDepth) == NULL){
							insert(theNode->attr.name,theNode,scopeDepth);
						}
						else{
							printf("%s already exists in hash table!\n",theNode->attr.name);
							//report error
						}
						scopeDepth++;
						//call postorder() on the node's other two children
						postorder(theNode->child[1]);
						postorder(theNode->child[2]);
						break;
						
					case ParamK:
						printf("ParamK");
						if(lookup(theNode->attr.name,scopeDepth) == NULL){
							/* determine if the parameter is an array or not */
							if(theNode->child[1] != NULL){ //parameter is an array
								theNode->type=2;
							}
							else{
								theNode->type=1;
							}
							insert(theNode->attr.name,theNode,scopeDepth);
						}
						else{
							printf("%s already exists in hash table!\n",theNode->attr.name);
							//report error
						}

						break;
					default:
						printf("Invalid Exp kind!\n");
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


void display(void){
	int i = 0;
	Entry * iter = NULL;
	for(i=0;i<MAXSIZE;i++){
		iter = hashTable[i];
		printf("hashTable[%d]: ",i);
		while(iter!=NULL){
			printf("(%s depth: %d) ",iter->name,iter->depth);
			//printf("(%s depth: %d  type: %d) ",iter->name,iter->depth,iter->type);
			iter = iter->next;
		}
		printf("\n");
	}
}

int insert(char* name, TreeNode* theTree, int depth){
	int newIndex = hash(name);
	Entry* iter = NULL;
	//create an Entry node and copy the data over
	Entry* newEntry = malloc(sizeof(Entry));

	newEntry->name = malloc(strlen(name));
	strcpy(newEntry->name,name);
	newEntry->next = NULL;
	newEntry->depth = depth;

	//store the new node into the hash table
	iter = hashTable[newIndex];
	if(iter != NULL){
		newEntry->next = iter;
	}
	hashTable[newIndex] = newEntry;
	return 1;
}


Entry* lookup(char* name, int scopeDepth){
	Entry * iter = NULL;
	int index = hash(name);
	iter = hashTable[index];
	
	printf("Looking up %s with depth: %d\n",name,scopeDepth);

	while(iter!=NULL){
		if((strcmp(name,iter->name)==0) && (iter->depth == scopeDepth)){
			printf("found a match!\n");
			return iter;
		}
		iter = iter->next;
	}
	printf("could not find a match!\n");
	return NULL;
}


/* remove all symbols in the symbol table at the given scope */
void deleteScope(int depth){
	Entry * iter = NULL;
	Entry * temp = NULL;
	int i;
	printf("inside delete scope");
	for(i=0; i<MAXSIZE; i++){
		iter = hashTable[i];
		if(iter!=NULL){
			/* handle the case where the node to be deleted is the first node in the list */
			if(iter->depth == depth){
				hashTable[i] = iter->next;
			}		

			temp = iter;	
			iter = iter->next;

			while(iter != NULL){
				if(iter->depth == depth){
					temp->next = iter->next;
				}
				temp = iter;
				iter = iter->next;
			}
		}
	}
}


/* return 1 if delete is successful; 0 otherwise */	
int delete(char* name){
	Entry * iter = NULL;
	Entry * temp = NULL;
	int index = hash(name);

        iter = hashTable[index];
        if(strcmp(name,iter->name)==0){
                hashTable[index] = iter->next;
                return 1;
        }

        temp = iter;
        iter = iter->next;

	while(iter!=NULL){
		if(strcmp(name,iter->name)==0){
			temp->next = iter->next;
			return 1; //successful delete
		}
		temp = iter;
		iter = iter->next;
	}
	printf("Could not delete node!\n");
	return 0; //unsuccessful delete
}


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
			switch(tree->kind.dec){

				case VarK:
					fprintf(listing,"Var: %s\n",tree->attr.name);
					if(tree->child[1] != NULL){
						printf("Var is an array of size %d\n",tree->child[1]->attr.val);
					}
					else{
						printf("tree->child[1] is NULL\n");
					}
					vFlag=1;
					break;
                                case FunK:
                                        fprintf(listing,"Function: %s\n",tree->attr.name);
/*
					if(tree->child[0]->attr.val == 1){
						fprintf(listing,"Returns an INT\n");
					}
					else{ // == 0
						fprintf(listing,"Returns VOID\n");
					}
					fprintf(listing," returns %s",tree->child[0]->attr.name);
*/
					fFlag=1;
                                        break;
                                case ParamK:
                                        fprintf(listing,"Parameter(s): %s\n",tree->attr.name);
/*
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
*/
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




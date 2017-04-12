#include "y.tab.h"
#include "globals.h"
#include "gencode.h"

//INSTRUCTION iMem[IADDR_SIZE];
int dMem[DADDR_SIZE];
int reg[NO_REGS];

void genStmt(TreeNode* theNode, int offset){
	if(theNode != NULL){
		switch(theNode->kind.stmt){
			case IfK:
				break;
			case WhileK:
				break;
			case ReturnK:
				break;
			case CmpdK:
				break;
			case AssignK:
				break;
			default:
				printf("Invalid Stmt kind!\n");
				break;
		}
	}


}

void genExp(TreeNode* theNode, int offset, int isAddress){
	if(theNode != NULL){                        
		switch(theNode->kind.exp){
			case OpK:
				switch(theNode->attr.op){
					case PLUS:
						break;
					case MINUS:
						break;
					case TIMES:
						break;
					case OVER:
						break;
					default:
						printf("Invalid attr.op\n");
						break;
				}
				break;
			case ConstK:
				break;
			case IdK:
				break;
			case CallK:
				break;
			case SubsK:
				break;
			default:
				printf("Invalid Exp kind!\n");
				break;
		}
	}
}

void genDec(TreeNode* theNode, int offset){
	if(theNode != NULL){
		switch(theNode->kind.dec){
			case VarK:
				break;
			case FunK:
				break;
			case ParamK:
				break;
			default:
				printf("Invalid Dec kind!\n");
				break;
		}
	}

}

void cGen(TreeNode * theNode, int offset, int isAddress){
        while(theNode != NULL){
                switch(theNode->nodekind){
                        case StmtK:
				genStmt(theNode,offset);
                                break;
                        case ExpK:
				genExp(theNode,offset,isAddress);
                                break;
                        case DecK:
				genDec(theNode,offset);
                                break;
                        default:
                                printf("BUG. Invalid node kind in cGen: %d \n",theNode->nodekind);
                                break;
                }
                theNode = theNode->sibling;
        }


}

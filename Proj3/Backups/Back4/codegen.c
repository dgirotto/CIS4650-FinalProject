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
                                                break;
                                        case AssignK:
                                                break;
                                        default:
                                                printf("Invalid Stmt kind!\n");
                                                break;
                                }
                                break;
                        case ExpK:
                                switch(theNode->kind.exp){
                                        case OpK:
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
                                break;
                        case DecK:
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
                                break;
                        default:
                                printf("Invalid node kind: %d \n",theNode->nodekind);
                                break;
                }
                theNode = theNode->sibling;
        }


}

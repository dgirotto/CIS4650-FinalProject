#include <stdio.h>
#include <stdlib.h>
#include <string.h>

//12-12:20 security proposal presentation

typedef struct treeNode{
	char * data;
	struct treeNode* left;
	struct treeNode* right;
}TreeNode;


TreeNode* createTree(){
	TreeNode * newTree = malloc(sizeof(TreeNode));
	newTree->data = NULL;
	newTree->left = NULL;
	newTree->right = NULL;
	return newTree;
}

TreeNode* createNode(char* theData){
	TreeNode * newNode = malloc(sizeof(TreeNode));
	newNode->data = malloc(sizeof(char)*strlen(theData));
	strcpy(newNode->data,theData);
	newNode->left = NULL;
	newNode->right = NULL;
	return newNode;
}
int main(void){
	TreeNode * newTree;
	newTree = createTree();
return 0;
}

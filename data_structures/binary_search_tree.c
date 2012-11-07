/* 
 Ryley Herrington
 CS261 - WInter
 */
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <assert.h>
#define LINELENGTH 100
typedef char * EleType;

struct node {
	char * value;
	struct node * left;
	struct node * right;
};

struct BinarySearchTree {
	struct node * root;
	int size;
};
void BSTinit(struct BinarySearchTree *tree);
struct node * newNode (EleType newValue, struct node * l, struct node * r);
struct node * BSTnodeAdd (struct node *current, EleType newValue);
void BSTadd(struct BinarySearchTree *tree, EleType newValue);
int BSTsize (struct BinarySearchTree *tree);
int comp(struct node * current, EleType n);
int BSTcontains (struct BinarySearchTree *tree, EleType d);
struct node * BSTnodeRemove (struct node * current, EleType d);
void BSTremove (struct BinarySearchTree *tree, EleType d);
EleType leftMostChild (struct node * current);
struct node * removeLeftmostChild (struct node *current);

void BSTinit(struct BinarySearchTree *tree) {
	tree->size = 0; 
	tree->root = 0;
}
struct node * newNode (EleType newValue, struct node * l, struct node * r) {
	struct node * newnode = (struct node *) malloc(sizeof(struct node));
	assert(newnode != 0);
	newnode->value = newValue;
	newnode->left = l;
	newnode->right = r;
	return newnode;
}
struct node * BSTnodeAdd (struct node *current, EleType newValue) {
	if (current == 0){
		return newNode(newValue, 0,0);}
	if (strcmp(newValue, current->value)<=0){
		current->left = BSTnodeAdd(current->left, newValue);
	}
	else{
		current->right = BSTnodeAdd(current->right, newValue);
	}
	return current;
}

void BSTadd(struct BinarySearchTree *tree, EleType newValue) {
	tree->root = BSTnodeAdd(tree->root, newValue); 
	tree->size++; 
}

int BSTsize (struct BinarySearchTree *tree) { 
	return tree->size; 
}
int comp(struct node * current, EleType n){
	if(current ==0)
		return 0;   
	if (strcmp(current->value,n)==0){
		return 1;
		if (strcomp(n,current-> value)<=0){
			return comp(current->left, n);
		}
		else{
			return comp(current->right, n);
		}
	}
}

int BSTcontains (struct BinarySearchTree *tree, EleType d) {
	struct node * current = tree->root;
	current -> value = d;
	return comp(current, d);
}
struct node * BSTnodeRemove (struct node * current, EleType d) {
	if(strcmp(current->value, d)==0){ // if current->value == d
		if(current->right == 0){
			return current->left;
		}
		else{
			current->value = leftMostChild(current->right);
			current->right = removeLeftmostChild(current->right);
		}
	}
	else if(strcmp(d,current->value)<0){ // if d<currentval
		current->left = removeLeftmostChild(current->left);    
	}
	else{
		current->right = removeLeftmostChild(current->right);
	}
	return current;
} 

void BSTremove (struct BinarySearchTree *tree, EleType d) {
	if (BSTcontains(tree, d)){
		tree->root = BSTnodeRemove(tree->root, d);
		tree->size--;
	}
}

EleType leftMostChild (struct node * current) {
	while (current->left != 0){
		current = current->left;
	}
	return current->value;
}

struct node * removeLeftmostChild (struct node *current) {
	struct node * fullTree = current;
	if (current == 0){
		return 0;
	}
	struct node *  parent = 0;
	while(current->left != 0){ 
		parent = current;
		current = current->left;
	}
	if(parent == 0){
		return 0;
	}
	parent->left = current->right;
	free(current);
	return fullTree;
}



int count_tree(struct node * n){
	if (n == 0)
		return 0;
	return (count_tree(n->left)+count_tree(n->right)+1);
}

void visitANode(struct node * current, FILE * f){
	if (current == 0){
		return;
	}
	//preorder traversal because it's before the work happens
	char vtype = (!current->left && !current->right) ? 'A' : 'Q'; 
	fprintf(f,"%c%s\n",vtype,current->value);//figure out if there's a "New Line" value in the string

	visitANode(current->left,f);
	visitANode(current->right,f);

}

void writeATree(FILE * f, struct BinarySearchTree * tree){
	visitANode(tree->root, f);	
}
char * newStr (char * charBuffer){
	char * p = (char *) malloc(1 + strlen(charBuffer));
	strcpy (p, charBuffer);
	return p;
}
struct node * readATree (FILE * f){
	char c, buffer[LINELENGTH];
	struct node * newTree;
	c = fgetc(f);
	fgets(buffer, LINELENGTH, f);
	buffer[strlen(buffer)-1]='\0';
	newTree = newNode(newStr(buffer),0,0);
	if (c == 'Q'){
		newTree->left = readATree(f);
		newTree->right = readATree(f);
	}
	return newTree;
}


int main(int argc, char* argv[]){ 
	struct BinarySearchTree tree;
	BSTinit(&tree);

	/*Reading from a file*/
	FILE * f = fopen("animals.txt", "r");;
	char buffer[LINELENGTH];
	if (!f){
		BSTadd(&tree,"cat");
	}
	else{
		tree.root = readATree(f);
		tree.size = count_tree(tree.root);
		fclose(f);
	}

	struct node * current = tree.root;
	while(current){
		if(current->left || current->right){
			printf("%s\n", current->value);
			fgets(buffer,sizeof(buffer),stdin);
			if(toupper(buffer[0]) == 'Y')
				current = current->left;
			else{
				assert(toupper(buffer[0])=='N');
				current=current->right;
			}
		}
		else{ //if(!current->left && !current->right)
			printf("Is %s the animal?\n", current->value);
			fgets(buffer, sizeof(buffer),stdin);
			buffer[strlen(buffer)-1]='\0';
			if(toupper(buffer[0]) == 'Y'){
				printf("I WIN!!\n");
	                	current = tree.root;   // restart game at the top of the tree
			}
			else{
				printf("I'm sorry, I don't know your animal.\n"
						"What animal were you thinking of?\n");
				fgets(buffer, sizeof(buffer),stdin);
				buffer[strlen(buffer)-1]='\0';
				current->right = newNode(current->value,0,0);
				current->left = newNode(strdup(buffer), 0,0);
				printf("Please enter a yes/no question which is true of a %s but not true of a %s?\n",current->left->value, current->right->value);
				fgets(buffer, sizeof(buffer),stdin);
				buffer[strlen(buffer)-1]='\0';
				printf("Thank you!\n");
				current->value = strdup(buffer);
				f = fopen("binary_search_tree.txt", "w");;
				assert(f);
				writeATree(f,&tree);
				fclose(f);
				exit(0);
			}
		}
	}
	return 0;
}

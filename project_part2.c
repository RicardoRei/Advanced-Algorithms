#include <stdio.h>
#include <stdlib.h>

typedef struct LCT *LCT;

/* Represents a node in the LCT. */
struct LCT {
	/* Left Child */
	LCT left;    
	/* Right Child */
  	LCT right;   
  	/* General Parent Pointer. */
	LCT* hook;   

	/* The size of this sub-tree.*/
  	int sum; /* Negative values are used to indicate that left and right pointers should be swapped
                on the sub-tree. */ 
};


LCT allocLct(int V)
{
	int i;
	LCT nodes = (LCT) malloc(sizeof(LCT)*V);
	for( i = 0; i < V; i++)
	{
		nodes[i].left = NULL;
		nodes[i].right = NULL;
		nodes[i].hook = NULL;
		nodes[i].sum = 0;
	}
	return nodes;
}

void freeLCT(LCT t)
{
	free(t);
}

void rotateRight(LCT node)
{
	LCT parent = node->hook[0];
	parent->left = node->right;
	
	if (parent->hook == NULL)
	{	
		free(node->hook);
		parent->hook = (LCT*) malloc(sizeof(LCT));
		node->hook = NULL;
	}
	else
	{
		node->hook[0] = parent->hook[0];
		node->hook[0]->left= node;
	}
	
	parent->hook[0] = node;
	node->right = parent;
}

void rotateLeft(LCT node)
{
	LCT parent = node->hook[0];
	parent->right = node->left;
	
	if (parent->hook == NULL)
	{
		free(node->hook);
		parent->hook = (LCT*) malloc(sizeof(LCT));
		node->hook = NULL;
	}
	else
	{
		node->hook[0] = parent->hook[0];
		node->hook[0]->left = node;
	}
		

	parent->hook[0] = node;
	node->left = parent;
}


void visit(LCT h)
{
	printf("%d\t", h->sum);
}

void traverse(LCT h)
{
	if (h == NULL)
	 	return;

	visit(h);
	traverse(h->left);
	traverse(h->right);
	
}

LCT insertTree(LCT h, LCT new_node)
{	
	if (h == NULL)
	{
		h = new_node;
		return h;
	}

	if (new_node->sum < h->sum)
		h->left = insertTree(h->left, new_node);
	else 
		h->right = insertTree(h->right, new_node);

	return h;
}

LCT splayingStep(LCT node)
{
	LCT parent = node->hook[0];
	LCT g_parent = NULL; 

	/* Case 1 ZIG */
	if (parent->hook == NULL && (parent->right == node))
	{
		rotateLeft(node);
		return node;
	}
	else if (parent->hook == NULL && (parent->left == node))
	{
		rotateRight(node);
		return node;	
	}

	/* Case 2 ZIG - ZIG */
	g_parent = parent->hook[0];
	/* if both x and p(x) are left childs */
	if ((parent == g_parent->left) && (parent->left == node))
	{
		rotateRight(parent);
		rotateRight(node);
	}
	/* if both x and p(x) are right childs */
	else if ((parent == g_parent->right) && (parent->right == node))
	{
		rotateLeft(parent);
		rotateLeft(node);
	}

	/* Case 3 ZIG - ZAG */
	/* p(x) is a left child and x is a right child */
	else if ((parent == g_parent->left) && (parent->right== node))
	{
		rotateLeft(node);
		rotateRight(node);
	}
	/* p(x) is a right child and x is a left child */
	else if ((parent == g_parent->right) && (parent->left== node))
	{
		rotateLeft(node);
		rotateRight(node);
	}

	return node;
}

LCT splay(LCT node)
{
	while (node->hook != NULL) node = splayingStep(node);
	return node;
}

int main()
{
	LCT tree = NULL;
	LCT vec = allocLct(5);
	int i;

	for (i = 0; i < 5; i++)
		vec[i].sum = i;
		
	tree = insertTree(tree, &vec[3]);

	tree = insertTree(tree, &vec[4]);
	vec[4].hook = (LCT*) malloc(sizeof(LCT)); 
	vec[4].hook[0] = &vec[3];

	tree = insertTree(tree, &vec[1]);
	vec[1].hook = (LCT*) malloc(sizeof(LCT)); 
	vec[1].hook[0] = &vec[3];

	tree = insertTree(tree, &vec[0]);
	vec[0].hook = (LCT*) malloc(sizeof(LCT)); 
	vec[0].hook[0] = &vec[1];

	tree = insertTree(tree, &vec[2]);
	vec[2].hook = (LCT*) malloc(sizeof(LCT)); 
	vec[2].hook[0] = &vec[1];

	traverse(tree);
	printf("\n");

	splay(&vec[1]);
	tree = &vec[1];
	traverse(tree);
	printf("\n");

	
	tree = splay(&vec[4]);
	traverse(tree);
	printf("\n");
	
	tree = splay(&vec[3]);
	traverse(tree);
	printf("\n");

	tree = splay(&vec[2]);
	traverse(tree);
	printf("\n");

	tree = splay(&vec[3]);
	traverse(tree);
	printf("\n");

	tree = splay(&vec[0]);
	traverse(tree);
	printf("\n");
	
	return EXIT_SUCCESS;
}





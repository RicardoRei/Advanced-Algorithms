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
	LCT parent;
	/** Pointer within the auxiliary tree */
	LCT pathParent;   
	/* The size of this sub-tree.*/
  	int depth;
};

/*********************************************** NOTE ******************************************************/
/*  In this code i compare pointers with each others, this is only safe because in this project every node */
/*  belongs to the same array allocated in the begin of main. 											   */
/***********************************************************************************************************/

LCT allocLct(int V)
{
	int i;
	LCT nodes = (LCT) malloc(sizeof(LCT)*V);
	for( i = 0; i < V; i++)
	{
		nodes[i].left = NULL;
		nodes[i].right = NULL;
		nodes[i].parent = NULL;
		nodes[i].pathParent = NULL;
		nodes[i].depth = 0;
	}
	return nodes;
}

void freeLCT(LCT t)
{
	free(t);
}

void rotateRight(LCT node)
{
	LCT parent = node->pathParent;
	parent->left = node->right;

	if (node->right != NULL)
		node->right->pathParent = parent;

	node->pathParent = parent->pathParent;

	if (node->pathParent != NULL)
		node->pathParent->left = node;

	parent->pathParent = node;
	node->right = parent;
}

void rotateLeft(LCT node)
{
	LCT parent = node->pathParent;
	parent->right = node->left;
	if (node->left != NULL)
		node->left->pathParent = parent;

	node->pathParent = parent->pathParent;

	if (node->pathParent != NULL)
		node->pathParent->left = node;

	parent->pathParent = node;
	node->left = parent;
}


void visit(LCT h)
{
	printf("%d\t", h->depth);
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

	if (new_node->depth < h->depth)
		h->left = insertTree(h->left, new_node);
	else 
		h->right = insertTree(h->right, new_node);

	return h;
}

LCT splayingStep(LCT node)
{
	LCT parent = node->pathParent;
	LCT g_parent = NULL; 

	/* Case 1 ZIG */
	if (parent->pathParent == NULL && (parent->right == node))
	{
		rotateLeft(node);
		return node;
	}
	else if (parent->pathParent == NULL && (parent->left == node))
	{
		rotateRight(node);
		return node;	
	}

	/* Case 2 ZIG - ZIG */
	g_parent = parent->pathParent;
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
	while (node->pathParent != NULL) node = splayingStep(node);
	return node;
}

void access(LCT t, int v)
{
	LCT node = &t[v];
	LCT aux = NULL;

	splay(node);
	if (node->right != NULL)
	{
		node->right->pathParent = node;
		node->right->parent = NULL;
		node->right = NULL;
	}

	aux = node->pathParent;
	while ( aux !=  NULL)
	{
		splay(aux);
		if (aux->right != NULL)
		{
			aux->right->pathParent = aux;
			aux->right->parent = NULL;
		}

		aux->right = node;
		node->parent = aux;
		node->pathParent = NULL;
		splay(node);
	}	
}

void link(LCT t, int u, int v)
{
	access(t, u);
	access(t, v);
	t[u].left = &t[v];
	t[v].right = &t[u];
}

void cut(LCT t, int u, int v)
{
	/* first i check if its a valid edge. */
	if (t[u].left == &t[v])
	{
		access(t, v);
		t[v].left->parent = NULL; /* = t[u].left = NULL */
		t[v].left = NULL;
	}
}


int main()
{
	LCT vec = NULL;
	int size, u, v;
	char command;

	scanf("%d\n", &size);
	vec = allocLct(size);

	while ((command = getchar()) != 'X') /* reads the command and if its X exits the while cycle */
	{  
        getchar(); /* reads the space after the command and the \n of X command */

        switch (command) 
        {
	        case 'L':
	        	scanf("%d %d\n",&u, &v);
	        	link(vec, u-1, v-1);
	        	break;

	        case 'C':
	        	scanf("%d %d\n",&u, &v);
	        	cut(vec, u-1, v-1);
	        	break;

	 		default:
	            printf("ERROR: Unknown command %c\n", command);
	    }
    }

    if (vec != NULL) freeLCT(vec); 

	return EXIT_SUCCESS;
}





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

/************************************************** NOTE ********************************************************/
/*  In this code i compare pointers with each others, this is only safe because in this project every node      */
/*  belongs to the same array allocated in the begin of main. 											        */
/****************************************************************************************************************/

/************************************************ SIGNATURES ****************************************************/
LCT allocLct(int V);
void freeLCT(LCT t);

/* Splaying Tree funtions */
void rotateLeft(LCT node);
void rotateRight(LCT node);
LCT splayingStep(LCT node);
LCT splay(LCT node);

/* Link-Cut Trees functions*/
void access(LCT t, int v);
void link(LCT t, int u, int v);
void cut(LCT t, int u, int v);
int connectedQ(LCT t, int u, int v);
LCT findRoot(LCT t, int v);
void reRoot(LCT t, int v);

/* Auxiliar funtions */
void unflip(LCT node);
void printArray(LCT array, int size);
/*****************************************************************************************************************/


int main()
{
	LCT vec = NULL;
	int size, u, v, i;
	char command;

	scanf("%d\n", &size);
	vec = allocLct(size);

	while ((command = getchar()) != 'X') /* reads the command and if its X exits the while cycle */
	{  
        
        switch (command) 
        {
	        case 'L':
	        	scanf("%d %d",&u, &v);
	        	link(vec, u-1, v-1);
	        	break;

	        case 'C':
	        	scanf("%d %d",&u, &v);
	        	cut(vec, u-1, v-1);
	        	break;

	        case 'Q':
	        	scanf("%d %d", &u, &v);
	        	if (connectedQ(vec, u-1, v-1))
	        		printf("T\n");
	        	else 
	        		printf("F\n");
	        	break;

	        case 'T':
	        	printArray(vec, size);
	        	break;

	 		default:
	            printf("ERROR: Unknown command %c\n", command);
	    }

	    getchar(); /* reads the \n */
    }

    if (vec != NULL) freeLCT(vec); 

	return EXIT_SUCCESS;
}

/* @brief: Function to allocate an array of nodes of type LCT. 
 *		   Receives an integer which defines the number of nodes.
 */
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
		nodes[i].depth = 1;
	}
	return nodes;
}

/* @brief: Function to free the memory of the array representing the nodes. 
 *		   Receives a pointer to the begining of the array.
 */
void freeLCT(LCT t)
{
	free(t);
}

/* @brief: Function that rotates a specific node to the right. 
 *		   Receives a pointer to that node.
 */
void rotateRight(LCT node)
{
	LCT parent = node->parent;
	LCT g_parent = NULL;
	if (parent == NULL) return; /* node is already the root */

	g_parent = parent->parent;
	parent->left = node->right;

	if (node->right != NULL)
		node->right->parent = parent;

	node->right = parent;
	parent->parent = node;
	node->parent = g_parent;

	if (g_parent != NULL)
	{
		if (g_parent->right == parent) g_parent->right = node; 
		else g_parent->left = node;
	}
	

	node->pathParent = parent->pathParent;
	parent->pathParent = NULL;
}

/* @brief: Function that rotates a specific node to the left. 
 *		   Receives a pointer to that node.
 */
void rotateLeft(LCT node)
{
	LCT parent = node->parent;
	LCT g_parent = NULL;
	if (parent == NULL) return; /* node is already the root */

	g_parent = parent->parent;
	parent->right = node->left;

	if (node->left != NULL)
		node->left->parent = parent;

	node->left = parent;
	parent->parent = node;
	node->parent = g_parent;

	if (g_parent != NULL)
	{
		if (g_parent->left == parent) g_parent->left = node; 
		else g_parent->right = node;
	}	
	
	/* code part to maintain our path pointers */	
	node->pathParent = parent->pathParent;
	parent->pathParent = NULL;
}

/* @brief: Splays a node according to the definition of the splaying step from "Self Adjusting Binary Search Trees"
 *         from Sleator and Tarjan.
 * 
 *		   Receives a pointer to that node.
 */
LCT splayingStep(LCT node)
{
	LCT parent = node->parent;
	LCT g_parent = parent->parent; /* parent is never NULL because we only call this while parent != NULL */

	if (g_parent != NULL)
		unflip(g_parent);
	unflip(parent);
	unflip(node);

	/* Case 1 ZIG */
	if (g_parent == NULL && (parent->right == node))
	{
		rotateLeft(node);
		return node;
	}
	else if (g_parent == NULL && (parent->left == node))
	{
		rotateRight(node);
		return node;	
	}

	/* Case 2 ZIG - ZIG */
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

/* @brief: Performs several Splays to a node until the node is made the root of his aux tree.
 *		   Receives a pointer to that node.
 */
LCT splay(LCT node)
{
	if (node != NULL)
		while (node->parent != NULL) node = splayingStep(node);
	return node;
}


/* @brief: Function that accesses a node, this function should change the represented tree in order to made
 *         the path from the root to node v the prefered path.
 *
 *         Receives an array with all LCT nodes and a int v that represents the position of the node we want
 *         to access in that array.
 */
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

	while (node->pathParent !=  NULL)
	{
		aux = node->pathParent;
		splay(aux);
		if (aux->right != NULL)
		{
			aux->right->pathParent = aux;
			aux->right->parent = NULL;
		}

		aux->right = node;
		node->parent = aux;
		splay(node);
	}	
}

LCT findRoot(LCT t, int v)
{
	access(t, v);

	LCT current = &t[v];
	while (current->left != NULL)
		current = current->left;

	splay(current);
	return current;
}

/* @brief: Link entre dois nos. Nos papers e nos videos o link implica aceder aos dois nos primeiro. No entanto o
 *		   o codigo que encontrei no git nao faz isso.
 *         Receives an array with all LCT nodes, int u that represents the position of the node u and int v that 
 *         represents the position of the node v.
 */
 void link(LCT t, int v, int w)
{
	if (connectedQ(t, v, w))
		return;

	reRoot(t, v);
	access(t, v);
	access(t, w);
	t[w].left = &t[v];
	splay(&t[v]);
}

/* @brief: This function removes the edge (u, v) if there is one.
 *
 *         Receives an array with all LCT nodes, int u that represents the position of the node u and int v that 
 *         represents the position of the node v.
 */
void cut(LCT t, int u, int v)
{
	if (t[u].left == &t[v])
	{
		access(t, u);
		t[v].parent = NULL;
		t[v].pathParent = NULL; 
		t[u].left = NULL;
	}
}

/*      Receives an array with all LCT nodes, int u that represents the position of the node u and int v that 
 *      represents the position of the node v.
 */
int connectedQ(LCT t, int u, int v)
{
	LCT current;

	reRoot(t, u);
	access(t, v);
	if (findRoot(t, v) == &t[u])
		return 1;

	return 0;
}

void unflip(LCT node)
{
	LCT aux;

	if (node->depth != -1)
		return;

	aux = node->left;
	node->left = node->right;
	node->right = aux;
	node->right->depth = -1;
	node->left->depth = -1;
}

void reRoot(LCT t, int v)
{
	access(t, v);
	t[v].depth *= -1;
	access(t, v);
}

/*
 * @brief: This auxiliar function is usefull for debug purpose by printing the array of LCT.
 *
 * @param: array - The array with the LCT nodes.
 *         size - The size of the array.
 */
void printArray(LCT array, int size)
{
	int i;
	puts("");
	for (i = 0; i < size; i++)
		printf("address:%p left:%p right:%p parent:%p pathParent:%p flip_bit:%d\n", 
			   &array[i], 
			   array[i].left, 
			   array[i].right,
			   array[i].parent, 
			   array[i].pathParent,
			   array[i].depth);
	
}


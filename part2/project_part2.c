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
void inOrder(LCT node);
/*****************************************************************************************************************/


int main()
{
	LCT vec = NULL;
	int size, u, v;
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
	LCT nodes = (LCT) malloc(sizeof(struct LCT)*V);
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
void rotateRight(LCT y)
{
  	LCT t;
	printf("Antes rotateRight\n");
	inOrder(y);
	LCT x = y->left;
	LCT z = y->parent;

	if (z != NULL){
		if (z->right == y)
			z->right = x;
		else
			z->left = x;
    }
    t = x->right;
	x->right = y;
	y->left = t;

	x->parent = z;
	y->parent = x;

	if (y->left != NULL){
		y->left->parent = y;
	}

    if(NULL != z){	/* code part to maintain our path pointers */	
        y->pathParent = z->pathParent;
        z->pathParent = NULL;
    }

	printf("Depois rotateRight\n");
	inOrder(y);
}


/* @brief: Function that rotates a specific node to the left. 
 *		   Receives a pointer to that node.
 */
void rotateLeft(LCT y)
{
	LCT t;
	printf("Antes rotateLeft\n");
	inOrder(y);
	LCT x = y->right;
	LCT z = y->parent;

	if (z != NULL){
		if (z->left == y)
			z->left = x;
		else
			z->right = x;
	}
	t = x->left;
	x->left = y;
	y->right = t;
	
	x->parent = z;
	y->parent = x;

	if (y->right != NULL)
		y->right->parent = y;


	if(NULL != z){	/* code part to maintain our path pointers */	
        y->pathParent = z->pathParent;
        z->pathParent = NULL;
    }

	printf("Depois rotateLeft\n");
	inOrder(y);
}

void inOrder(LCT node){

	if (node->left)
		inOrder(node->left);

	printf("%p \n", (void *)node);
	if(node->right)
		inOrder(node->right); 

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
		printf("1\n");
		rotateLeft(parent);
		return node;
	}
	else if (g_parent == NULL && (parent->left == node))
	{
		printf("2\n");
		rotateRight(parent);
		return node;	
	}

	/* Case 2 ZIG - ZIG */
	/* if both x and p(x) are left childs */
	if ((parent == g_parent->left) && (parent->left == node))
	{
		printf("3\n");
		rotateRight(g_parent);
		rotateRight(parent);
	}

	/* Case 3 ZIG - ZAG */
	/* p(x) is a left child and x is a right child */
	else if ((parent == g_parent->left) && (parent->right== node))
	{	
		printf("4\n");
		rotateLeft(parent);
		rotateRight(g_parent);
	}

	/* if both x and p(x) are right childs */
	else if ((parent == g_parent->right) && (parent->right == node))
	{
		printf("5\n");
		rotateLeft(g_parent);
		rotateLeft(parent);
	}
	/* p(x) is a right child and x is a left child */
	else if ((parent == g_parent->right) && (parent->left== node))
	{	
		printf("6\n");
		rotateRight(parent);
		rotateLeft(g_parent);
	}

	return node;
}

/* @brief: Performs several Splays to a node until the node is made the root of his aux tree.
 *		   Receives a pointer to that node.
 */
LCT splay(LCT node)
{
	if (node != NULL)
		while (node->parent != NULL) 
			node = splayingStep(node);
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
	LCT current = &t[v];
	access(t, v);

	
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
	if (connectedQ(t, v, w)){
		printf("Trying to link connected nodes\n");
		return;
	}

	reRoot(t, v); /*not in teoricas, requested in enunciado*/
	access(t, v);
	access(t, w);
	t[v].left = &t[w]; 
	t[w].parent = &t[v];  
}

/* @brief: This function removes the edge (u, v) if there is one.
 *
 *         Receives an array with all LCT nodes, int u that represents the position of the node u and int v that 
 *         represents the position of the node v.
 */
void cut(LCT t, int v, int u)
{	
	if(connectedQ(t,v,u)){
		access(t, v); 
		t[v].left->parent = NULL;
		t[v].left = NULL;
	}
}

/*      Receives an array with all LCT nodes, int u that represents the position of the node u and int v that 
 *      represents the position of the node v.
 */

 
int connectedQ(LCT t, int u, int v)
{
	reRoot(t, u);
	access(t, v);

	if (findRoot(t, v) == &t[u]){
		return 1;
	}
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
	if (node->right != NULL)
		node->right->depth = -1;
	if (node->left != NULL)
		node->left->depth = -1;
}

void reRoot(LCT t, int v)
{
	access(t, v);
	t[v].depth *= -1;
	access(t, v);
}

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

#include <stdio.h>
#include <stdlib.h>
#include <time.h>

typedef struct LCT *LCT;

/* Represents a node in the LCT. */
struct LCT {
	/* Left Child */
	LCT left;    
	/* Right Child */
  	LCT right;   
  	/* General Parent Pointer. */
	LCT hook;
	/* The size of this sub-tree.*/
  	int sum;
};

/************************************************** NOTE ********************************************************/
/*  In this code we compare pointers with each others, this is only safe because in this project every node     */
/*  belongs to the same array allocated in the begin of main. 											        */
/****************************************************************************************************************/

/************************************************ SIGNATURES ****************************************************/
LCT allocLct(int V);
void freeLCT(LCT t);

/* Splaying Tree funtions */
void rotateLeft(LCT node);
void rotateRight(LCT node);
void splayingStep(LCT node);
void splay(LCT node);

/* Link-Cut Trees functions*/
void access(LCT t, int v);
void link(LCT t, int u, int v);
void cut(LCT t, int u, int v);
int connectedQ(LCT t, int u, int v);
void reRoot(LCT t, int v);

/* Auxiliar funtions */
void unflip(LCT node);
int checkPrefPath(LCT t, int u, int v);

/*****************************************************************************************************************/
int main()
{
	LCT vec = NULL;
	int size, u, v;
	int command;
	clock_t begin, end;
	double sum = 0;
	scanf("%d\n", &size);

	vec = allocLct(size);
	while ((command = getchar()) != 'X' && EOF != command) /* reads the command and if its X exits the while cycle */
	{  
        getchar(); /* reads space after command */
        switch (command) 
        {
	        case 'L':

	        	scanf("%d %d",&u, &v);

	        	begin = clock();
	        	link(vec, u-1, v-1);
	        	end = clock();

	        	sum += (double)(end - begin) / CLOCKS_PER_SEC;
	        	break;

	        case 'C':
	        	scanf("%d %d",&u, &v);

	        	begin = clock();
	        	cut(vec, u-1, v-1);
	        	end = clock();

	        	sum += (double)(end - begin) / CLOCKS_PER_SEC;
	        	break;

	        case 'Q':
	        	scanf("%d %d", &u, &v);
	        	(connectedQ(vec, u-1, v-1)) ? printf("T\n") : printf("F\n");
	        	break;
	    
	 		default:
	            printf("ERROR: Unknown command %d\n",(int)command);
	    }
	    getchar(); /* reads the \n */
    }

    if (vec != NULL) freeLCT(vec); 

    printf("Amortized execution time: %f\n", sum);
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
		nodes[i].hook = NULL;
		nodes[i].sum = 1;
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
	LCT parent = node->hook;
	/* cases where the rotate right causes no efect */
  	if (node == NULL || parent == NULL || parent->left != node)
  		return;

	/* switch parent hook ith node hook (equal in rotate left)*/
 	if (parent->hook != NULL)
 	{
 		node->hook = parent->hook;
 		/* conditions for the path parent hooks */
 		if (node->hook->left == parent) node->hook->left = node; 
 		if (node->hook->right == parent) node->hook->right = node;
 	}
 	else
 		node->hook = NULL;

  	if (node->right != NULL)
  		node->right->hook = parent;

  	parent->left = node->right;
  	node->right = parent;
  	parent->hook = node;
}


/* @brief: Function that rotates a specific node to the left. 
 *		   Receives a pointer to that node.
 */
void rotateLeft(LCT node)
{
	LCT parent =  node->hook;

	if (node == NULL || parent == NULL || parent->right != node)
		return;

	/* switch parent hook with node hook (equal in rotate right)*/
	if (parent->hook != NULL)
	{
		node->hook = parent->hook;
		/* conditions for the path parent hooks */
		if (node->hook->left == parent) node->hook->left = node;
		if (node->hook->right == parent) node->hook->right = node;
	}
	else
		node->hook = NULL;

	if (node->left != NULL)
		node->left->hook = parent;

	parent->right = node->left;
	node->left = parent;
	parent->hook = node;
}

/* @brief: Splays a node according to the definition of the splaying step from paper "Self Adjusting Binary Search 
 *         Trees" from Sleator and Tarjan.
 * 
 *		   Receives a pointer to that node.
 */
void splayingStep(LCT node)
{
	LCT parent = node->hook;
	LCT g_parent = parent->hook; /* parent is never NULL because we only call this while parent != NULL */

	if (g_parent != NULL)
		unflip(g_parent);
	unflip(parent);
	unflip(node);

	/* Case 1 ZIG */
	if (g_parent == NULL && (parent->right == node))
		rotateLeft(node);

	else if (g_parent == NULL && (parent->left == node))
		rotateRight(node);

	/* Case 2 ZIG - ZIG */
	/* if both x and p(x) are left childs */
	else if ((parent == g_parent->left) && (parent->left == node))
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
	else if ((parent == g_parent->left) && (parent->right == node))
	{	
		rotateLeft(node);
		rotateRight(node);
	}
	/* p(x) is a right child and x is a left child */
	else if ((parent == g_parent->right) && (parent->left == node))
	{	
		rotateRight(node);
		rotateLeft(node);
	}
	/* In the context of this project g_parent can be != NULL because its a pathparent*/
	else if (parent->right == node)
		rotateLeft(node);

	else if (parent->left == node)
		rotateRight(node);
}

/* @brief: Performs several Splays to a node until the node is made the root of his aux tree.
 *		   Receives a pointer to that node.
 */
void splay(LCT node)
{
	if (node != NULL)
	{
		/* unflip nodes */
		unflip(node);

		/* While hook =! NULL or node hook doesnt point back to node */
		while (1)
		{
			if (node->hook == NULL) break;

			else if (node->hook->left != node && node->hook->right != node) break;
			
			else splayingStep(node);
		}
	}		
}

/* @brief: Function that accesses a node, this function should change the represented tree in order to made
 *         the path from the root to node v the prefered path.
 *         This function was defined according with whats defined in "A Data Structure for Dynamic trees" paper
 *         from Sleator and Tarjan.
 *
 *         Receives an array with all LCT nodes and a int v that represents the position of the node we want
 *         to access in that array.
 */
void access(LCT t, int v)
{
	LCT node = &t[v];
	LCT w = NULL;
	splay(node);
	node->right = NULL;

	while (node->hook !=  NULL)
	{
		w = node->hook;
		splay(w);
		w->right = node;
		splay(node);
	}
}

/* @brief: Adds the edge from r to v to the represented tree. Before linking the node r is made the root of 
 *         its represented tree.
 *
 *         Receives an array with all LCT nodes, int u that represents the position of the node u and int v that 
 *         represents the position of the node v.
 */
void link(LCT t, int r, int v)
{
	/* specific part for this project */
	if (connectedQ(t, r, v))
		return;
	
	reRoot(t, r);
	/* actual link operations */
	access(t, v);
	t[r].left = &t[v]; 
	t[v].hook = &t[r];  
}

/* @brief: This function removes the edge (u, v).
 *
 *         Receives an array with all LCT nodes, int u that represents the position of the node u and int v that 
 *         represents the position of the node v.
 */
void cut(LCT t, int r, int v)
{	
	/* specific part for this project */
	reRoot(t, r);
	/* actual link operations */
	access(t, v);
	if (t[v].left == &t[r] && t[r].right == NULL)
	{
		t[v].left->hook = NULL;
		t[v].left = NULL;
	}
}

/* @brief: This function checks if a node u and a node v are in the same path. We assume that node u has a depth 
 *         higher than v.
 *
 *         Receives an array with all LCT nodes, int u that represents the position of the node u and int v that 
 *         represents the position of the node v.
 */
int checkPrefPath(LCT t, int u, int v)
{
	LCT current = &t[u];

	while (current->hook != NULL)
	{
		if (current->hook == &t[v]) return 1;
		current = current->hook;
	}

	return 0;
}

/* @brief: Funtion that verifies if there is a path between a node u and a node v.
 *
 *         Receives an array with all LCT nodes, int u that represents the position of the node u and int v that 
 *         represents the position of the node v.
 */
int connectedQ(LCT t, int u, int v)
{
	reRoot(t, u);
	access(t, v);
	return checkPrefPath(t, u, v);
}

/* @brief: Funtion that unflips a node if he is fliped.
 *
 *         Receives the node we want to unflip.
 */
void unflip(LCT node)
{
	LCT aux;

	if (node->sum != -1)
		return;

	/* switch right with left */
	aux = node->left;
	node->left = node->right;
	node->right = aux;

	/* flip right and left childs bits */
	if (node->right != NULL)
		node->right->sum *= -1;
	if (node->left != NULL)
		node->left->sum *= -1;
	/* restore sum bit */
	node->sum = 1;
}

/* @brief: Funtion that makes v the root of the represented tree.
 *
 *         Receives an array with all LCT nodes, int v that represents the node we want to make the root.
 */
void reRoot(LCT t, int v)
{
	access(t, v);
	t[v].sum *= -1; /* invert node */
	access(t, v);
}

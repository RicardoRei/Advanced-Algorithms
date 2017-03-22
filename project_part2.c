#include <stdio.h>
#include <stdlib.h>


typedef struct node
{
	struct node * left;
	struct node * left
	struct node * hook;
	int sum;
} vertex;

typedef vertex * LCT;

LCT allocLct(int V)
{
	int i;
	LCT nodes = (LCT) malloc(sizeof(struct vertex)*V);
	
	for( i = 0; i < V; i++)
	{
		nodes[i]->left = NULL;
		nodes[i]->right = NULL;
		nodes[i]->hook = NULL;
		nodes[i]->sum = 0;
	}

	return nodes;
}

void freeLCT(LCT t)
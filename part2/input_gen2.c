#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

/* @brief: This file generates a random LCT struct. The first argument represents the number of nodes we want to 
 *         create, the second one the number of link and cut operations and the third one is the nr of links per
 *		   100 operations. 
 *         EX: ./gen 100000 5000 100 -> creates 100000 nodes and performs 5000 link/cut ops and 100% of this ops 
 *              are links.
 */
int main(int argc,  char* argv[])
{
	int n1, n2, op, count = 0;
	int n_nodes = atoi(argv[1]);
	int n_ops = atoi(argv[2]);
	int l_percent = atoi(argv[3]);
	FILE * fp = fopen("input.txt" ,"w+");

	/* initialize random seed: */
	srand ( time(NULL) );

	fprintf(fp, "%d\n", n_nodes);
	while (count < n_ops)
	{
		op = rand()%(100+1);
		n1 = rand()%(n_nodes+1);
		n2 = rand()%(n_nodes+1);

		if (op <= l_percent)
			fprintf(fp, "%c %d %d\n", 'L', n1, n2);
		else
			fprintf(fp, "%c %d %d\n", 'C', n1, n2);

		count++;
	}

	fprintf(fp, "X\n");
	fclose(fp);
}

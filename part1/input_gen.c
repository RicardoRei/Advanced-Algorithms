#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

char indexToLetter(int index);
void highlyRepetitiveSequenceGen(int P_SIZE, int T_SIZE, FILE *fp);
void randomSequenceGen(int P_SIZE, int T_SIZE, FILE *fp);

/* @brief: This file generates a random DNA sequence T and a random pattern P. The size of the T is passed by the 
 *         the first arg of main,the size of P is the second arg, the third arg sets the type of sequence.
 *         EX: ./gen 100000 5 1 generates a random sequence T with size 100000 and a P with size 5. 
 */
int main(int argc,  char* argv[])
{
	int P_SIZE = atoi(argv[2]);
	int T_SIZE = atoi(argv[1]);
	FILE * fp = fopen("input.txt" ,"w+");

	/* initialize random seed: */
	srand ( time(NULL) );

	if (atoi(argv[3]) == 0) randomSequenceGen(P_SIZE, T_SIZE, fp);
	else highlyRepetitiveSequenceGen(P_SIZE, T_SIZE, fp);

	fclose(fp);
}

/*
 * @brief: This function generates T_SIZE x a random letter, and prints that letter to a file.
 *         After generating T it generates P with the same process.
 *
 * @param: P_SIZE - Size of the pattern we want to generate.
 *         T_SIZE - Size of the text we want to generate.
 *         fp - File pointer.
 * 
 * Output exemple: T ACCATGGACTAGA...
 *                 K ACTGT
 *                 B ACTGT
 *                 x
 */
void randomSequenceGen(int P_SIZE, int T_SIZE, FILE *fp)
{
	int n, count = 0;
	char * pattern = (char*) malloc(sizeof(char) * P_SIZE);

	fprintf(fp, "%c ", 'T');
	while (count < T_SIZE-1)
	{ 
		n = rand()%4;
		fprintf(fp, "%c", indexToLetter(n));
		count++;
	}
	n = rand()%4;
	fprintf(fp, "%c\n", indexToLetter(n));

	count = 0;
	while (count < P_SIZE)
	{ 
		n = rand()%4;
		pattern[count] = indexToLetter(n);
		count++;
	}

	fprintf(fp, "N %s\nK %s\nB %s\nX\n", pattern, pattern, pattern);
}


/*
 * @brief: This function generates a text with size T_SIZE higly repetitive and a pattern with size P_SIZE
 *         with only one letter.
 *
 * @param: P_SIZE - Size of the pattern we want to generate.
 *         T_SIZE - Size of the text we want to generate.
 *         fp - File pointer.
 * 
 * Output exemple: T AAAAAAAGGGGGGGGGGGCCC..
 *                 K CCC
 *                 B CCC
 *                 x
 */

void highlyRepetitiveSequenceGen(int P_SIZE, int T_SIZE, FILE *fp)
{
	int count = 0, i, n;
	int nr_letters;
	char * pattern = (char*) malloc(sizeof(char) * P_SIZE);
	char pattern_letter;

	fprintf(fp, "%c ", 'T');
	while (count < T_SIZE-1)
	{
		n = rand()%4;
		nr_letters = rand()%(T_SIZE-count);

		i=0;
		while (i < nr_letters)
		{
			fprintf(fp, "%c", indexToLetter(n));
			count++;
			i++;
		}
	}
	fprintf(fp, "%c\n", indexToLetter(n));

	pattern_letter = indexToLetter(rand()%4);
	count = 0;
	while (count < P_SIZE)
	{ 
		pattern[count] = pattern_letter;
		count++;
	}
	fprintf(fp, "N %s\nK %s\nB %s\nX\n", pattern, pattern, pattern);
}

/*
 * @brief: This auxiliar function gives index corresponding to the letter c in table R.
 *         Table R is organized by ATCG.
 *
 * @param: c - The char we want to know the index.
 */
char indexToLetter(int index)
{
	switch (index) {
		case 0: return 'A';
		case 1: return 'T';
		case 2: return 'C';
		case 3: return 'G';
		default: return '?';
	}
}

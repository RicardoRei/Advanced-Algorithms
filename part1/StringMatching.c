#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX(a,b) ((a) > (b) ? a : b) /* function to compute the max between 2 numbers */

/*
 * @brief: This structure maintains the size of the string T and the nr of the occupied spaces in that
 *		   array. With this information we can decide if the string T needs to be expanded or not.
 */
typedef struct
{
	int size;
	int occupied;
	char * str;

} DynamicArray;

/************************************************ SIGNATURES ****************************************************/

DynamicArray* createDynamicArray();
void freeDynamicArray(DynamicArray * array);
void readString(DynamicArray * array);
void naiveStringMatching(char * T, int n, char * P, int m);
int * computePrefixFunction(char * P, int m);
void KMP_matcher(char * T, int n, char * P, int m);
void BM_matcher(char * T, int n, char * P, int m);
int * computeRightmost(char * P, int m);
int * computeNTable(char * P, int m);
int * compute_L_Prime_Table(char * P, int m, int * N);
int * compute_l_prime_table(char * P, int m, int * N);

/* Auxiliar Functions */
void printTable(int * table, int size);
int letterToIndex(char c);

/*****************************************************************************************************************/

int main()
{
	char command;
	DynamicArray * T = NULL;
	DynamicArray * P = NULL;
	
	while ((command = getchar()) != 'X') /* reads the command and if its X exits the while cycle */
	{  
        getchar(); /* reads the space after the command and the \n of X command */

        if (P != NULL) freeDynamicArray(P);/*P's are recycled every command which means we can free here*/
		P = createDynamicArray();

        switch (command) 
        {
	        case 'T':

	        	/* we need to reset the array at every T command and free the memory from the last array*/
	        	if (T != NULL) freeDynamicArray(T);
	        	T = createDynamicArray();
	        	readString(T);
	        	break;

	        case 'N':
	        	readString(P);
	        	naiveStringMatching(T->str, T->occupied, P->str, P->occupied);
	            break;

	        case 'K':
	        	readString(P);

	        	KMP_matcher(T->str, T->occupied, P->str, P->occupied);
	        	break;

	        case 'B':
	        	readString(P);
				BM_matcher(T->str, T->occupied, P->str, P->occupied);
	            break;

	 		default:
	            printf("ERROR: Unknown command %c\n", command);
	    }
    }
    /* free memory for T and P */
    if (T != NULL) freeDynamicArray(T); 
    if (P != NULL) freeDynamicArray(P); 
	
	return EXIT_SUCCESS;
}

/************************************************ DYNAMIC ARRAY **********************************************/

/*
 * @brief: This function creates an DynamicArray structure and it initializes the field values.
 *
 * Return: returns a pointer to that structure.
 */
DynamicArray* createDynamicArray()
{
	DynamicArray * array = (DynamicArray *) malloc(sizeof(DynamicArray));
	array->str = (char *) malloc(sizeof(char)); /* string T is initialized with size for only 1 char */
	array->size = 1;
	array->occupied = 0;
	return array;
}

/*
 * @brief: This function will free the memory allocated for a structure of type DynamicArray.
 *
 * @param: it receives a pointer to the Structure its supose to free.
 *
 * Note: its hard to test this function because when memory is freed its still acessible until other 
 *		 program randomly overwrites that part of the memory.
 */
void freeDynamicArray(DynamicArray * array)
{
	free(array->str);
	free(array);
}
										   
/************************************************* COMMAND T ****************************************************/

/*
 * @brief: This funtion will read the caracters from the command T and store them into string T of the 
 *		   DynamicArray. When occcupied field is equal to size (when string T has no space) the size of
 *		   string T its duplicated.
 *
 * @param: it receives a pointer to the Structure were its going to store the string.
 */
void readString(DynamicArray * array)
{
	char new = getchar();

	while (new != '\n')
	{
		/* we need more memory when the size is equal to the occupied spaces */
		if (array->occupied == array->size)
		{
			array->size = 2*array->size;
			array->str = (char *) realloc(array->str, array->size);
		}

		array->str[array->occupied++] = new;
		new = getchar();
	}
}

/************************************************* COMMAND N ****************************************************/

/*
 * @brief: Naive String Matching algorithm studied in theoretical class number 3.
 *		   This aproach its takes O(nm) which its not very practical for online string matching.
 *
 * @param: T - The string where we want to find the patterns.
 *	       n - The size of string T.
 *		   P - The pattern we want to find.
 *		   m - The size of that pattern.
 */
void naiveStringMatching(char * T, int n, char * P, int m)
{
	int i;
	for (i = 0; i + m <= n; i++)
		if (0 == strncmp(&(T[i]), P, m))
			printf("%d ", i);
	printf("\n");
}

/************************************************* COMMAND K ****************************************************/

/*
 * @brief: Computes the prefix function. Based on the pseudocode from cap 32.4 from Intruduction to 
 *		   Algorithms (CLRS 3rd edition).
 *
 * @param: P is the pointer to the buffer that contains the pattern and m is the size of it.
 *
 * @return: returns a vector with the function pi computed.
 */
int * computePrefixFunction(char * P, int m)
{
	int * pi = (int *) malloc(sizeof(int) * m);
	int q, k = 0;
	pi[0] = k;

	for (q = 1; q < m; q++)
	{
		while ( k > 0 && P[k] != P[q] )
			k = pi[k-1];

		if (P[k] == P[q])
			k++;
		
		pi[q] = k;
	}
	
	return pi;
}

/*
 * @brief: Knuth-Morris-Pratt Algorithm based in cap 32.4 from Intruduction to Algorithms (CLRS 3rd edition).
 *
 * @param: T - The string where we want to find the patterns.
 *	       n - The size of string T.
 *		   P - The pattern we want to find.
 *		   m - The size of that pattern.
 */
void KMP_matcher(char * T, int n, char * P, int m)
{
	int * pi = computePrefixFunction(P, m);
	int i, q = 0, count = 0; 

	for (i = 0; i < n ; i++)
	{	
		/* ++count is always true, this expression serves only to increment the counter of comparations */ 
		while ( q > 0 && P[q] != T[i] && ++count)
			q = pi[q-1];
			
		if (++count && P[q] == T[i])
			q++;	
		
		if (q == m)
		{
			printf("%d ", i - m +1);
			count += (i < n-1) ? 1 : 0;
			q = pi[q-1];
		}

	}

	printf("\n%d \n", count);
	free(pi);
}

/************************************************* COMMAND B ****************************************************/

/*
 * @brief: Computes the rightmost ocurrences of the letters in the pattern.
 *
 * @param: P is the pointer to the buffer that contains the pattern and m is the size of it.
 *
 * @return: returns a vector.
 */
int * computeRightmost(char * P, int m){
	int * rightmost =  (int *) malloc(sizeof(int) * 4); /*ATCG*/
	int found=0;
	int i;
	rightmost[0]= 0; rightmost[1]= 0; rightmost[2]= 0; rightmost[3]= 0;

	for ( i=m-1; i>=0; i--){
		switch (P[i]) 
		{
	        case 'A' :
	        	if (rightmost[0] == 0){
	        		rightmost[0] = i+1;
	        		found++;
	        	}
	        	break;
	        case 'T':
	        	if (rightmost[1] == 0){
	        		rightmost[1] = i+1;
	        		found++;
	        	}
	        	break;
	        case 'C':
	        	if (rightmost[2] == 0){
	        		rightmost[2] = i+1;	
	        		found++;
	        	}
	        	break;
	        case 'G':
	        	if (rightmost[3] == 0){
	    	        rightmost[3] = i+1;
	    	        found++;		
	    	    }		
	        	break;
		}
		if (found == 4)
			return rightmost;
	}

	return rightmost;
}

/*
 * @brief: Computes table N for a pattern P with size m. The entry N[i] ( 0 <= i < m-1) contains the lenght
 *         of the longest suffix of the substring P[0..i] that is also a suffix of the full pattern P.
 *         (based on a definition from cap 2.2.4 from Algorithms on Strings, Trees, and Sequences: Gusfield 1997.)
 *
 * @param: P - pointer to the buffer that contains the pattern.
 *         m - the size of the pattern.
 *
 * @return: returns a vector.
 */
int * computeNTable(char * P, int m)
{
	int * N =  (int *) malloc(sizeof(int) * m-1);
	int i, j;

	for (i = 0; i < m-1; i++) 
	{
		for (j = i; j >= 0; j--) 
		{
			if (P[j] != P[m-1-i+j]) break;
		}
		N[i] = i-j;
	}

	return N;
}

/*
 * @brief: Computes table L'. The entry L'[i] (0 <= i < m) contains the largest index j less than n such
 *         that N[j] = |P[i..m-1]|
 *         (based on Theorem 2.2.2 from the book: Algorithms on Strings, Trees, and Sequences: Gusfield 1997.)
 *
 * @param: P - pointer to the buffer that contains the pattern.
 *         m - the size of the pattern.
 *         N - table N explained in function computeNTable.
 *
 * @return: returns a vector.
 */
int * compute_L_Prime_Table(char * P, int m, int * N)
{
	int * L_prime = (int *) malloc(sizeof(int) * m);
	int i, j, largest_j, substring_size;

	for (i = 0; i < m; i++)
	{
		largest_j = 0;
		substring_size = m - i; /* |p[i..n]| = n - i + 1 as defined in the book but m = n+1 in that definition*/

		for (j = 0; j < m-1; j++)
			if ((j+1) > largest_j && N[j] == substring_size)
				largest_j = j+1;

		L_prime[i] = largest_j;
	}
	return L_prime;
}

/*
 * @brief: Computes table l'. The entry l'[i] (0 <= i < m) contains the largest j <= |P[i..m-1]| such that
 *         N[j] == j.
 *         (based on Theorem 2.2.4 from the book: Algorithms on Strings, Trees, and Sequences: Gusfield 1997.)
 *
 * @param: P - pointer to the buffer that contains the pattern.
 *         m - the size of the pattern.
 *         N - table N explained in function computeNTable.
 *
 * @return: returns a vector.
 */
int * compute_l_prime_table(char * P, int m, int * N)
{
	int * l_prime = (int *) malloc(sizeof(int) * m);
	int i, j, largest_j, substring_size;

	for (i = 0; i < m; i++)
	{
		largest_j = 0;
		substring_size = m -i;

		for (j = 1; j <= substring_size; j++)
			if (j > largest_j && N[j-1] == j)
				largest_j = j;

		l_prime[i] = largest_j;
	}
	return l_prime;
}

/*
 * @brief: Boyer Moore Algorithm based in cap 2.2.6 from Algorithms on Strings, Trees, and Sequences: Gusfield 1997.
 *
 * @param: T - The string where we want to find the patterns.
 *	       n - The size of string T.
 *		   P - The pattern we want to find.
 *		   m - The size of that pattern.
 */
void BM_matcher(char * T, int n, char * P, int m)
{
	int * N = computeNTable(P, m);
	int * L_Prime = compute_L_Prime_Table(P, m, N);	
	int * l_prime = compute_l_prime_table(P, m, N);
	int * R = computeRightmost(P, m);
	int k, i, h, goodSuffixShift, badSuffixShift, count=0;
	
	k = m-1;
	while (k <= n-1) 
	{
		i = m-1;
		h = k;
		/* ++count expression is just to increment the counter and its value is always true */
		while (i > -1 && ++count && P[i] == T[h]) 
		{
			i--;
			h--;
		}

		if (i == -1) 
		{	
			/* printing h is the same as printing the position of the first letter of P in T*/
			printf("%d ", h+1);
			k = k + m - l_prime[1];
		}

		else if (i == m-1)
			/* Good Suffix rule says that in case of a match the pattern should shift 1 and Bad Suffix rule
			   says that a shift should be the Max between 1 and table of the rightmost. So in this case we 
			   can consider the Bad Suffix rule only.
			*/
			k += MAX(1, i+1 - R[letterToIndex(T[h])]);

		else 
		{
			/* 
			The Shift given by Good Suffix Rule as explained in cap 2.2.5 is different when a mismatch occur
			in i and L'[i+1] > 0. 
			The next expression verifies the value of L'[i+1] and assigns the correct value. 
			*/
			goodSuffixShift = (L_Prime[i+1] == 0) ? m - l_prime[i+1] : m - L_Prime[i+1];
			badSuffixShift = MAX(1, i+1 - R[letterToIndex(T[h])]);
			k += MAX(badSuffixShift, goodSuffixShift);
		}
	}

	free(N);
	free(L_Prime);
	free(l_prime);
	free(R);
	printf("\n%d \n", count);
}

/************************************************** AUXILIAR ****************************************************/

/*
 * @brief: This auxiliar function gives index corresponding to the letter c in table R.
 *         Table R is organized by ATCG.
 *
 * @param: c - The char we want to know the index.
 */
int letterToIndex(char c)
{
	switch (c) 
	{
		case 'A': return 0;
		case 'T': return 1;
		case 'C': return 2;
		case 'G': return 3;
		default: return -1;
	}
}

/*
 * @brief: This auxiliar function is usefull for debug purpose by printing a table.
 *
 * @param: table - The table we want to print.
 *         size - The size of the table.
 */
void printTable(int * table, int size)
{
	int i;
	for (i = 0; i < size-1; i++)
		printf("%d ", table[i]);
	printf("%d\n", table[size-1]);
}

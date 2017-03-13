#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX 100 /* max value for the size of a pattern */

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

/******************************************** SIGNATURES **********************************************/

DynamicArray* createDynamicArray();
void freeDynamicArray(DynamicArray * array);
void readString(DynamicArray * array);
void naiveStringMatching(char * T, int n, char * P, int m);
int * computePrefixFunction(char * P, int m);
void KMP_matcher(char * T, int n, char * P, int m);


int max(int a, int b, int c);
int * computeRightmost(char * P, int m);
void Boyer_Moore_matcher(char * T, int n, char * P, int m);
char * reverseString(char * P, int n);
int * z_based_BoyerMoore(char * P, int n);
int * preProcessingDetail(char * P, int n);


/*******************************************************************************************************/

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
	        	preProcessingDetail(P->str,P->occupied);
	        	/* BOYES MOORE FUNCTION SHOULD BE HERE*/
	        	freeDynamicArray(P);
	            break;

	 		default:
	            printf("ERROR: Unknown command\n");
	    }
    }
    /* free memory for T and P */
    if (T != NULL) freeDynamicArray(T); 
    if (P != NULL) freeDynamicArray(P); 
	
	return EXIT_SUCCESS;
}

/******************************************** DYNAMIC ARRAY ********************************************/

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
										   
/********************************************* COMMAND T **********************************************/

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

/********************************************* COMMAND N **********************************************/

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
	puts("");
}

/********************************************* COMMAND K **********************************************/

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
	int q, k = -1;
	pi[0] = k;

	for (q = 1; q < m; q++)
	{
		while ( k > -1 && P[k+1] != P[q] )
			k = pi[k];
		
		if (P[k+1] == P[q])
			k++;
		
		pi[q] = k;
	}
	return pi;
}


/*
 * @brief: returns the maximum between 3 numbers.
 * Clearly not optimized!!	
 */
int max(int a, int b, int c){

	if ((a > b) && (a > c))
		return a;

	if ((a>b) && (a<c))
		return c;

	if ((a<b) && (b<c))
		return c;

	if ((a<b) && (b>c))
		return b;

	return 0;
}



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

	rightmost[0]=0;
	rightmost[1]=0;
	rightmost[2]=0;
	rightmost[3]=0;

	for ( i=m-1; i>=0; i--){
		switch (P[i]) 
        {
	        case 'A':
	        	if (rightmost[0]==0){
	        		rightmost[0]=i;
	        		found++;
	        	}
	        case 'T':
	        	if (rightmost[1]==0){
	        		rightmost[1]=i;
	        		found++;
	        	}
	        case 'C':
	        	if (rightmost[2]==0){
	        		rightmost[2]=i;	
	        		found++;
	        	}
	        case 'G':
	        	if (rightmost[3]==0){
	    	        rightmost[3]=i;
	    	        found++;		
	    	    }		
		}

		if (found == 4)
			return rightmost;
	}
	return rightmost;
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
	int i, q = -1, count = 0; /* counter not being used yet... */
	
	for (i = 0; i < n ; i++)
	{	

		while ( q > -1 && P[q+1] != T[i])
			q = pi[q];

		if ( P[q + 1] == T[i] )
			q++;
		
		
		if ( q == m -1)
		{
			printf("%d ", i - q);
			q = pi[q];
		}	
	}
	printf("\n%d\n", count);
}

/*
 * @brief: Calculates table L'()
 */
int * z_based_BoyerMoore(char * P, int n)
{
	int * L_Prime = (int *) malloc(sizeof(int) * n);
	char * reversedPattern = reverseString(P,n);
	int * zBasedOnReverse = computePrefixFunction(reversedPattern,n);

	int i,j;

	for (i = 0; i < n ; i++){
		L_Prime[i] = 0;
	}

	for (j = 0; j < n-1 ; j++){
		i = n - zBasedOnReverse[j] + 1;
		L_Prime[i] = j;
	}

	return L_Prime;
}



int * preProcessingDetail(char * P, int n)
{
	int * l_Prime = (int *) malloc(sizeof(int) * n);	
	int i, suffixSize, j,len;

	for (i = n-1; i > 0 ; i--)
	{
		suffixSize = n-i;
		len=0;
		for (j=0; j < suffixSize;j++)
		{
			
			
			if (P[j]==P[j+i])
			{
				len++;
			}
			else
				break;
		}
		

		l_Prime[n-i]=len;
	}

	return l_Prime;
}



/*
 * @brief: reverses a string
 */
char * reverseString(char * P, int n)
{
	int i;
	char *str = (char *) malloc(sizeof(char) * n);

	for (i = 0; i < n ; i++){
		str[n-i-1]=P[i];
	}

	return str;
}



/*
 * @brief: BoyerMoore Algorithm based in cap 32.4 from Intruduction to Algorithms (CLRS 3rd edition).
 *
 * @param: T - The string where we want to find the patterns.
 *	       m - The size of string T.
 *		   P - The pattern we want to find.
 *		   n - The size of that pattern.
 */
void Boyer_Moore_matcher(char * T, int m, char * P, int n)
{
	/*PreProcessing*/
	int * rightmost = computeRightmost(P,n);
	int * L_Prime = z_based_BoyerMoore(P,n);

	int rightVal;

	/*Missing l(i)*/


	int k = n;
	while (k <= m){
		int i = n;
		int h = k;

		while (i > 0 && (P[i] == T[h])){
			/*Comparing counter should be here*/
			i--;
			h--;
		}

		if (i==0){
			printf("%d ", k);
			k = k + n ; /*l'(2);*/
		}

		else{

			switch(T[k])
			{
				case 'A':
					rightVal=rightmost[0];
				case 'T':
					rightVal=rightmost[1];
				case 'C':
					rightVal=rightmost[2];
				case 'G':
					rightVal=rightmost[3];	
			}


			/*int shift=max(i-rightVal,rule2,1); pg17*/
		}
	}
}






/********************************************** AUXILIAR  ***********************************************/

/*
 * @brief: This function reads the pattern from the standard input.
 *
 * @param: Receives a pointers to the buffer where the pattern should be stored.
 *
 * @return:	Returns the number of characters readed from the standard input. (The size of the pattern)
 *
 *	NOTE: This is the more efficient way, because it calculates the size of the pattern while it is being
 *		  read from the input, yet there is a simpler way in comments.
 *		  The scanf should return the number of readed elements but i think thats only in c++. 
 */
int readPattern(char * pattern)
{
	/**************************** Alternative ************************************ 
	scanf("%s", pattern);
	return strlen(pattern);
	******************************************************************************/
	int i = 0;
	char new;
	
	while ((new = getchar()) != '\n') 
		pattern[i++] = new;

	pattern[i] = '\0';
	return i;
}


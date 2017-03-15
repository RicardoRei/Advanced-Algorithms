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
	        	/*Boyer_Moore_matcher(T->str, T->occupied, P->str, P->occupied);*/
	        	preProcessingDetail(P->str,P->occupied);
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
	        case 'A' :
	        	if (rightmost[0]==0){
	        		rightmost[0]=i;
	        		found++;
	        	}
	        	break;
	        case 'T':
	        	if (rightmost[1]==0){
	        		rightmost[1]=i;
	        		found++;
	        	}
	        	break;
	        case 'C':
	        	if (rightmost[2]==0){
	        		rightmost[2]=i;	
	        		found++;
	        	}
	        	break;
	        case 'G':
	        	if (rightmost[3]==0){
	    	        rightmost[3]=i;
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


	int i,j,a;

	for (i = 0; i < n ; i++){
		L_Prime[i] = 0;
	}

	for (j = 0; j < n-1 ; j++){
		i = n - zBasedOnReverse[j] + 1; /* estes prefixos podem dar -1 ?*/
		if(i>=0 && i<n){
			L_Prime[i] = j;
		}
	}

	for(a=0;a<n;a++){
		printf("L'(%d): %d \n",a,L_Prime[a]);
	}

	free(reversedPattern);
	free(zBasedOnReverse);

	return L_Prime;
}


int is_prefix(char *word, int wordlen, int pos) {
    int i;
    int suffixlen = wordlen - pos;

    for (i = 0; i < suffixlen; i++) {
        if (word[i] != word[pos+i]) {
            return 0;
        }
    }
    return 1;
}

/*
 * @brief: Calculates table l'()
 */
int * preProcessingDetail(char * P, int n)
{
	int * l_Prime = (int *) malloc(sizeof(int) * n);	
	int i, j,a,largestLength;
	char * pattern = (char *)malloc(sizeof(char) * n);

	for (i=0;i<n;i++){

		strcpy(pattern, P+i); /*Pattern of size n-i*/
		largestLength=0;

		j=1;
		while (1){
			if (is_prefix(pattern,n-i,j)){
				largestLength=n-i-j;
				break;
			}
			else if(j>n){
				break;
			}
			else{
				j++;
			}
		}	
		l_Prime[i]=largestLength;
	}	

	for(a=0;a<n;a++){
		printf("l'(%d), %d \n",a,l_Prime[a]);
	}

	free(pattern);

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
	int a,rightVal, delta2, comparator;
	int * rightmost = computeRightmost(P,n);
	printf("rightmost A: %d \n",rightmost[0]);
	printf("rightmost T: %d \n",rightmost[1]);
	printf("rightmost C: %d \n",rightmost[2]);
	printf("rightmost G: %d \n",rightmost[3]);
	fflush(stdout);



	int * L_Prime = z_based_BoyerMoore(P,n);
	


	int * l_Prime = preProcessingDetail(P,n);
	

	fflush(stdout);


	/*How to debug?*/
	/*Gonna print tables?*/
	/*
	int k = n;
	comparator = 0;
	while (k <= m){
		printf("k:%d, m:%d\n",k,m);
		int i = n;
		int h = k;

		while (i > 0 && (P[i] == T[h])){
			comparator++;
			i--;
			h--;
		}

		if (i==0){
			printf("matches: %d \n", k);
			k = k + n - l_Prime[2];
		}
		else{

			switch(T[k])
			{
				case 'A':
					rightVal=rightmost[0];
					printf("set rightval using A\n");
					break;
				case 'T':
					rightVal=rightmost[1];
					printf("set rightval using T\n");
					break;
				case 'C':
					rightVal=rightmost[2];
					printf("set rightval using C\n");
					break;
				case 'G':
					rightVal=rightmost[3];
					printf("set rightval using G\n");
					break;	
			}

			delta2 = L_Prime[i];
			if (delta2 == 0){
				delta2 = l_Prime[i];
			}

			k = max(i-rightVal,delta2,1);
		}
	}
	printf("compared: %d\n", comparator);*/
	free(rightmost);
	free(L_Prime);
	free(l_Prime);
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


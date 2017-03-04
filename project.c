#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX 100 /* max value for the size of a pattern */

/****************************************** DYNAMIC ARRAY *********************************************/

/*
 * @brief: This structure maintains the size of the string T and the nr of the occupied spaces in that
 *		   array. With this information we can decide if the string T needs to be expanded or not.
 */
typedef struct
{
	int size;
	int occupied;
	char * T;

} DynamicArray;

/*
 * @brief: This function creates an DynamicArray structure and it initializes the field values.
 *
 * Return: returns a pointer to that structure.
 */
DynamicArray* createDynamicArray()
{
	DynamicArray * array = (DynamicArray *) malloc(sizeof(DynamicArray));
	array->T = (char *) malloc(sizeof(char)); /* string T is initialized with size for only 1 char */
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
	free(array->T);
	free(array);
}
										   
/********************************************* COMMAND T *********************************************/

/*
 * @brief: This funtion will read the caracters from the command T and store them into string T of the 
 *		   DynamicArray. When occcupied field is equal to size (when string T has no space) the size of
 *		   string T its duplicated.
 *
 * @param: it receives a pointer to the Structure were its going to store the string.
 */
void readStringT(DynamicArray * array)
{
	char new = getchar();

	while (new != '\n')
	{
		/* we need more memory when the size is equal to the occupied spaces */
		if (array->occupied == array->size)
		{
			array->size = 2*array->size;
			array->T = (char *) realloc(array->T, array->size);
		}

		array->T[array->occupied++] = new;
		new = getchar();
	}

	printf("Array after command T - size: %d occupied: %d T:{%s}\n", array->size, array->occupied, array->T);
}

/********************************************* COMMAND N *********************************************/

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

/*
 * @brief: Handler for the command N. This function reads the pattern and calls the naiveStringMatching 
 *		   function.
 *
 * @param: receives a DynamicArray pointer that points to the structure that contains the String T where
 *		   we want to find the pattern.
 *
 *	NOTE: This is a more efficient way, because it calculates the size of the pattern while it is being
 *		  read from the input. 
 *		  But there is a simpler one in comments. 
 */
void commandN(DynamicArray * array)
{
	/**************************** Alternative ************************************ 

	char * pattern =  (char *) malloc(sizeof(char) * MAX);
	scanf("%s", pattern);
	naiveStringMatching(array->T, array->occupied, pattern, strlen(pattern));

	******************************************************************************/

	char * pattern =  (char *) malloc(sizeof(char) * MAX);
	int i = 0;
	char new;
	
	while ((new = getchar()) != '\n') 
		pattern[i++] = new;	
	pattern[i] = '\0';

	naiveStringMatching(array->T, array->occupied, pattern, i);
}

int main()
{
	char command;
	DynamicArray * array = NULL;
	
	while ((command = getchar()) != 'X') {  /* reads the command and if its X exits the while cycle */
        
        getchar(); /* reads the space after the command and the \n of X command */
        
        switch (command) {

	        case 'T':

	        	/* we need to reset the array at every T command and free the memory from the last array*/
	        	if (array != NULL) freeDynamicArray(array);
	        	array = createDynamicArray();
	        	readStringT(array);
	        	break;

	        case 'N':
	        
	        	commandN(array);
	            break;

	        case 'K':
	        	/* do nothing */

	        	/* eventually every command will need to read the last \n 
	        	   but while its not implemented this getchar must be here
	        	*/
	        	getchar(); 
	            break;
	        case 'B':
	        	/* do nothing */

	        	/* eventually every command will need to read the last \n 
	        	   but while its not implemented this getchar must be here
	        	*/
	        	getchar();
	            break;
	 		default:
	            printf("ERROR: Unknown command\n");
	    }
    }

	return EXIT_SUCCESS;
}





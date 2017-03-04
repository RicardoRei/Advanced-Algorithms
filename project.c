#include <stdio.h>
#include <stdlib.h>
#include <string.h>


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

int main()
{
	char command;
	DynamicArray * array = NULL;
	
	while ((command = getchar()) != 'X') {  /* reads the command and if its X exits the while cycle */
        switch (command) {

        case 'T':
        	/* we need to reset the array at every T command and free the memory from the last array*/
        	if (array != NULL) freeDynamicArray(array);
        	array = createDynamicArray(); 

        	getchar(); /* reads the space after the command */
        	readStringT(array);
        	/* do nothing */
        	break;
        case 'N':
        	/* do nothing */
            break;
        case 'K':
        	/* do nothing */
            break;
        case 'B':
        	/* do nothing */
            break;
 		default:
            printf("ERRO: Comando desconhecido\n");
        }

        if (command != 'T') getchar(); /* this getchar takes the \n on the end of N, K and B commands */
    }

	return EXIT_SUCCESS;
}





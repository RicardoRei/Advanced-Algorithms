#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int main()
{
	char *vec = (char*) malloc(sizeof(char));
	int ocupied=0;
	int size = 1;
	char new;

	while (true)
	{
		puts("introduza uma letra");
		new = getchar();
		getchar();

		if (ocupied < size)
		{
			vec[ocupied++] = new;
		}
		else
		{
			size = 2*size;
			vec = (char *) realloc(vec, size);
			vec[ocupied++] = new;
		}
		printf("%s %lu %d\n", vec, strlen(vec), size);
	}

	return 0;
}

/*

vec = empty

while (true)

	randomly choose:
	1) insert x
	2) remove vec[i]
	
	if 1) do while in main

	if 2) then
		if free space > size/4 then 
			size = size/2

*/
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>

/* if I understand correctly,
we would be calling this C code
from our kleenc.ml compiler,
so we would probably be pattern matching
on various array access syntax variations, i.e.:
arr[int a: int b] ->
arr[:int b] -> end_index_array
Arr[a:] -> start_index_array
Arr[:] -> whole_array
?
*/

char *whole_array(char *array)
{

	return array;
}

char *start_index_array(char *array, int start)
{
	char *subarray = malloc(strlen(array) + 1 - start);
	
	for (int i = start; i < strlen(array) + 1; i++) {
		subarray[i - start] = array[i];
	}
	
	return subarray;
}

/* in the general case, we don't need a null terminator at the end,
but with strings, we do -- need to account for this*/
char *end_index_array(char *array, int end)
{
	char *subarray = malloc(end + 1);
	
	for (int i = 0; i < end + 1; i++) {
		subarray[i] = array[i];
	}
	
	return subarray;
}

int main()
{
	char *string = "test string";
/*
	char *new_string = malloc(strlen(string) + 1);
	
	strcpy(new_string, string+5);
*/
	char *start_string = start_index_array(string, 5);
	char *end_string = end_index_array(string, 3);

	printf("Original String: %s\n", string);
	printf("New String: %s\n", start_string);
	printf("New String: %s\n", end_string);

	return 0;
}


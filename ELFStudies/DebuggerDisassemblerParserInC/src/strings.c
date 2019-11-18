#include "strings.h"

extern char ** create_string_array(int max_strings, int max_string_length){
	char ** str_arr = malloc(max_strings * sizeof(char *));
	for(int j=0; j<max_strings; ++j){
		str_arr[j] = calloc(max_string_length, sizeof(char));	
	}	
	return str_arr;
}

extern char ** split_string(char string[], char * delimiter, int max_strings, int max_string_length){
	char ** str_arr = create_string_array(max_strings, max_string_length); 
	char *pch;
   	pch = strtok(string, delimiter);
	int i=0;
	while(pch != NULL){
		strcpy(str_arr[i], pch);
		pch = strtok(NULL, delimiter);
		++i;
	}	
	return str_arr;
}

extern int find_string_array_length(char ** string_array){
	int i = 0;
	do{
		++i;	
	}while(*string_array[i] != 0);
	return i;
}

extern int array_length(int *array){
	int i = 0;
	while(*(array+i))
		++i;
	return i;
}

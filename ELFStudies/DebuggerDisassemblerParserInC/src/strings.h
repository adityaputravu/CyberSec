#ifndef strings_h__
#define strings_h__

#include <stdio.h>
#include <string.h>
#include <stdlib.h>

extern char ** create_string_array(int max_strings, int max_string_length);
extern char ** split_string(char string[], char * delimiter, int max_strings, int max_string_length);
extern int find_string_array_length(char ** string_array);
extern int array_length(int *array);

#endif

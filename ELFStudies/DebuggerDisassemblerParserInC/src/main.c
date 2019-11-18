#include <stdio.h>

#include "debugger.h"
#include "header.h"

#define MAX_STRING_LENGTH 	256
#define MAX_STRINGS 		10

int main(int argc, char* argv[]){
    if(argc >= 2){
	struct e_header64* header;
	header = elf64_header_struct(argv[1]);
	print_elf_header(header);
	attach(argv[1], NULL); 
    }
    else
	printf("Usage: %s <file>\n", argv[0]);

    return 0;
    
}


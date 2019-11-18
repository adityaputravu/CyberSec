#ifndef header_h__
#define header_h__
#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>

#define E_NIDENT    16

typedef unsigned char BYTE;

/* Elf64 Base Types */
typedef unsigned char 	Elf64_Byte;  // 1
typedef short 		Elf64_Half;  // 2
typedef int 		Elf64_Word;  // 4 
typedef long 		Elf64_Dword; // 8

struct e_header64{
	Elf64_Byte   e_ident[16];
	Elf64_Half   e_type;
	Elf64_Half   e_machine;
	Elf64_Word   e_version;
	Elf64_Dword  e_entry;
	Elf64_Dword  e_phoff;
	Elf64_Dword  e_shoff;
	Elf64_Word   e_flags;
	Elf64_Half   e_ehsize;
	Elf64_Half   e_phentsize;
	Elf64_Half   e_phnum;
	Elf64_Half   e_shentsize;
	Elf64_Half   e_shnum;
	Elf64_Half   e_shstrndx;
};

extern char* read_bytes(FILE* fp, int offset, int bytes);
extern char* read_bytes_backwards(FILE* fp, int offset, int bytes);
extern int btoi(char* bytes, int n);

extern bool is_elf(char *file);
extern int elf_type(char *file);
extern struct e_header64* elf64_header_struct(char *file);

#endif

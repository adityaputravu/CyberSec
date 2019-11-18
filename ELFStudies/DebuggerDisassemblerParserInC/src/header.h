#ifndef header_h__
#define header_h__
#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>

#define E_NIDENT    16

typedef unsigned char BYTE;

/* Elf32 Base Types */
typedef unsigned char 	Elf32_Byte;  // 1
typedef short 		Elf32_Word;  // 2 
typedef int 		Elf32_Dword; // 4

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
struct e_header{
	Elf32_Byte   e_ident[16];
	Elf32_Word   e_type;
	Elf32_Word   e_machine;
	Elf32_Dword  e_version;
	Elf32_Dword  e_entry;
	Elf32_Dword  e_phoff;
	Elf32_Dword  e_shoff;
	Elf32_Dword  e_flags;
	Elf32_Word   e_ehsize;
	Elf32_Word   e_phentsize;
	Elf32_Word   e_phnum;
	Elf32_Word   e_shentsize;
	Elf32_Word   e_shnum;
	Elf32_Word   e_shstrndx;
};
struct p_header64{
	Elf64_Word   p_type;
	Elf64_Word   p_flags;
	Elf64_Dword  p_offset;
	Elf64_Dword  p_vaddr;	
	Elf64_Dword  p_paddr;	
	Elf64_Dword  p_filesz;	
	Elf64_Dword  p_memsz;	
};
struct p_header{
	Elf32_Dword  p_type;
	Elf32_Dword  p_offset;
	Elf32_Dword  p_vaddr;	
	Elf32_Dword  p_paddr;	
	Elf32_Dword  p_filesz;	
	Elf32_Dword  p_memsz;	
	Elf32_Dword  p_flags;
};

extern char* read_bytes(FILE* fp, int offset, int bytes);

extern char* read_bytes_backwards(FILE* fp, int offset, int bytes);
extern int btoi(char* bytes, int n);
extern int set(int * property, int size, FILE* fp, char* temp, int offset);

extern bool is_elf(char *file);
extern int elf_type_file(char *file);
extern int elf64_type_struct(struct e_header64* header);
extern void print_elf_header(struct e_header64* header);
extern struct e_header64* elf64_header_struct(char *file);
extern struct e_header* elf_header_struct(char *file);

extern struct p_header64* elf64_program_struct(char *file, int e_phoff);
extern struct p_header* elf_program_struct(char *file, int e_phoff);

extern void print_program_header(struct p_header64* header);
#endif

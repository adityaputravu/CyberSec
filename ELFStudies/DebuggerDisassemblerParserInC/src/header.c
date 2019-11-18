#include "header.h"

extern char* read_bytes(FILE* fp, int offset, int bytes){
	char* buffer = calloc(bytes, sizeof(char));
	fseek(fp, offset, SEEK_SET);
	fgets(buffer, bytes, fp);
	return buffer;
}

extern char* read_bytes_backwards(FILE* fp, int offset, int bytes){
	char* buffer = malloc(bytes * sizeof(char));
	fseek(fp, offset+bytes, SEEK_SET);
	for(int i=bytes; i>0; --i){
		buffer[bytes-i] = fgetc(fp);
		fseek(fp, offset+i-1, SEEK_SET);
	}
	return buffer;
}

extern int btoi(char* bytes, int n){
	// Reads little endian as well
	int sum = 0;
	for(int i=0;i<n;++i){
		sum += bytes[i] << (i*8);
	}
	return sum;
}

extern bool is_elf(char *file){
	FILE *fp;
	if((fp = fopen(file, "rb")) == NULL){
		perror("Unable to open file\n");
		return false;
	}
	char buffer[4];
	fseek(fp, 0, SEEK_SET);
	fgets((char *)&buffer, 4, fp);
	fclose(fp);
	if(!strcmp((char *)&buffer, "\x7fELF"))
		return true;
	return false;
}

extern int elf_type_file(char *file){
	FILE *fp;
	if((fp = fopen(file, "rb")) == NULL){
		perror("Error opening file");
		return -1;
	}
	char type; 
	fseek(fp, 4, SEEK_SET);
	type = fgetc(fp);
	fclose(fp);
	return type * 32;
}


extern int elf_type_struct(struct e_header64* header){
	return header->e_ident[4];
}

extern void print_elf_header(struct e_header64* header){
	printf("[*] e_ident:\t\t\t\t[ ");
	for(int i=0; i<16; ++i)
		printf("%02x ", header->e_ident[i]);
	puts("]");
	printf("[*] ELF:\t\t\t\tELF%d\n", elf_type_struct(header)*32);
	printf("[*] Entry:\t\t\t\t0x%08x\n", header->e_entry);
	printf("[*] Program Header Offset:\t\t0x%08x\n", header->e_phoff);
	printf("[*] Section Header Offset:\t\t0x%08x\n", header->e_shoff);
	printf("[*] Elf Header Size:\t\t\t0x%08x\n", header->e_ehsize);
	printf("[*] Program Headers:\t\t\t%d\n", header->e_phnum);
	printf("[*] Section Headers:\t\t\t%d\n", header->e_shnum);
}

extern int set(int * property, int size, FILE* fp, char* temp, int offset){
	temp = read_bytes(fp, offset, size);
	*property = btoi(temp, size);
	free(temp);
	return offset+=size;
}

extern struct e_header64* elf64_header_struct(char *file){
	if(is_elf(file)){
		perror("Not an elf file!\n");
		return NULL;
	}
	FILE *fp;
	if((fp = fopen(file, "rb")) == NULL){
		perror("Error opening file");
		return NULL;
	}
		
	struct e_header64* header = malloc(sizeof(struct e_header64));	

	char *temp; 
	signed int offset = 0;
	
	// e_ident
	temp = read_bytes(fp, offset, sizeof(header->e_ident));
	strncpy(header->e_ident, temp, sizeof(header->e_ident));
	free(temp);

	offset += 16;

	offset = set(&(header->e_type),      sizeof(header->e_type), fp, temp, offset);
	offset = set(&(header->e_machine),   sizeof(header->e_machine), fp, temp, offset);
	offset = set(&(header->e_version),   sizeof(header->e_version), fp, temp, offset);
	offset = set(&(header->e_entry),     sizeof(header->e_entry), fp, temp, offset);
	offset = set(&(header->e_phoff),     sizeof(header->e_phoff), fp, temp, offset);
	offset = set(&(header->e_shoff),     sizeof(header->e_shoff), fp, temp, offset);
	offset = set(&(header->e_flags),     sizeof(header->e_flags), fp, temp, offset);
	offset = set(&(header->e_ehsize),    sizeof(header->e_ehsize), fp, temp, offset);
	offset = set(&(header->e_phentsize), sizeof(header->e_phentsize), fp, temp, offset);
	offset = set(&(header->e_phnum),     sizeof(header->e_phnum), fp, temp, offset);
	offset = set(&(header->e_shentsize), sizeof(header->e_shentsize), fp, temp, offset);
	offset = set(&(header->e_shnum),     sizeof(header->e_shnum), fp, temp, offset);
	offset = set(&(header->e_shstrndx),  sizeof(header->e_shstrndx), fp, temp, offset);
	
	fclose(fp);
	return header;
}

extern struct e_header* elf_header_struct(char *file){
	if(is_elf(file)){
		perror("Not an elf file!\n");
		return NULL;
	}
	FILE *fp;
	if((fp = fopen(file, "rb")) == NULL){
		perror("Error opening file");
		return NULL;
	}
		
	struct e_header* header = malloc(sizeof(struct e_header));	
	char *temp; 
	signed int offset = 0;
	
	// e_ident
	temp = read_bytes(fp, offset, sizeof(header->e_ident));
	strncpy(header->e_ident, temp, sizeof(header->e_ident));
	free(temp);
	offset += 16;

	offset = set(&(header->e_type),      sizeof(header->e_type), fp, temp, offset);
	offset = set(&(header->e_machine),   sizeof(header->e_machine), fp, temp, offset);
	offset = set(&(header->e_version),   sizeof(header->e_version), fp, temp, offset);
	offset = set(&(header->e_entry),     sizeof(header->e_entry), fp, temp, offset);
	offset = set(&(header->e_phoff),     sizeof(header->e_phoff), fp, temp, offset);
	offset = set(&(header->e_shoff),     sizeof(header->e_shoff), fp, temp, offset);
	offset = set(&(header->e_flags),     sizeof(header->e_flags), fp, temp, offset);
	offset = set(&(header->e_ehsize),    sizeof(header->e_ehsize), fp, temp, offset);
	offset = set(&(header->e_phentsize), sizeof(header->e_phentsize), fp, temp, offset);
	offset = set(&(header->e_phnum),     sizeof(header->e_phnum), fp, temp, offset);
	offset = set(&(header->e_shentsize), sizeof(header->e_shentsize), fp, temp, offset);
	offset = set(&(header->e_shnum),     sizeof(header->e_shnum), fp, temp, offset);
	offset = set(&(header->e_shstrndx),  sizeof(header->e_shstrndx), fp, temp, offset);
	
	fclose(fp);
	return header;
}



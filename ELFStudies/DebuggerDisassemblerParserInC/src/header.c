#include "header.h"

extern char* read_bytes(FILE* fp, int offset, int bytes){
	char* buffer = malloc(sizeof(char) * bytes);
	fseek(fp, offset, SEEK_SET);
	fgets(buffer, bytes+1, fp);
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
		sum += bytes[i] << i*8;
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
	fgets(&buffer, 4, fp);
	fclose(fp);
	if(!strcmp(&buffer, "\x7fELF"))
		return true;
	return false;
}

extern int elf_type(char* file){
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
	int size;
	signed int offset = -1 * sizeof(header->e_ident);

	// e_ident
	temp = read_bytes(fp, offset+=sizeof(header->e_ident), sizeof(header->e_ident));
	strncpy(header->e_ident, temp, sizeof(header->e_ident));	
	free(temp);

	int set(char* property, FILE* fp, char* temp, int offset){
		int size = sizeof(*property);
		temp = read_bytes(fp, offset, size);
		*property = btoi(temp, size);
		free(temp);
		return offset+=size;
	}

	offset = set(header->e_type, fp, temp, offset);
	offset = set(header->e_machine, fp, temp, offset);
	offset = set(header->e_version, fp, temp, offset);
	offset = set(header->e_entry, fp, temp, offset);
	offset = set(header->e_phoff, fp, temp, offset);
	offset = set(header->e_shoff, fp, temp, offset);
	offset = set(header->e_flags, fp, temp, offset);

	/*
	// e_type
	size = sizeof(header->e_type);
	temp = read_bytes(fp, offset+=size, size);
	header->e_type = btoi(temp, size); 
	free(temp);

	// e_machine
	size = sizeof(header->e_machine);
	temp = read_bytes(fp, offset+=size, size);
	header->e_machine = btoi(temp, size); 
	free(temp);
	
	// e_version
	size = sizeof(header->e_version);
	temp = read_bytes(fp, offset+=size, size);
	header->e_version = btoi(temp, size); 
	free(temp);
	
	// e_entry
	size = sizeof(header->e_entry);
	temp = read_bytes(fp, offset+=size, size);
	header->e_entry = btoi(temp, size); 
	free(temp);
	
	// e_phoff
	size = sizeof(header->e_phoff);
	temp = read_bytes(fp, offset+=size, size);
	header->e_phoff = btoi(temp, size); 
	free(temp);
	
	// e_shoff
	size = sizeof(header->e_shoff);
	temp = read_bytes(fp, offset+=size, size);
	header->e_shoff = btoi(temp, size); 
	free(temp);
	
	// e_flags
	size = sizeof(header->e_flags);
	temp = read_bytes(fp, offset+=size, size);
	header->e_flags = btoi(temp, size); 
	free(temp);
	
	// e_ehsize
	size = sizeof(header->e_ehsize);
	temp = read_bytes(fp, offset+=size, size);
	header->e_ehsize = btoi(temp, size); 
	free(temp);
	
	// e_phentsize
	size = sizeof(header->e_phentsize);
	temp = read_bytes(fp, offset+=size, size);
	header->e_phentsize = btoi(temp, size); 
	free(temp);

	// e_phnum
	size = sizeof(header->e_phnum);
	temp = read_bytes(fp, offset+=size, size);
	header->e_phnum = btoi(temp, size); 
	free(temp);

	// e_shentsize
	size = sizeof(header->e_shentsize);
	temp = read_bytes(fp, offset+=size, size);
	header->e_shentsize = btoi(temp, size); 
	free(temp);

	// e_shnum
	size = sizeof(header->e_shnum);
	temp = read_bytes(fp, offset+=size, size);
	header->e_shnum = btoi(temp, size); 
	free(temp);

	// e_shstrndx
	size = sizeof(header->e_shstrndx);
	temp = read_bytes(fp, offset+=size, size);
	header->e_shstrndx = btoi(temp, size); 
	free(temp);

	*/
	
	fclose(fp);

	printf("* [%s]\n", header->e_ident);
	printf("* %08x\n", header->e_type);
	printf("* %08x\n", header->e_machine);
	printf("* %08x\n", header->e_version);
	printf("* %08x\n", header->e_entry);
	printf("* %08x\n", header->e_phoff);
	printf("* %08x\n", header->e_shoff);
	printf("* %08x\n", header->e_flags);
}


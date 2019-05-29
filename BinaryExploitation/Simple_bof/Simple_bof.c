#include <stdio.h>
#define FLAG_SIZE 32

/*

gcc -m32 -no-pie -fno-stack-protector -z execstack -o simple_BOF simple_BOF.c

*/
void flag(){

	FILE *fp;
	fp = fopen("./flag.txt", 'r');
	
	char flag[FLAG_SIZE];
	fgets(&flag, FLAG_SIZE, fp);
	
	printf("FLAG: %s", flag);
}

int main(){
	
	char buffer[256];
	puts("Hi! What is your name: ");
	gets(&buffer);
	return 0;
}


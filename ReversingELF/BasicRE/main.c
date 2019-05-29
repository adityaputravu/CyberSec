#include <stdio.h>
#include <sys/ptrace.h> 

struct flag{
	int flag_this[25];
};


int main(int argc, char *argv[]){
	if(argc != 2){
		puts("No");
		exit(1);
	}	

	if(ptrace(PT_TRACE_ME, 0, 1, 0) == -1){
		puts("No debuggers :P");
		exit(1);
	}
	
	if(strlen(argv[1]) < 21){
		puts("Close but no.");
		exit(1);
	}

	struct flag solution = {{185, 179, 190, 184, 132, 147, 144, 147, 160, 145, 150, 156, 154, 160, 149, 144, 157, 222, 222, 222, 130
	}};

	char *buffer = malloc(25);

	for(int i=0; i<21; ++i){
		char b = argv[1][i]^0xff;
		buffer[i] = b;
	}
	
	printf("FLAG: %s", buffer);

	return 0;
}

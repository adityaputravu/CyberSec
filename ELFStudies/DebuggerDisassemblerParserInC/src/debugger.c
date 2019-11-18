#include "debugger.h"

extern void help(){
	printf("Commands:\n[help]\t\t\tBring up this menu\n[step]\t\t\tStep through\n[continue]\t\tContinue till you finish or hit a breakpoint\n[bp] 0x[address]\tSet a breakpoint\n[rip]\t\t\tPrints the current instruction\n");
}

extern int* little_endian_array(int dword){
	int *bytes = calloc(8, sizeof(int));
	for(int i=0; i<32; i+=8)
		*(bytes+i) = ((dword >> i) & 0xff);
	return bytes;
}

extern void print_byte_array(int rip, int addr){
	int *bytes = little_endian_array(rip);
	printf("[*] %x: ", addr);
	for(int i=0; i<(8*6); i+=8){
		printf("%02x ", *(bytes + i));
	}
	puts("\n");
	free(bytes);
}

extern bool check_breakpoint(int addr, int breakpoints[]){
	for(int i=0; i<(sizeof(breakpoints)/sizeof(int)); ++i){
		if(addr == breakpoints[i]){
			return true;
		}
	}
	return false;
}

extern void debug(int pid){ 
    int icounter = 0;
    int wait_status;
    bool ptrace_called = false;
    int *breakpoints = calloc(1024, sizeof(int));
    char *input;
    char **input_split;
    struct user_regs_struct reg; // Registers
    int rip;

    wait(&wait_status);
    ptrace_called = !ptrace_called;
    help();
    while(WIFSTOPPED(wait_status)){
	ptrace(PTRACE_GETREGS, pid, NULL, &reg);
	rip = ptrace(PTRACE_PEEKDATA, pid, reg.rip, NULL);

	if (check_breakpoint(reg.rip, breakpoints)){
		printf("Breakpoint hit: 0x%llx\n", reg.rip);
	}

    	input = malloc(256);
	printf("> ");
	
	fgets(input, 256, stdin);
	fflush(stdin);
	
	// Important to detect single word commands
	*(input + strlen(input)-1)  = ' ';

	input_split = split_string(input, " ", MAX_STRINGS, MAX_STRING_LENGTH);

	if(!strcmp(input_split[0], "step")){ 
		printf("Stepping ...\n");
		if(ptrace(PTRACE_SINGLESTEP, pid, 0, 0) < 0){
			perror("Single step failed\n");
			break;
		}
		print_byte_array(rip, reg.rip);	
    		ptrace_called = true;
		++icounter;	
	}
	else if(!strcmp(input_split[0], "continue")){ 
		printf("Stepping ...\n");
		while(WIFSTOPPED(wait_status)){
			ptrace(PTRACE_GETREGS, pid, NULL, &reg);
			rip = ptrace(PTRACE_PEEKDATA, pid, reg.rip, NULL);
			if(ptrace(PTRACE_SINGLESTEP, pid, 0, 0) < 0){
				perror("Single step failed\n");
				break;
			}
			if(check_breakpoint(reg.rip, breakpoints)){
				printf("Breakpoint hit: 0x%llx\n", reg.rip);
				break;
			}
			++icounter;
			wait(&wait_status);
		}
	}
	else if(!strcmp(input_split[0], "bp")){ 

		breakpoints[array_length(breakpoints)] = strtol(input_split[1], NULL, 16);	
		printf("Breakpoint set @ 0x%x\n", breakpoints[array_length(breakpoints)-1]);

		ptrace_called = false;
	}
	else if(!strcmp(input_split[0], "rip")){
		print_byte_array(rip, reg.rip);	
		ptrace_called = false;
	}
	else if(!strcmp(input_split[0], "help")){ 
		help();
		ptrace_called = false;	
	}
	else{
		printf("Command not recognised\n");
		ptrace_called = false;
	}
		
	if (ptrace_called) 
		wait(&wait_status);
    }
    printf("\nStopped status: 0x%x\n", wait_status);
    printf("Program completed in: %d steps\n", icounter);

    // Freeing unused memory
    free(input);
    for(int i=0; i<MAX_STRINGS; ++i){
	free(input_split[i]);
    }
    free(input_split);
    free(breakpoints);
}

extern void child_execute(const char* path, char *const argv[]){
	if (ptrace(PTRACE_TRACEME, 0, 0, 0) != 0){
		perror("Error with spawning a child tracee\n");
	}
	execv(path, argv);
	
}

extern void attach(const char* path, char *const argv[]){
    int child = fork();
    switch(child){
    
	case -1:
	    printf("Error forked process not created.\n");
	    break;

	case 0:
	    //child
	    child_execute(path, argv);
	    break;

	default:
	    //parent
	    printf("Spawned pid: %d\n", child); 
	    debug(child);

	    break;
    }
}


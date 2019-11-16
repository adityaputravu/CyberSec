#include "debugger.h"

void help(){
	printf("Commands:\n[help]\t\tBring up this menu\n[step]\t\tStep through\n[continue]\tContinue till you finish or hit a breakpoint\n[bp]\t\tSet a breakpoint\n");
}

int* little_endian_array(int dword){
	static int bytes[8];
	for(int i=0; i<32; i+=8)
		bytes[i] = ((dword >> i) & 0xff);
	return bytes;
}

void print_byte_array(int rip, int addr){
	int *bytes = little_endian_array(rip);
	printf("[*] %x: ", addr);
	for(int i=0; i<(8*6); i+=8){
		printf("%02x ", *(bytes + i));
	}
	puts("\n");
}

bool check_breakpoint(int addr, int breakpoints[]){
	for(int i=0; i<(sizeof(breakpoints)/sizeof(int)); ++i){
		if(addr == breakpoints[i]){
			return true;
		}
	}
	return false;
}

void debug(int pid){ 
    int icounter = 0;
    int wait_status;
    bool ptrace_called = false;
    int bp;
    int breakpoints[1024];
    char *input;
    int *bytes;
    struct user_regs_struct reg; // Registers
    int rip;
	
    wait(&wait_status);
    ptrace_called = !ptrace_called;
    help();
    while(WIFSTOPPED(wait_status)){
	ptrace(PTRACE_GETREGS, pid, NULL, &reg);
	rip = ptrace(PTRACE_PEEKDATA, pid, reg.rip, NULL);

	if (check_breakpoint(reg.rip, breakpoints)){
		printf("Breakpoint hit: %x\n", reg.rip);
		continue;
	}

    	input = calloc(256, sizeof(char));
	printf("> ");
	scanf("%256s",input);
	if(!(input - strstr(input, "step"))){ 
		printf("Stepping ...\n");
		if(ptrace(PTRACE_SINGLESTEP, pid, 0, 0) < 0){
			perror("Single step failed\n");
			break;
		}
		print_byte_array(rip, reg.rip);	
    		ptrace_called = true;
	}
	else if(!(input - strstr(input, "continue"))){ 
		--icounter;
		while(WIFSTOPPED(wait_status)){
			ptrace(PTRACE_GETREGS, pid, NULL, &reg);
			rip = ptrace(PTRACE_PEEKDATA, pid, reg.rip, NULL);
			if(ptrace(PTRACE_SINGLESTEP, pid, 0, 0) < 0){
				perror("Single step failed\n");
				break;
			}
			if(check_breakpoint(reg.rip, breakpoints)){
				printf("Breakpoint hit: %x\n", reg.rip);
				break;
			}
			++icounter;
			wait(&wait_status);
		}
	}
	else if(!(input - strstr(input, "bp"))){
		printf("Enter breakpoint (hex) address: 0x");
		scanf("%x", &bp);
		printf("Breakpoint set @ 0x%x\n", bp);
		ptrace_called = false;
	}
	else if(!(input - strstr(input, "help"))){
		help();
		ptrace_called = false;	
		--icounter;
	}
	else{
		printf("Command not recognised\n");
		ptrace_called = false;
	}
		
	
	if (ptrace_called) 
		wait(&wait_status);
	++icounter;	
    }
    printf("\nStopped status: 0x%x\n", wait_status);
    printf("Program completed in: %d steps\n", icounter);
}

void child_execute(const char* path, char *const argv[]){
	if (ptrace(PTRACE_TRACEME, 0, 0, 0) != 0){
		perror("Error with spawning a child tracee\n");
	}
	execv(path, argv);
	
}

void attach(const char* path, char *const argv[]){
    int child = fork();
    int err;
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


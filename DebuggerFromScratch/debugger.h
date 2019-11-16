#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <stdbool.h>
#include <sys/ptrace.h>
#include <sys/wait.h>
#include <sys/user.h>

//void get_regs(int pid, struct user_regs_struct *reg);
//int get_rip(int pid, struct user_regs_struct reg);

void help();

int* little_endian_array(int dword);
void print_byte_array(int rip, int addr);

bool check_breakpoint(int addr, int breakpoints[]);
void debug(int pid); 
void child_execute(const char* path, char *const argv[]);
void attach(const char* path, char *const argv[]);

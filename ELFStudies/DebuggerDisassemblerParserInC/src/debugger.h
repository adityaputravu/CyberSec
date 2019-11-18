#ifndef debugger_h__ 
#define debugger_h__

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <stdbool.h>
#include <sys/ptrace.h>
#include <sys/wait.h>
#include <sys/user.h>

#include "strings.h"

#define MAX_STRING_LENGTH 256
#define MAX_STRINGS 	  10

//void get_regs(int pid, struct user_regs_struct *reg);
//int get_rip(int pid, struct user_regs_struct reg);

extern void help();

extern int* little_endian_array(int dword);
extern void print_byte_array(int rip, int addr);

extern bool check_breakpoint(int addr, int breakpoints[]);
extern void debug(int pid); 
extern void child_execute(const char* path, char *const argv[]);
extern void attach(const char* path, char *const argv[]);

#endif // debugger_h__

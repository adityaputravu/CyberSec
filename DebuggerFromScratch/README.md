# Debugger

I decided to code a debugger for Linux x86-64 to try and learn more about program execution

Compiling
## main
`gcc -o main main.c debugger.c`

## new (test program)
`nasm -f elf64 new.nasm`
`ld new.o -o new`

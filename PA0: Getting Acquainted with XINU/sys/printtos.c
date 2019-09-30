#include <stdio.h>
long *esp, *ebp;
void printtos() {
	printf("void printtos\(\)\n");
	asm("movl %esp, esp");
	asm("movl %ebp, ebp");
	printf("Before [%08x]:%08x\n", ebp+2,*(ebp+2));
	printf("After [%08x]:%08x\n\telement[%08x]: %08x\n\telement[%08x]: %08x\n\telement[%08x]: %08x\n\telement[%08x]: %08x\n", ebp,*ebp,esp+4,*(esp+4),esp+3, *(esp+3), esp+2, *(esp+2), esp, *(esp+1));
}


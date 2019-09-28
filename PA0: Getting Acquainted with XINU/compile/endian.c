#include <stdio.h>
int i=1;
char *c = (char*)&i; 
main () {
if (*c) { 
	printf("Little");
}
else
	printf("Big");
}

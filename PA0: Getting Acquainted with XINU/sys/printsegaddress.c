#include <stdio.h>
extern int etext, edata, end;
void printsegaddress() {
	printf("void printsegaddress\(\)\n");
	printf("Current: etext[%08x]=%08x, edata[%08x]=%08x, ebss[%08x]=%08x\n", &etext,etext,&edata,edata,&end,end);
	printf("Preceding: etext[%08x]=%08x, edata[%08x]=%08x, ebss[%08x]=%08x\n", &etext-1,*(&etext-1),&edata-1,*(&edata-1),&end-1,*(&end-1));
	printf("After: etext[%08x]=%08x, edata[%08x]=%08x, ebss[%08x]=%08x\n", &etext+1,*(&etext+1),&edata+1,*(&edata+1),&end+1,*(&end+1));
}

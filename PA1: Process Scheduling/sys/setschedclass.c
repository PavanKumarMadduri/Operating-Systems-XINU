#include<kernel.h>
#include<stdio.h>
#include<q.h>
#include<proc.h>
int epoch=10;
int linux_flag=0;
extern int sched_uler;
void setschedclass(int sched_class) {
	sched_uler = sched_class;
}
int getschedclass() {
	return sched_uler;
}

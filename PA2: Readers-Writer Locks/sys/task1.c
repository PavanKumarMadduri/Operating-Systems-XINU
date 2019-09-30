#include <conf.h>
#include <kernel.h>
#include <sem.h>
#include <lock.h>
#include <stdio.h>
#include <proc.h>

#define DEFAULT_LOCK_PRIO 20

char op1[10], op2[10];
int cnt1, cnt2;
STATWORD ps;
void reader_lock (char msg, int lck, int lprio) {
        
	kprintf ("  %c: to acquire lock\n", msg);
	lock (lck, READ, lprio);
	op1[cnt1++]=msg;
	kprintf ("  %c: acquired lock, sleep 3s\n", msg);
	sleep(2);
	kprintf ("  %c: to release lock\n", msg);
	releaseall (1, lck);
}

void write_lock (char msg, int lck, int lprio) {
	disable(ps);
	kprintf ("  %c: to acquire lock\n", msg);
	lock (lck, WRITE, lprio);
	op1[cnt1++]=msg;
	kprintf ("  %c: acquired lock, sleep 3s\n", msg);
	sleep(2);
	kprintf ("  %c: to release lock\n", msg);
	releaseall (1, lck);
	restore(ps);
}

void reader_sem (char msg, int semph) {
        
	kprintf ("  %c: to acquire semaphore\n", msg);
	wait(semph);
	op2[cnt2++]=msg;
	kprintf ("  %c: acquired semaphore, sleep 3s\n", msg);
	sleep(2);
	kprintf ("  %c: to release semaphore\n", msg);
	 signal(semph);
}

void write_sem (char msg, int semp) {
        
	kprintf ("  %c: to acquire semaphore\n", msg);
	wait(semp);
	op2[cnt2++]=msg;
	kprintf ("  %c: acquired semaphore, sleep 3s\n", msg);
	sleep(2);
	kprintf ("  %c: to release semaphore\n", msg);
	signal(semp);
}


void locktest() {

	int prA, prB, prC;
	int lck,lck2;
	
	lck=lcreate();
	lck2=lcreate();
	
	prA = create(write_lock, 2000, 30, "lock_write1", 3, 'A', lck, 20);
	prB = create(reader_lock, 2000, 25, "lock_read", 3, 'B', lck2, 20);
	prC = create(write_lock, 2000, 20, "lock_write2", 3, 'C', lck, 20);

	kprintf("\nPriority Inversion Case\n");

	resume(prC);
	resume(prA);
	sleep(2);
	resume(prB);

	sleep(5);

	ldelete(lck);
	ldelete(lck2);

	kill(prC);
	kill(prB);
	kill(prA);

	kprintf("Priority Inversion - Order in which the processes are executed - output=%s\n\n", op1);

}
void semtest() {

        int sprA, sprB, sprC;
        int sem, sem2;

	sem=screate(1);
	sem2=screate(1);

        sprA = create(write_sem, 2000, 30, "sem_write", 2, 'A', sem);
        sprB = create(reader_sem, 2000, 25, "sem_read", 2, 'B', sem2);
        sprC = create(write_sem, 2000, 20, "sem_write", 2, 'C', sem);
	
	kprintf("Semaphore Case\n");	

        resume(sprC);
        resume(sprA);
        sleep(2);
	resume(sprB);
	
	sleep(5);	

	sdelete(sem);
	sdelete(sem2);

        kill(sprC);
        kill(sprA);
        kill(sprB);

	kprintf("Semaphore - Order in which the processes are executed - output=%s\n", op2);

}

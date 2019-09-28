#define a_freemem 0
#define a_chprio 1
#define a_getpid 2
#define a_getprio 3
#define a_gettime 4
#define a_kill 5
#define a_receive 6
#define a_recvclr 7
#define a_recvtim 8
#define a_resume 9
#define a_scount 10
#define a_sdelete 11
#define a_send 12
#define a_setdev 13
#define a_setnok 14
#define a_screate 15
#define a_signal 16
#define a_signaln 17
#define a_sleep 18
#define a_sleep10 19
#define a_sleep100 20
#define a_sleep1000 21
#define a_sreset 22
#define a_stacktrace 23
#define a_suspend 24
#define a_unsleep 25
#define a_wait 26

int start_flag;

extern int ctr1000;

typedef struct sample {
	int ctr;
	int timer;
	char* name;
} info;

info proc_timer[NPROC][27];

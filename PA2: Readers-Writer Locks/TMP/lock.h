#define	NLOCKS	50
#define LFREE   '\01'           /* this semaphore is free               */
#define LUSED   '\02'           /* this semaphore is used               */
#define	READ	2
#define	WRITE	1
#define isbadlock(l)     (l<0 || l>=NLOCKS)

struct  lentry  {               /* lock table entry                */
        char    lstate;         /* the state LFREE or LUSED             */
        int     lqhead;         /* q index of head of list              */
        int     lqtail;         /* q index of tail of list              */
	int	ltype;		/* READ or WRITE Lock */
	int 	lckcnt;		/* Number of processes curretly acquired this lock */
	int	lproc[50];	/* List of processes and type for each process */
	int 	lprio;		/* maximum priority among all the processes waiting in the lock’s wait queue. */
};
extern  struct  lentry  locks[];
extern  int     nextlock;

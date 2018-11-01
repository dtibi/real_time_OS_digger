/* kill.c - kill */
/* 8086 version */

#include <conf.h>
#include <kernel.h>
#include <proc.h>
#include <sem.h>
#include <mem.h>

/*------------------------------------------------------------------------
 *  kill  --  kill a process and remove it from the system
 *------------------------------------------------------------------------
 */
SYSCALL kill(pid)
	int	pid;			/* process to kill		*/
{
	struct	pentry	*pptr;		/* points to proc. table for pid*/
	int	ps;			/* saved processor status	*/

	disable(ps);
	if (isbadpid(pid) || (pptr = &proctab[pid])->pstate==PRFREE) {
		restore(ps);
		return(SYSERR);
	}
	if (--numproc == 0)
		xdone();
	freestk(pptr->pbase, pptr->plen);
	switch (pptr->pstate) {

	case PRCURR:	pptr->pstate = PRFREE;	/* suicide */
			resched();

	case PRWAIT:	semaph[pptr->psem].semcnt++;
						/* fall through */
	case PRSLEEP:
	case PRREADY:	dequeue(pid);
						/* fall through */
	default:	pptr->pstate = PRFREE;
	}
	restore(ps);
	return(OK);
}

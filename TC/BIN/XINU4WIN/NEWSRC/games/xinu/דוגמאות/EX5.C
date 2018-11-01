/* ex5.c - xmain, prod2, cons2 */

#include <conf.h>
#include <kernel.h>
#include <time.h>
#include <stdio.h>

int	n=0;		/* external variables are shared by all processes */
time_t t1,t2;
/*------------------------------------------------------------------------
 *  xmain  --  producer and consumer processes synchronized with semaphores
 *------------------------------------------------------------------------
 */
xmain()
{
	int	prod2(), cons2();
	int	produced, consumed;

/*
	time_t time(time_t *tloc);
*/	
	
	(void) time(&t1);
	consumed = screate(0);
	// screate  --  create and initialize a semaphore, returning its id
	
	
	produced = screate(1);
	resume(create(prod2,INITSTK,INITPRIO,"prod",2,consumed,produced));
	resume(create(cons2,INITSTK,INITPRIO,"cons",2,consumed,produced));
	(void) time(&t2);
	printf("\nxmain end Time = %d secondsn\n", (int) t2-t1);
}

/*------------------------------------------------------------------------
 *  prod2  --  increment n 2000 times, waiting for it to be consumed
 *------------------------------------------------------------------------
 */
prod2(consumed,produced)
int consumed,produced;
{
	int	i;

	for (i=1; i<=2000; i++) {
		wait(consumed); 
		/*	syscall wait  ( semaphore  sem   ) 
			if ( --sptr->semcnt < 0 ) Make current process wait on a semaphore
			Parameters: sem - target semaphore  
			Returns: OK = 1 on success, SYSERR = -1 on failure  
		*/
		n++;
		signal(produced);
		/*	syscall signal  ( semaphore  sem   ) 
			if ( sptr->semcnt++ < 0 ) signal a semaphore, releasing one waiting thread 
			Parameters: sem  target semaphore  
			Returns: OK on success, SYSERR on failure 	
		*/
	}
	(void) time(&t2);
	printf("\n prod2 end Time = %d seconds\n", (int) t2-t1);
}

/*------------------------------------------------------------------------
 *  cons2  --  print n 2000 times, waiting for it to be produced
 *------------------------------------------------------------------------
 */
cons2(consumed,produced)
int consumed,produced;
{
	int	i;

	for (i=1; i<=2000; i++) 
	{
		wait(produced);
		printf("n is %d\n",n);
		signal(consumed);
	}
	(void) time(&t2);
	printf("\n cons2 end Time = %d seconds\n", (int) t2-t1);
}

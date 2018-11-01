/* ex5a.c - xmain, prod2, cons2 - busy wait -  wastes CPU time */

#include <conf.h>
#include <kernel.h>
#include <time.h>
#include <stdio.h>


time_t t1,t2;

int	n=0;	/* external variables are shared by all processes */
volatile int     produced, consumed;


/*-----------------------------------------------------------------------
 *  xmain  --  producer and consumer processes synchronized with messages
 *-----------------------------------------------------------------------
 */
xmain()
{
	int	prod2(), cons2();
	(void) time(&t1);

	produced = 1;
	consumed = 0;
        
	resume(create(cons2,INITSTK,INITPRIO,"cons",0));
    resume(create(prod2,INITSTK,INITPRIO,"prod",0));
	(void) time(&t2);
	printf("\nxmain end Time = %d secondsn\n", (int) t2-t1);
} /* xmain */

/*--------------------------------------------------------------------
 *  prod2  --  increment n 2000 times, waiting for it to be consumed
 *--------------------------------------------------------------------
 */
prod2()
{
	int	i, k;

	for (i=1; i<=2000; i++) {
               while(consumed == 0)
                  ;
	       n++;
	       produced = 1;
               consumed = 0;
	}/* for */
	(void) time(&t2);
	printf("\n prod2 end Time = %d seconds\n", (int) t2-t1);
} /* prod2 */

/*---------------------------------------------------------------------
 *  cons2  --  print n 2000 times, waiting for it to be produced
 *---------------------------------------------------------------------
 */
cons2()
{
	int	i;

	for (i=1; i<=2000; i++) {
              while(produced == 0)
                  ;
              printf("n is %d\n", n);
              consumed = 1;
	      produced = 0;
	} /* for */
	(void) time(&t2);
	printf("\n cons2 end Time = %d secondsn\n", (int) t2-t1);

} /* cons2 */

/* clkint.c - clkint */

#include <conf.h>
#include <kernel.h>
#include <sleep.h>
#include <io.h>

/*------------------------------------------------------------------------
 *  clkint  --  clock service routine
 *  called at every clock tick and when starting the deferred clock
 *------------------------------------------------------------------------
 */
INTPROC clkint(mdevno)
int mdevno;				/* minor device number		*/
{
	int	i;

	tod++;
	if (defclk) {
		clkdiff++;
		return;
	}
	if (slnempty)
		if ( (--*sltop) <= 0 )
			wakeup();
	if ( (--preempt) <= 0 )
		resched();
}

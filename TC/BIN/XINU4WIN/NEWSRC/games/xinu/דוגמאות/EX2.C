/* ex2.c - xmain, prA, prB */

#include <conf.h>
#include <kernel.h>

/*------------------------------------------------------------------------
 *  xmain  --  example of creating processes in PC-Xinu
 *------------------------------------------------------------------------
 */
xmain()
{
	int prA(), prB();
/*
-------------------------------------------------------------------------------------------------------
tid_typ create  ( void *  procaddr,  uint  ssize,  int  priority,  char *  name,  int  nargs,  ...   ) 
typedef int  tid_typ 
typedef unsigned int  uint 

Parameters: procaddr  procedure address  
			ssize  stack size in bytes  
			priority  thread priority  
			name  name of the thread  
			nargs  number of args that follow  

Returns:	new thread ID

Create a thread to start running a procedure 
---------------------------------------------------------------------------------------------------------
syscall resume  ( tid_typ  tid   ) 
typedef int  syscall 
 
Parameters:	tid  target thread  
Returns:	priority 

Unsuspend a thread, making it ready. 
*/
	resume( create(prA, INITSTK, INITPRIO, "proc 1", 0) );
//	#define	INITSTK		512		/* initial process stack	*/
//	#define	INITPRIO	20		/* initial process priority	*/

	resume( create(prB, INITSTK, INITPRIO, "proc 2", 0) );
}

/*------------------------------------------------------------------------
 *  prA  --  repeatedly print 'A' without ever terminating
 *------------------------------------------------------------------------
 */
prA()
{
	while (1)
		putc(CONSOLE, 'A');
}

/*------------------------------------------------------------------------
 *  prB  --  repeatedly print 'B' without ever terminating
 *------------------------------------------------------------------------
 */
prB()
{
	while (1)
		putc(CONSOLE, 'B');
}

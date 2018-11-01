/* ex7.c - xmain, prod2, cons2 */

#include <conf.h>
#include <kernel.h>

int	n=0;	/* external variables are shared by all processes */
int     produced, consumed;


/*-----------------------------------------------------------------------
 *  xmain  --  producer and consumer processes synchronized with messages
 *-----------------------------------------------------------------------
 */
xmain()
{
	int	prod2(), cons2();

	consumed = create(cons2,INITSTK,INITPRIO,"cons",0);
	produced = create(prod2,INITSTK,INITPRIO,"prod",0);
	resume(consumed);
	resume(produced);
}

/*--------------------------------------------------------------------
 *  prod2  --  increment n 2000 times, waiting for it to be consumed
 *--------------------------------------------------------------------
 */
prod2()
{
	int	i, k;

	for (i=1; i<=2000; i++) {
		n++;
/*
syscall send  ( tid_typ  tid,  message  msg )
typedef int  message

Parameters:	tid  thread id of recipient  
			msg  contents of message  

Returns:	OK = 1 on success, SYSERR = -1 on failure 
 
Send a message to another thread

If the receiver process already 
has a pending message, then the send( ) call 
returns an error to the sender process
*/
                send(consumed, n);
/*
message  receive (void)
		
Returns:	message

receive - wait for a message and return it 

Each passing message is limited to one word
(the size of an integer or pointer) 
and permitted only one unreceived message per process at any time	

the receiver process is blocked if there is no message		
*/
		
		k = receive();
	}
}

/*---------------------------------------------------------------------
 *  cons2  --  print n 2000 times, waiting for it to be produced
 *---------------------------------------------------------------------
 */
cons2()
{
	int	i, msg;

	for (i=1; i<=2000; i++) {
		msg = receive();
                printf("n is %d\n", msg);
		send(produced, 1);
	}
}

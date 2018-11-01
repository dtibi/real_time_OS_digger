/* ex1.c - xmain */

#include <conf.h> // a definition of the constant CONSOLE = 0, and the procedures `resume', `create', and `putc' used by the program

/*------------------------------------------------------------------------
 *  xmain  --  write "hi" on the CONSOLE
 *------------------------------------------------------------------------
 */
xmain() // XINU - הפונקציה הראשית ב - xmain
// int 	xmain();		/* user's main procedure	*/
//	resume( create(xmain,INITSTK,INITPRIO,"xmain",2,argc,argv) );
{
	putc(CONSOLE,'h');// CONSOOLE = 0
	putc(CONSOLE,'i');
	putc(CONSOLE,'\n');
}

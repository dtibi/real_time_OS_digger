/* c_intr1d.c - Change Ctrl-Break interrupt handler.
   Pure C version.   */

#include <stdio.h>
#include <dos.h>

volatile int Ctrl_Break_Flag;

void interrupt (*Int27Save) (void);  /* Pointer to function */

void interrupt Ctrl_Break_Handler(void)
   {
     Ctrl_Break_Flag = 1;
     printf("C: Ctrl-Break has been pressed.\n");
   } /* Ctrl_Break_Handler */

void main(void)
 {
   Int27Save = getvect(27);               /* Preserve old pointer */
   setvect(27, Ctrl_Break_Handler);           /* Set entry to new handler */
   printf("C: Press Ctrl-Break to continue.\n");

   Ctrl_Break_Flag = 0;

   while (Ctrl_Break_Flag == 0)
       ;   /* Do nothing */

   printf("C: Terminating ...\n");

   setvect(27,Int27Save);               /* Restore old pointer */

 } /* main */


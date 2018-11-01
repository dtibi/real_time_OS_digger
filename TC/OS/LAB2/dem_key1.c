/* dem_key1.c - demostrate use of int 16h, AH = 0  */

#include <stdio.h>

void main(void)
 {

   unsigned int scan_code;
   char scan_temp, inp_char;

   printf("\nPress ESC to exit program\n");

   do {
	printf("Press any key (almost)\n:");

	asm  {
	       MOV AH,0           ; /* BIOS read char from buffer option */
	       INT 16h            ; /* BIOS read char from buffer        */
	       MOV scan_temp,AH   ; /* Transfer scan code to program     */
	       MOV inp_char,AL    ; /* Transfer char to program          */
	     }

	scan_code = (unsigned int) scan_temp;

	if (scan_code == 1)
	    printf("You pressed ESC, \n");

	printf("You pressed key assigned"
              " scan code = %d, char_value= '%c'\n",
				    scan_code,  inp_char);


     } while(!(scan_code == 1));


 } /* main */

#include <stdio.h>
#include <dos.h>

void interrupt (*Int9Save) (void);

unsigned char scan;
unsigned int scan_code;
unsigned int pressed=0;


void interrupt h9(void)
{
	if (pressed){
		asm {
			jmp skipall
		}
	}
	asm{
	in 	al,60h    //input
	mov 	scan, al  // scan code
	test 	al,80h
	jnz   	rls_key
	}
	pressed = 1;
rls_key: //if key rls
	asm{
	in   al,61h
	or   al,80h
	out  61h,al
	and  al,7Fh
	out  61h,al
	mov   al,20h //end int
	out   20h,al
	}
	skipall:
}
//main
void main(void)
{
	clrscr();
	pressed=0;
	Int9Save = getvect(9); // save old func
	setvect(9, h9);    // new func

	printf("C: Press Ctrl-Break to continue.\n");
	printf("\nPress ESC to exit program\n");

	do {
		printf("Press any key (almost)\n: ");

		while(!pressed);
		scan_code = (unsigned int) scan;
		if (scan_code == 1)
		printf("You pressed ESC, \n");
		printf("You pressed key assigned: scan code = %d ", scan_code);
		pressed=0;
	} while(!(scan_code == 1));
	setvect(9,Int9Save);  // old func
}

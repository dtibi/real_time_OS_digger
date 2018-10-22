#include <stdio.h>
#include <dos.h>
#include "myints.h"

volatile unsigned char scan;
void interrupt MyISR9(void)
{
	asm{
		//PUSHF
		//CALL DWORD PTR Int9Save
		
		in al, 60h    //input
		mov scan, al  // scan code
		
		in al, 61h
		or al, 80h
		out 61h, al
		and al, 7Fh
		out 61h, al
		mov al, 20h //end int
		out 20h, al
	}
}
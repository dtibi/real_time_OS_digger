#include <stdio.h>
#include <dos.h>
#include "myints.h"
#include "map.h"

volatile unsigned char scan;
volatile unsigned char timer;
void interrupt (*Int9Save) (void);
void interrupt (*Int8Save) (void);

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
	
	if (scan==46) {
		clean_screen();
		xdone();
	}
	
	else
		move_digger(&player);
}

void interrupt MyISR8(void)
{
	Int8Save();
	timer++;
	if(timer%180)timer=0;
}
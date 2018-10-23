#include <stdio.h>
#include <dos.h>
#include <conf.h>
#include <kernel.h>
#include <io.h>
#include <bios.h>
#include "myints.h"
#include "map.h"

volatile unsigned char scan;
volatile unsigned char ascii;
volatile unsigned char timer;
volatile unsigned int receiver_pid;

void interrupt (*Int9Save) (void);
void interrupt (*Int8Save) (void);


INTPROC MyISR9(int mdevno)
{
	char result = 0;
	int scan = 0;
	int ascii = 0;

	asm {
	  MOV AH,1
	  INT 16h
	  JZ Skip1
	  MOV AH,0
	  INT 16h
	  MOV BYTE PTR scan,AH
	  MOV BYTE PTR ascii,AL
	} //asm
	if (scan==46) {
		clean_screen();
		xdone();
	}

	send(receiver_pid, scan); 

	Skip1:

} // new_int9


void set_new_int9_newisr()
{
  int i;
  for(i=0; i < 32; i++)
    if (sys_imp[i].ivec == 9)
    {
     sys_imp[i].newisr = MyISR9;
     return;
    }

} // set_new_int9_newisr

void interrupt MyISR9a_old(void)
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
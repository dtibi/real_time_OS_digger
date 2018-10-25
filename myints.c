#include <stdio.h>
#include <dos.h>
#include <conf.h>
#include <kernel.h>
#include <io.h>
#include <bios.h>
#include "myints.h"
#include "map.h"

volatile unsigned char scan;
volatile unsigned char timer;
volatile unsigned int map_move_digger_pid, digger_move_pid, map_debug_pid, terminate_xinu_pid;

void interrupt (*Int9Save) (void);
void interrupt (*Int8Save) (void);


INTPROC MyISR9(int mdevno)
{
	int scan = 0;
	asm {
	  MOV AH,1
	  INT 16h
	  JZ Skip1
	  MOV AH,0
	  INT 16h
	  MOV BYTE PTR scan,AH
	} //asm
	if (scan==46) {
		clean_screen();
		send(terminate_xinu_pid,0);
	}

	send(digger_move_pid, scan); 

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


void interrupt MyISR8(void)
{
	Int8Save();
	timer++;
	if(timer%180)timer=0;
}
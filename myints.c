#include <stdio.h>
#include <dos.h>
#include <conf.h>
#include <kernel.h>
#include <io.h>
#include "myints.h"
#include "map.h"


volatile int count,delay_timer;
volatile unsigned char scan;
volatile unsigned int map_moves_pid, digger_move_pid, debug, terminate_xinu_pid, move_enemys_pid,bg_sound_pid;
volatile int keep_playing=1;


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


void interrupt MyISR8(void)
{
    count++;
    asm {
        MOV AL,20h
        OUT 20h,AL
    }
	
	if (count>=delay_timer) send(bg_sound_pid,0);

} // newint8(void)


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

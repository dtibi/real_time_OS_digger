#include <stdio.h>
#include <dos.h>
#include <conf.h>
#include <kernel.h>
#include <io.h>
#include "myints.h"
#include "map.h"
#include "sound.h"


volatile unsigned long count,delay_timer;
volatile unsigned int map_moves_pid, digger_move_pid, debug, terminate_xinu_pid, move_enemys_pid,bg_sound_pid;


INTPROC (*Int9Save)(int);
INTPROC (*Int8Save) (int);


INTPROC MyISR9(int mdevno)
{	
	int scan = 0;
	asm {
	  PUSH AX
	  MOV AH,1
	  INT 16h
	  JZ Skip1
	  MOV AH,0
	  INT 16h
	  MOV BYTE PTR scan,AH
	  POP AX
	} //asm
	//sprintf(debug_str,"scan= %d ,",scan);
	//send(debug,debug_str);
	if (scan==46) {	
		clean_screen();
		send(terminate_xinu_pid,0);
	}
	if (scan==1) {	
		if(bg_sound_pid==-1) {
			resume(bg_sound_pid = create(background_music,INITSTK,INITPRIO-3,"background_sounds",0));
		} else {
			kill(bg_sound_pid);
			no_sound();
			bg_sound_pid=-1;
		}
	}
	send(digger_move_pid, scan); 
	asm {
		jmp Skip2
	}
	Skip1:
	asm {
		POP AX
	}
	Skip2:
	
	return (OK);

} // new_int9


INTPROC MyISR8(int mdevno)
{
    Int8Save(mdevno);
	count++;
	
	//if (count>=delay_timer) send(bg_sound_pid);
	
	if(count%((TIME_TIK*7)/10)==0)send(digger_move_pid,TIME_TIK);
	if(count%TIME_TIK==0)send(move_enemys_pid,TIME_TIK);
	if(count>=delay_timer){
		delay_timer=count+LETCH;
		send(bg_sound_pid);
	}
	
	if (count>=LETCH){
		count=0;
		delay_timer=0;
	}
	//sprintf(debug_str,"count= %6d   delay_timer=%d",count,delay_timer);
	//send(debug,debug_str);
	return(OK);
} // newint8(void)

void restore_ints(){
	int i;
	for(i=0; i < 32; i++){
		if (sys_imp[i].ivec == 9){
			sys_imp[i].newisr = Int9Save;
		}
		if (sys_imp[i].ivec == 8){
			sys_imp[i].newisr = Int8Save;
		}
	}
}

void setup_interrupts(){
	//setup latch to LETCH assuming a second is worth 1069 interrupts8
	int i;
	asm {
	  CLI
	  PUSH AX
	  MOV AL,036h
	  OUT 43h,AL
	  MOV AX,LETCH
	  OUT 40h,AL
	  MOV AL,AH
	  OUT 40h,AL 
	  POP AX
	  STI
	} // asm
	
	for(i=0; i < 32; i++){
		if (sys_imp[i].ivec == 9){
			Int9Save = sys_imp[i].newisr;
			sys_imp[i].newisr = MyISR9;
		}
		if (sys_imp[i].ivec == 8){
			Int8Save = sys_imp[i].newisr;
			sys_imp[i].newisr = MyISR8;
		}
	}
	
	

}

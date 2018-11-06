#include "myints.h"
#include "map.h"
#include "sound.h"

int dig_uppid,mon_uppid, dispid, recvpid, nob_uppid,debug,gold_falling_pid,sound_effects_pid,score_lives_pid,terminate_xinu_pid,nobbin_creator_pid,receiver_pid;
long time_from_start=0;
int num_of_pids,pressed_flag=0,pressed=0;

INTPROC (*Int9Save)(int);
INTPROC (*Int8Save) (int);

INTPROC MyISR9(int mdevno) {	
	char scan=0,ascii=0,result=mdevno;
	disp_draw_pixel_with_char(0,71,RED_BG, ' ');
	asm {
	  PUSH AX
	  MOV AH,1
	  INT 16h
	  JZ Skip1
	  MOV AH,0
	  INT 16h
	  MOV BYTE PTR scan,AH
	  MOV BYTE PTR ascii,AL
	  POP AX
	} //asm
	
	if (scan == LEFT_ARROW) result = LEFT_ARROW;
	else if (scan == UP_ARROW) result = UP_ARROW;
	else if (scan == RIGHT_ARROW) result = RIGHT_ARROW;
	else if (scan == DOWN_ARROW) result = DOWN_ARROW;
	else if (scan == SPACE_BAR) result = SPACE_BAR;
	else if (scan == 46) {
		send(terminate_xinu_pid);
		return(OK);
	}
	else result = -1;
	
	if(scan==3){
		setup_clean_screen();
		send(butlerpid, MSGPSNAP);
	}
	Skip1:
	if(pressed_flag==1 && scan==0) result=0;
	if (scan==0) pressed_flag=1;
	else pressed_flag=0; 
	if(result!=-1) send(receiver_pid, result); 
	disp_draw_pixel_with_char(0,71,BLACK_BG, ' ');
	return (OK);

} // new_int9


INTPROC MyISR8(int mdevno) {
    Int8Save(mdevno);
	time_from_start++;
	
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


void kill_xinu(int* sched_arr_pid ){
	int i;
	receive();
	for(i=0; i < num_of_pids; i++){
		kill(sched_arr_pid[i]);
	} 
	sleept(SECONDT);
	no_sound();
	setup_clean_screen();
	xdone();
	asm INT 27; // terminate xinu
	return;
}


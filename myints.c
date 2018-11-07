#include "myints.h"
#include "map.h"
#include "sound.h"

int dig_uppid, mon_uppid, dispid, recvpid, nob_uppid, debug, gold_falling_pid, sound_effects_pid, score_lives_pid, terminate_xinu_pid, nobbin_creator_pid, receiver_pid;
long tod = 0;
int num_of_pids, pressed_flag=0, pressed=0;

INTPROC (*Int9Save)(int);

//ISR 9 handler- only when pressed get the scancode of the key 
INTPROC MyISR9(int mdevno) {	
	char scan = 0, ascii = 0, result = mdevno;
	disp_draw_pixel_with_char(0, 71, RED_BG, ' ');
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
	}
	
	//check if one of the arrows key or space was pressed
	if (scan == LEFT_ARROW || scan == UP_ARROW || scan == RIGHT_ARROW || scan == DOWN_ARROW || scan == SPACE_BAR) result = scan;
	else if (scan == 46) {
		send(terminate_xinu_pid);
		return(OK);
	}
	
	//check if 'c' was clicked
	else if(scan == 3){
		setup_clean_screen();
		send(butlerpid, MSGPSNAP);
	}
	
	else result = -1;
	
	Skip1:
	if(pressed_flag == 1 && scan == 0) result = 0;
	if (scan == 0) pressed_flag=1;
	else pressed_flag=0; 
	
	if(result != -1) send(receiver_pid, result); //send the scan code if it's not -1
	
	disp_draw_pixel_with_char(0,71,BLACK_BG, ' ');
	return (OK);

}

//restore the interupts to original
void restore_ints() {
	int i;
	
	for(i = 0; i < 32; i++) {
		if (sys_imp[i].ivec == 9) sys_imp[i].newisr = Int9Save;
	}
	
	asm {
	  CLI
	  PUSH AX
	  MOV AL,036h
	  OUT 43h,AL
	  MOV AX,65500
	  OUT 40h,AL
	  MOV AL,AH
	  OUT 40h,AL 
	  POP AX
	  STI
	}
}

//setup the ISRs handlers
void setup_interrupts() {
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
	}
	
	for(i=0; i < 32; i++) {
		if (sys_imp[i].ivec == 9) {
			Int9Save = sys_imp[i].newisr; //save the original ISR
			sys_imp[i].newisr = MyISR9; //set the MyISR9 as the ISR 9
		}
	}
}

//execute all the processes, restore the ISR and shut program off
void kill_xinu(int* sched_arr_pid ) {
	int i;
	
	receive();
	for(i=0; i < num_of_pids; i++)
		kill(sched_arr_pid[i]);
	
	sleept(SECONDT);
	disp_draw_game_over();
	background_music();
	no_sound();
	setup_clean_screen();
	restore_ints();
	xdone();
	asm INT 27; // terminate xinu
	return;
}

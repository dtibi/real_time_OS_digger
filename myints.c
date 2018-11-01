#include "myints.h"
#include "map.h"
#include "sound.h"


volatile unsigned long count,delay_timer;
int uppid, dispid, recvpid, debug,gold_falling_pid,sound_effects_pid,score_lives_pid;
int receiver_pid;



INTPROC (*Int9Save)(int);
INTPROC (*Int8Save) (int);


INTPROC MyISR9(int mdevno)
{	
	int scan = mdevno,ascii,result;
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
	else result = 0;
	if (scan == 46 && ascii==3) {
		setup_clean_screen();
		asm INT 27; // terminate xinu
	}
	
	if(result!=0)send(receiver_pid, result); 
	//if(scan==3)send(sound_effects_pid,0);
	Skip1:
	return (OK);

} // new_int9


/* INTPROC MyISR8(int mdevno)
{
    Int8Save(mdevno);
	count++;
	
	//if (count>=delay_timer) send(bg_sound_pid);
	
	if(count%((TIME_TIK*7)/10)==0)send(digger_move_pid,TIME_TIK);
	if(count%TIME_TIK==0){send(move_enemys_pid,TIME_TIK);send(gold_falling_pid);}
	if(count>=delay_timer){
		delay_timer=count+LETCH;
		send(sound_effects_pid);
	}
	
	if (count>=LETCH){
		count=0;
		delay_timer=0;
	}
	//sprintf(debug_str,"count= %6d   delay_timer=%d",count,delay_timer);
	//send(debug,debug_str);
	return(OK);
} // newint8(void)
 */
void restore_ints(){
	int i;
	for(i=0; i < 32; i++){
		if (sys_imp[i].ivec == 9){
			sys_imp[i].newisr = Int9Save;
			return;
		}
	}
}

void setup_interrupts(){
	//setup latch to LETCH assuming a second is worth ?? interrupts8
	int i;
	/* asm {
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
	} // asm */
	
	for(i=0; i < 32; i++){
		if (sys_imp[i].ivec == 9){
			Int9Save = sys_imp[i].newisr;
			sys_imp[i].newisr = MyISR9;
			return;
		}
	}
	
	

}

#include "sound.h"
#include "myints.h"


void my_delay(int n, int type)
{
	int delay;
	
	if(type == 0)				
		delay = SHORT_DELAY;  // delay between two same notes
	else
		delay = LONG_DELAY;   // delay between two different notes
	
    sleept(n*delay);
	

} //mydelay



/*------------------------------------------------
 ChangeSpeaker - Turn speaker on or off. */

void change_speaker(int status)
{
    int portval;
//   portval = inportb( 0x61 );
    portval = 0;
    asm {
        PUSH AX
        MOV AL,61h
        MOV byte ptr portval,AL
        POP AX
    }

    if ( status==ON )
     portval |= 0x03;
    else
     portval &=~ 0x03;
        // outportb( 0x61, portval );
        asm {
          PUSH AX
          MOV AX,portval
          OUT 61h,AL
          POP AX
        } // asm

} /*--ChangeSpeaker( )----------*/

void sound(int hertz)
{
	unsigned divisor = 1193180L / hertz;

	change_speaker( ON );

   //        outportb( 0x43, 0xB6 );
    asm {
        PUSH AX
        MOV AL,0B6h
        OUT 43h,AL
        POP AX
    } // asm


     //       outportb( 0x42, divisor & 0xFF ) ;
    asm {
        PUSH AX
        MOV AX,divisor
        AND AX,0FFh
        OUT 42h,AL
        POP AX
    } // asm


     //        outportb( 0x42, divisor >> 8 ) ;

    asm {
        PUSH AX
        MOV AX,divisor
        MOV AL,AH
        OUT 42h,AL
        POP AX
    } // asm

} /*--Sound( )-----*/

void no_sound()
{
    change_speaker( OFF );

} /*--NoSound( )------*/


void background_music_test(){
	
	//int i;
	
	my_delay(1,0);
	//sound(A4S);
	//my_delay(2,1);
    //no_sound();
}

void background_music(){
	
	int i;
	
	while(1){
		
		my_delay(1,0);
		sound(A4S);
		my_delay(2,1);
		
		for(i = 0; i < 2; i++){
			no_sound();
			my_delay(2,1);
			sound(A4S);
			my_delay(1,1);
			no_sound();
			my_delay(2,0);
			sound(A4S);
			my_delay(1,1);
			no_sound();
			my_delay(1,0);
			sound(A4S);
			my_delay(1,1);
			sound(G4S);
			my_delay(1,1);
			sound(A4S);
			my_delay(1,1);
			no_sound();
			my_delay(2,0);
			sound(A4S);
			my_delay(2,1);
		}
		
		no_sound();
		my_delay(2,1);
		
		sound(C5S);
		my_delay(2,1);
		sound(A4S);
		my_delay(1,1);
		no_sound();
		my_delay(1,1);
		sound(G4S);
		my_delay(2,1);
		sound(F4S);
		my_delay(1,1);
		
		no_sound();
		my_delay(1,1);
		
		sound(D4S);
		my_delay(1,1);
		no_sound();
		my_delay(2,0);
		sound(D4S);
		my_delay(1,1);
		sound(F4);
		my_delay(1,1);
		sound(F4S);
		my_delay(1,1);
		no_sound();
		my_delay(1,0);
		sound(D4S);
		my_delay(1,1);
		
		no_sound();
				
	}
}

void beethoven(){
	while(1){
		sound(E4);
		my_delay(1,1);
		no_sound();
		my_delay(1,0);
		sound(E4);
		my_delay(1,1);
		sound(F4);
		my_delay(1,1);
		sound(G4);
		my_delay(1,1);
		no_sound();
		my_delay(1,0);
		sound(G4);
		my_delay(1,1);
		sound(F4);
		my_delay(1,1);
		sound(E4);
		my_delay(1,1);
		sound(D4);
		my_delay(1,1);
		sound(C4);
		my_delay(1,1);
		no_sound();
		my_delay(1,0);
		sound(C4);
		my_delay(1,1);
		sound(D4);
		my_delay(1,1);
		sound(E4);
		my_delay(1,1);
		no_sound();
		my_delay(1,0);
		sound(E4);
		my_delay(1,1);
		sound(D4);
		my_delay(1,1);
		no_sound();
		my_delay(1,0);
		sound(D4);
		my_delay(1,1);
		no_sound();
		my_delay(1,1);
		
		sound(E4);
		my_delay(1,1);
		no_sound();
		my_delay(1,0);
		sound(E4);
		my_delay(1,1);
		sound(F4);
		my_delay(1,1);
		sound(G4);
		my_delay(1,1);
		no_sound();
		my_delay(1,0);
		sound(G4);
		my_delay(1,1);
		sound(F4);
		my_delay(1,1);
		sound(E4);
		my_delay(1,1);
		sound(D4);
		my_delay(1,1);
		sound(C4);
		my_delay(1,1);
		no_sound();
		my_delay(1,0);
		sound(C4);
		my_delay(1,1);
		sound(D4);
		my_delay(1,1);
		sound(E4);
		my_delay(1,1);
		sound(D4);
		my_delay(1,1);
		no_sound();
		my_delay(1,0);
		sound(C4);
		my_delay(1,1);
		no_sound();
		my_delay(1,0);
		sound(C4);
		my_delay(1,1);
		no_sound();
		my_delay(1,1);
	}
}

void play_death(){
	//1
	sound(C4);
	my_delay(3,1);
	no_sound();
	my_delay(0,0);
	//2
	sound(C4);
	my_delay(2,1);
	no_sound();
	my_delay(0,0);
	//3
	sound(C4);
	my_delay(1,1);
	no_sound();
	my_delay(0,0);
	//4
	sound(C4);
	my_delay(3,1);
	no_sound();
	my_delay(0,0);
	//5
	sound(D4S);
	my_delay(2,1);
	no_sound();
	my_delay(0,1);
	//6
	sound(D4);
	my_delay(1,1);
	no_sound();
	my_delay(0,0);
	//7
	sound(D4);
	my_delay(2,1);
	no_sound();
	my_delay(0,0);
	//8
	sound(C4);
	my_delay(1,1);
	no_sound();
	my_delay(0,0);
	//9
	sound(C4);
	my_delay(2,1);
	no_sound();
	my_delay(0,0);
	//10
	sound(BB4);
	my_delay(1,1);
	no_sound();
	my_delay(0,0);
	//11
	sound(C4);
	my_delay(2,1);
	no_sound();
	my_delay(1,1);
}

void sound_effects(){
	int sound_id;
	while(1){
		sound_id = -1;
		sound_id = receive();
		
		if(sound_id==0) play_death();
		
	}
}

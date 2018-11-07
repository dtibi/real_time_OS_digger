#include "sound.h"
#include "myints.h"

void my_delay(int n, int type) {
	int delay;
	
	if(type == 0) delay = SHORT_DELAY;  // delay between two same notes
	else delay = LONG_DELAY;   // delay between two different notes
	
	sleept(n * delay);
}


 //ChangeSpeaker - Turn speaker on or off
void change_speaker(int status) {
    int portval;

    portval = 0;
    asm {
        PUSH AX
        MOV AL,61h
        MOV byte ptr portval,AL
        POP AX
    }

    if (status == ON )
		portval |= 0x03;
    else
		portval &=~ 0x03;
    
	asm {
		PUSH AX
        MOV AX, portval
        OUT 61h,AL
        POP AX
	}
}

void sound(int hertz) {
	unsigned divisor = 1193180L / hertz;
	change_speaker(ON);

    asm {
        PUSH AX
        MOV AL,0B6h
        OUT 43h,AL
        POP AX

        PUSH AX
        MOV AX,divisor
        AND AX,0FFh
        OUT 42h,AL
        POP AX

        PUSH AX
        MOV AX,divisor
        MOV AL,AH
        OUT 42h,AL
        POP AX
    }
}

void background_music() {
	int i;
	
	my_delay(1,0);
	sound(A4S);
	my_delay(2,1);
	
	for(i = 0; i < 2; i++){
		change_speaker(OFF);
		my_delay(2,1);
		sound(A4S);
		my_delay(1,1);
		change_speaker(OFF);
		my_delay(2,0);
		sound(A4S);
		my_delay(1,1);
		change_speaker(OFF);
		my_delay(1,0);
		sound(A4S);
		my_delay(1,1);
		sound(G4S);
		my_delay(1,1);
		sound(A4S);
		my_delay(1,1);
		change_speaker(OFF);
		my_delay(2,0);
		sound(A4S);
		my_delay(2,1);
	}
	
	change_speaker(OFF);
	my_delay(2,1);
	
	sound(C5S);
	my_delay(2,1);
	sound(A4S);
	my_delay(1,1);
	change_speaker(OFF);
	my_delay(1,1);
	sound(G4S);
	my_delay(2,1);
	sound(F4S);
	my_delay(1,1);
	
	change_speaker(OFF);
	my_delay(1,1);
	
	sound(D4S);
	my_delay(1,1);
	change_speaker(OFF);
	my_delay(2,0);
	sound(D4S);
	my_delay(1,1);
	sound(F4);
	my_delay(1,1);
	sound(F4S);
	my_delay(1,1);
	change_speaker(OFF);
	my_delay(1,0);
	sound(D4S);
	my_delay(1,1);
	change_speaker(OFF);
	sound(A4S);
	my_delay(2,1);
	change_speaker(OFF);
	my_delay(2,1);	
}

void beethoven() {
		sound(E4);
		my_delay(1,1);
		change_speaker(OFF);
		my_delay(1,0);
		sound(E4);
		my_delay(1,1);
		sound(F4);
		my_delay(1,1);
		sound(G4);
		my_delay(1,1);
		change_speaker(OFF);
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
		change_speaker(OFF);
		my_delay(1,0);
		sound(C4);
		my_delay(1,1);
		sound(D4);
		my_delay(1,1);
		sound(E4);
		my_delay(1,1);
		change_speaker(OFF);
		my_delay(1,0);
		sound(E4);
		my_delay(1,1);
		sound(D4);
		my_delay(1,1);
		change_speaker(OFF);
		my_delay(1,0);
		sound(D4);
		my_delay(1,1);
		change_speaker(OFF);
		my_delay(1,1);
		
		sound(E4);
		my_delay(1,1);
		change_speaker(OFF);
		my_delay(1,0);
		sound(E4);
		my_delay(1,1);
		sound(F4);
		my_delay(1,1);
		sound(G4);
		my_delay(1,1);
		change_speaker(OFF);
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
		change_speaker(OFF);
		my_delay(1,0);
		sound(C4);
		my_delay(1,1);
		sound(D4);
		my_delay(1,1);
		sound(E4);
		my_delay(1,1);
		sound(D4);
		my_delay(1,1);
		change_speaker(OFF);
		my_delay(1,0);
		sound(C4);
		my_delay(1,1);
		change_speaker(OFF);
		my_delay(1,0);
		sound(C4);
		my_delay(1,1);
		change_speaker(OFF);
		my_delay(1,1);
}

//the music that will be played when the digger died
void play_death() {
	//1
	sound(C4);
	my_delay(3,1);
	change_speaker(OFF);
	my_delay(1,0);
	//2
	sound(C4);
	my_delay(2,1);
	change_speaker(OFF);
	my_delay(1,0);
	//3
	sound(C4);
	my_delay(1,1);
	change_speaker(OFF);
	my_delay(1,0);
	//4
	sound(C4);
	my_delay(3,1);
	change_speaker(OFF);
	my_delay(1,0);
	//5
	sound(D4S);
	my_delay(2,1);
	change_speaker(OFF);
	my_delay(1,0);
	//6
	sound(D4);
	my_delay(1,1);
	change_speaker(OFF);
	my_delay(1,0);
	//7
	sound(D4);
	my_delay(2,1);
	change_speaker(OFF);
	my_delay(1,0);
	//8
	sound(C4);
	my_delay(1,1);
	change_speaker(OFF);
	my_delay(1,0);
	//9
	sound(C4);
	my_delay(2,1);
	change_speaker(OFF);
	my_delay(1,0);
	//10
	sound(BB4);
	my_delay(1,1);
	change_speaker(OFF);
	my_delay(1,0);
	//11
	sound(C4);
	my_delay(2,1);
	change_speaker(OFF);
	my_delay(1,1);
}

//the sound that will play when enemy was killed
void enemy_killed_sound() {
	sound(B4);
	my_delay(2,0);
	sound(G4);
	my_delay(2,0);
	sound(B4);
	my_delay(2,0);
	change_speaker(OFF);
	my_delay(1,0);
}

//that sound that will be play when fire was shoot
void fire_sound() {
	sound(C4S);
	my_delay(2,0);
	sound(D4S);
	my_delay(2,0);
	sound(F4S);
	my_delay(2,0);
	change_speaker(OFF);
	my_delay(1,0);
}

//that sound that will be play when diamond was taken
void diamond_taken_sound() {
	sound(F4);
	my_delay(2,0);
	sound(D4);
	my_delay(2,0);
	change_speaker(OFF);
	my_delay(1,0);
}

//that sound that will be play when cherry was taken
void cherry_taken_sound() {
	sound(D4);
	my_delay(2,0);
	change_speaker(OFF);
	my_delay(1,0);
	sound(D4);
	my_delay(2,0);
	sound(A4);
	my_delay(3,0);
	change_speaker(OFF);
	my_delay(1,0);
}

//the controler of the sounds- select sound with that function
void sound_effects() {
	int sound_id;
	while(1){
		sound_id = -1;
		sound_id = receive();
		
		if(sound_id == 0) play_death();
		else if(sound_id == 1) enemy_killed_sound();
		else if(sound_id == 2) diamond_taken_sound();
		else if(sound_id == 3) cherry_taken_sound();
		else if(sound_id == 4) fire_sound();
	}
}

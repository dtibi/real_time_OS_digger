// sou3.c
#include <dos.h>
#include <stdio.h>

#define ON (1)
#define OFF (0)
#define MAX_CLICKS 200
volatile int global_saved_times[MAX_CLICKS/2];
volatile int global_i=0;
volatile int global_key_counter=0;
volatile int global_timer=0;
volatile char global_scan_code = -1;
volatile int global_debug=0;
volatile int global_latch=65;
volatile int global_speaker_flag=0;

void interrupt (*int8save)(void);
void interrupt (*int9save)(void);

void interrupt newint8(void);
void interrupt newint9(void);
void setup_interrupts();
void finilize();
void ChangeSpeaker( int status );
void setup_sound_latch();
void NoSound( void );

int main( void ){
	if(global_debug)
		printf("setting up interrupts\n");
   setup_interrupts();
   while(1);
   
} /*--main( )-------*/


void interrupt newint8(void){
   (*int8save)();
   global_timer++;
   if(global_timer>110){
	   printf("second passed\n");
	   NoSound();
	   global_speaker_flag=0;
	   global_timer=0;
   } else {
		if(global_speaker_flag)
			setup_sound_latch();
   }
} // newint8(void)

void interrupt newint9(void){
	char sc,ascii;
	if(global_debug)
			printf("got into interrupt 9\n");
	(*int9save)();
	asm {
	  MOV AH,1
	  INT 16h
	  JZ Skip1
	  MOV AH,0
	  INT 16h
	  MOV BYTE PTR ascii,AL
	  MOV BYTE PTR sc,AH
	}
	
	printf("%c\n",ascii);
	
	if(ascii == 'c') 
		global_latch=65;
	else if (ascii=='d') 
		global_latch=73;
	else if (ascii=='e') 
		global_latch=82;
	else if (ascii=='f') 
		global_latch=87;
	else if (ascii=='g') 
		global_latch=98;
	else if (ascii=='a') 
		global_latch=110;
	else if (ascii=='b') 
		global_latch=123;
	else if (ascii=='q') {
		finilize();
	}
	global_timer=0;
	global_speaker_flag=1;
	Skip1: // release

} // newint9(void)


void setup_interrupts(){
	//setup latch to 700 assuming a second is worth 1069 interrupts8
	if(global_debug)
			printf("starting asm code in setup\n");
		
        asm {
          CLI
          PUSH AX
          MOV AL,036h
          OUT 43h,AL
          MOV AX,9700
          OUT 40h,AL
          MOV AL,AH
          OUT 40h,AL 
          POP AX
		  STI
        } // asm
	
	if(global_debug)
			printf("finished asm code in setup\n");
		
	int8save = getvect(8);
    setvect(8,newint8);
	
	
	if(global_debug)
			printf("done setting int 8\n");
		
	//setting up new int9 for keypress
	int9save = getvect(9);
    setvect(9,newint9);
	
	if(global_debug)
			printf("done setting int 9\nsetup Done\n");
}

void finilize(){
	setvect(8,int8save);
	setvect(9,int9save);
	NoSound();
	asm {
		mov AH,4Ch
		int 21h
	}
}

void ChangeSpeaker( int status ) {
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

void setup_sound_latch(){
	unsigned divisor = 1193180L / global_latch;

	ChangeSpeaker( ON );
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

void NoSound( void ){
	 ChangeSpeaker( OFF );
} /*--NoSound( )------*/

#include <dos.h>
#include <stdio.h>
#include "sound.h"

volatile int count;

void interrupt newint8(void)
{
    count++;
    asm {
        MOV AL,20h
        OUT 20h,AL
    }

} // newint8(void)

void interrupt (*int8save)(void);

void my_delay(int n)
{
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
    } // asm

    int8save = getvect(8);
    setvect(8,newint8);
    asm  { STI
	};
    count = 0;
    while(count <= n*110)
        ;

        asm {
          CLI
          PUSH AX
          MOV AL,036h
          OUT 43h,AL
          MOV AX,0
          OUT 40h,AL
          MOV AL,AH
          OUT 40h,AL 
          POP AX
        } // asm


    setvect(8,int8save);


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

void background_music(){
	
//	PlaySound("C:\\XINU4WIN\\NEWSRC\\digger\\Gold_Digger.wav", NULL, SND_ASYNC);
	
	sound( 355 );
    my_delay( 2 );
    sound( 533 );
    my_delay( 2 );
    sound( 755 );
    my_delay( 2 );
    sound( 355 );
    my_delay( 2 );
	sound( 533 );
    my_delay( 2 );
	sound( 755 );
    my_delay( 2 );
	
    no_sound();
}
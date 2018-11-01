// sou3.c
#include <dos.h>
#include <stdio.h>

#define ON (1)
#define OFF (0)

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

void mydelay(int n)
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
    asm  { STI};
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

 void ChangeSpeaker( int status )
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

	void Sound( int hertz )
	{
	 unsigned divisor = 1193180L / hertz;

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

      void NoSound( void )
        {
             ChangeSpeaker( OFF );
        } /*--NoSound( )------*/

       int main( void )
        {
           Sound( 355 );
           mydelay( 2 );
           Sound( 733 );
           mydelay( 2 );
           Sound( 355 );
           mydelay( 2 );
           Sound( 733 );
           mydelay( 2 );

           NoSound( );
           return(0);
        } /*--main( )-------*/

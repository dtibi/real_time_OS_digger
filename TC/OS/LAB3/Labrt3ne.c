#include <dos.h>
#include <stdio.h>

#define ON (1)
#define OFF (0)
#define PRESSED_KEYS 20

volatile int time_count = 0;
volatile int pressed_count = 0;
char saved_key;
char ScanCode;
volatile int saved_flag = 0;
volatile int key_clicked = 0;
volatile int latch = 1;
volatile int latchArr[PRESSED_KEYS / 2];
volatile int latchArrPointer = 0;

long max_time = 0, min_time = 0, med_time = 0, total_time = 0;

void interrupt (*int8save)(void);
void interrupt (*int9save)(void);

void interrupt newint8(void)
{

   time_count++;
   asm {
        MOV AL,20h
        OUT 20h,AL
       }
} // newint8(void)

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

void interrupt newint9(void)
{
	(*int9save)();	// calling original interrupt
	
	asm{
		in   al,60h
		MOV ScanCode,AL
		IN  AL,60h
		PUSH AX
		IN AL,61h
		OR AL,80h
		OUT 61h,AL
		AND AL,7Fh
		OUT 61h,AL
		POP AX
		TEST AL,80h
		jz  Skip1 // Jump if pressed
	}
	printf("button was released = %d\n",ScanCode);
	
		
	asm {
		MOV AH,1	// checking if buffer is empty
		INT 16h
		JZ Skip1
		MOV AH,0	// extracting the entered ascii code of the pressed key
		INT 16h
	}
	pressed_count++;
	
	if (saved_flag == 0){
		saved_key = ScanCode;
		saved_flag = 1;
		time_count = 0;
	} else if(ScanCode == saved_key){
			saved_flag=0;
			latch = time_count;
			sound(latch);
			printf("latch: %d\n",latch);
			latchArr[latchArrPointer] = latch;
			latchArrPointer++;
	 }
	 
	Skip1:
}
	
	


void printArr(){
	int i,j,temp;
	float SUM = 0,Mid_time,Max_time,Min_time;
	nosound();
	printf("timed:\n");
	for(i = 0; i < PRESSED_KEYS / 2 ;i++){
		printf("%d ",latchArr[i]);
	}
	
	for(i = 0; i < (PRESSED_KEYS / 2) - 1; i++){
		for(j = 0; j < (PRESSED_KEYS / 2) - i - 1; j++)
			if(latchArr[j] > latchArr[j+1]){
				temp = latchArr[j];
				latchArr[j] = latchArr[j+1];
				latchArr[j+1] = temp;
			}
	}

	printf("\nsorted:\n");
	for(i = 0; i < PRESSED_KEYS / 2 ;i++){
		printf("%d ",latchArr[i]);
		SUM += latchArr[i];
	}
	Min_time = latchArr[0];
	Max_time = latchArr[latchArrPointer - 1];
	//temp = (latchArrPointer / 2);
	Mid_time = latchArr[latchArrPointer / 2];
	printf("\nMax_time= %.3f seconds\nMinimum_time= %.3f seconds\n",Max_time / 1069,Min_time / 1069);
	printf("Med_time= %.3f seconds\nTotal time= %.3f seconds\n",Mid_time / 1069,SUM / 1069);
	setvect(8,int8save); /* restoring int 8*/
	setvect(9,int9save); /* restoring int 9*/
	printf("\nExiting ...\n");
	asm{	// asm exit protocol
		mov ah,4ch
		int 21h
	}
}

void init(){
	asm {
          CLI
          PUSH AX
          MOV AL,036h
          OUT 43h,AL
          MOV AX,700
          OUT 40h,AL
          MOV AL,AH
          OUT 40h,AL 
          POP AX
		  STI
        } // asm
	
	int8save = getvect(8);
	setvect(8,newint8);
	int9save = getvect(9);
	setvect(9,newint9);
}

int main(){
	
	init();
	
	while(pressed_count < PRESSED_KEYS){}
	
	printArr();
	
	return 0;
}
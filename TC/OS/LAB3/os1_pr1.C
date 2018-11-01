#include <stdio.h>
#include <conio.h>
#include <dos.h>

volatile int sum=0; /*num of int*/

void interrupt (*int8Save) (void);
char time_limited_getch ( int secs );

void interrupt h8 (void)
{
	asm{
		PUSHF
		CALL DWORD PTR int8Save
        }
		sum++; /*counter*/
}

char time_limited_getch ( int secs )
{
	volatile int flag;

	char c1;

	sum = 0;
  while(1)
  {
	asm{
	PUSH AX
	MOV AH,1
	INT 16h
	PUSHF
	POP AX
	AND AX,64  //zero flag
	MOV flag,AX
	POP AX

	} /* checking if butten was presed*/

	if( flag==0 )
	{
		asm{
		PUSH AX
		MOV AH,0
		INT 16h
		MOV c1,AL
		POP AX
		}/* reading the butten*/
		return c1;
	}
	if( sum>secs*18 )
		return 'y';
  } /* while */
}/*end time_limited_getch*/


void main()
{
	char c;
	clrscr();

	int8Save = getvect(8);
	setvect(8,h8);

	do{
	printf( "continue? (y/n)");
	c=time_limited_getch ( 7 );
	printf("\n %c %d\n",c,sum);
	}while( c != 'y');

	printf("Terminating ...");

	setvect(8,int8Save); /* reastoring int 8*/
	getch();
}/*end main*/

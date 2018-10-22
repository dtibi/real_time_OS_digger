#include <stdio.h>
#include <dos.h>
#include "digger.h"
#include "map.h"

volatile unsigned char scan;

void interrupt MyISR9(void)
{	

	asm{
		//PUSHF
		//CALL DWORD PTR Int9Save
		
		in al, 60h    //input
		mov scan, al  // scan code
		
		in al, 61h
		or al, 80h
		out 61h, al
		and al, 7Fh
		out 61h, al
		mov al, 20h //end int
		out 20h, al
	}
}

void create_digger(Digger *player)
{
	(*player).x = 7;
	(*player).y = 7;
	(*player).direction = 'l';
}

void move(Digger *player)
{
	switch (scan)
	{
		case 80: //down
			if((*player).direction != 'd')
				(*player).direction = 'd';
			else if((*player).y != ROWS)
				(*player).y--;
			break;
		
		case 72: //up
			if((*player).direction != 'u')
				(*player).direction = 'u';
			if((*player).y != 0)
				(*player).y++;
			break;
		
		case 75: //left
			if((*player).direction != 'l')
				(*player).direction = 'l';
			if((*player).x != 0)
				(*player).x--;
			break;
		
		case 77: //right
			if((*player).direction != 'r')
				(*player).direction = 'r';
			if((*player).x != COLUMNS)
				(*player).x++;
			break;
	}
}
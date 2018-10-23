#include <stdio.h>
#include <dos.h>
#include "digger.h"
#include "map.h"
#include "myints.h"

void create_digger(Digger *player)
{
	(*player).x = 40;
	(*player).y = 23;
	(*player).direction = 'l';
}

void move(Digger *player)
{	
	//moveDirection = scanCodeToDirection(scan);
	switch (scan)
	{
		case 80: //down
			if((*player).direction != 'd'){
				(*player).direction = 'd';
			}
			else if((*player).y < ROWS_PIXELS-1)
				(*player).y++;
			break;
		
		case 72: //up
			if((*player).direction != 'u')
				(*player).direction = 'u';
			else if((*player).y > 2)
				(*player).y--;
			break;
		
		case 75: //left
			if((*player).direction != 'l')
				(*player).direction = 'l';
			else if((*player).x > 2)
				(*player).x--;
			break;
		
		case 77: //right
			if((*player).direction != 'r')
				(*player).direction = 'r';
			else if((*player).x < COLUMNS_PIXELS-1)
				(*player).x++;
			break;
	}
}

/*
void changeDirection (Digger* player, scan)
{
	switch ((*player).direction)
	{
		case 'l':
			
			if (scan == RIGHT_ARROW)
			{
				x
			}
			
			else if (scan == DOWN_ARROW)
			{
				
			}
			
			else // scan == UP_ARROW
			{
				
			}
			
			break;
		
		case 'r':
			if (scan == LEFT_ARROW)
			{
				
			}
			
			else if (scan == DOWN_ARROW)
			{
				
			}
			
			else // scan == UP_ARROW
			{
				
			}
			break;
		
		case 'd':
			if (scan == LEFT_ARROW)
			{
				
			}
			
			else if (scan == RIGHT_ARROW)
			{
				
			}
			
			else // scan == UP_ARROW
			{
				
			}
			break;
		
		case 'u':
			if (scan == LEFT_ARROW)
			{
				
			}
			
			else if (scan == RIGHT_ARROW)
			{
				
			}
			
			else // scan == DOWN_ARROW
			{
				
			}
			
			break;
	}
}


int directionChange(scan, direction)
{
	char temp 
	if (scan == LEFT_ARROW)
}
*/

void move_digger(Digger *player){
		move(player);
}
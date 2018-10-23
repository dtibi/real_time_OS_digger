#include <stdio.h>
#include <dos.h>
#include "digger.h"
#include "map.h"
#include "myints.h"

//Create digger
Digger create_digger()
{
	Digger player;
	player.x = 40;
	player.y = 22;
	player.direction = LEFT_ARROW;
	
	return player;
}

void move(Digger *player,int direction)
{	
	int *XYchanges;
	if (!move_is_possible((*player).x,(*player).y,direction,1)) return;
	
	if(direction != (*player).direction) //check if the wanted move direction is diffrent from the current
	{
		XYchanges = directionChanged(direction, (*player).direction); //check what are the changes in x and y according to the prev direction and the next direction
		(*player).direction = direction; //chane direction
		(*player).x -=  XYchanges[0]; //update x
		(*player).y -=  XYchanges[1]; //update y
		return;
	}
	//printf("dx=%d - dy=%d ,",XYchanges[0],XYchanges[1]);
	switch (direction)
	{
		case LEFT_ARROW:
			(*player).x --;
			break;
		case RIGHT_ARROW:
			(*player).x ++;
			break;
		case DOWN_ARROW:
			(*player).y ++;
			break;
		case UP_ARROW:
			(*player).y --;
			break;
	}
}

int* directionChanged (int prevDirection, int nextDirection)
{
	int XYchanges[2] = {0, 0};
	switch (prevDirection)
	{
		case LEFT_ARROW:
			switch(nextDirection)
			{	
				case RIGHT_ARROW:
					XYchanges[0] = 2;
					break;
					
				case DOWN_ARROW:
					XYchanges[0] = 1;
					XYchanges[1] = -1;
					break;
					
				case UP_ARROW:
					XYchanges[0] = 1;
					XYchanges[1] = 1;
					break;
			}
			break;
		
		case RIGHT_ARROW:
			switch(nextDirection)
			{
				case LEFT_ARROW:
					XYchanges[0] = -2;
					break;
					
				case DOWN_ARROW:
					XYchanges[0] = -1;
					XYchanges[1] = 1;
					break;
					
				case UP_ARROW:
					XYchanges[0] = -1;
					XYchanges[1] = 1;
					break;
			}
			break;
		
		case DOWN_ARROW:
			switch(nextDirection)
			{
				case LEFT_ARROW:
					XYchanges[0] = -1;
					XYchanges[1] = 1;
					break;
					
				case RIGHT_ARROW:
					XYchanges[0] = 1;
					XYchanges[1] = 1;
					break;
					
				case UP_ARROW:
					XYchanges[1] = 2;
					break;
			}
			break;
		
		case UP_ARROW:
			switch(nextDirection)
			{
				case LEFT_ARROW:
					XYchanges[0] = -1;
					XYchanges[1] = -1;
					break;
					
				case RIGHT_ARROW:
					XYchanges[0] = 1;
					XYchanges[1] = -1;
					break;
					
				case DOWN_ARROW:
					XYchanges[1] = -2;
					break;
			}
			break;
	}
	return XYchanges;
}

void move_digger(Digger *player){
	while(1){
		move(player,receive());
	  } // while
		
}

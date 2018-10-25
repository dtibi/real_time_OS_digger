#include <stdio.h>
#include <dos.h>
#include "digger.h"
#include "map.h"
#include "myints.h"

//Create digger
Digger create_digger()
{
	Digger player;
	player.x = 42;
	player.y = 23;
	player.direction = LEFT_ARROW;
	
	return player;
}

void move(Digger *player,int direction)
{
	int *XYchanges;
	
	if(direction != (*player).direction) //check if the wanted move direction is diffrent from the current
		(*player).direction = direction; //TODO: add here call the redraw digger
	
	if (!move_is_possible((*player).x,(*player).y,direction, 1)) return;
	
	else{
		
		if (getNextPixelType((*player).x, (*player).y, direction) == 2) //TODO: change this so the function will get the level
		{}
	
		else if (getNextPixelType((*player).x, (*player).y, direction) == 3) //TODO: change this so the function will get the level
		{}//
		
		switch (direction)
		{
			case LEFT_ARROW:
					(*player).x -= WIDTH;
				break;
			case RIGHT_ARROW:
					(*player).x += WIDTH;
				break;
			case DOWN_ARROW:
					(*player).y +=HEIGHT;
				break;
			case UP_ARROW:
					(*player).y -=HEIGHT;
				break;
		}
	}
}

void move_digger(Digger *player){	
	int scanned=0;
	while(1){
		scanned = receive();
		sprintf(debug_str,"scanned value is - %d",scanned);
		//send(debug,debug_str);
		move(player,scanned);
		send(map_moves_pid,0);
	  }
}

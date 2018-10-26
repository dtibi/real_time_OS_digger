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
		gameMap.currentLevel[pixel_2_row((*player).y)][pixel_2_column((*player).x)] = 99;
	}
}

void move_digger(Digger *player){	
	int input=0;
	while(1){
		while(input!=TIME_TIK){
			input = receive();
		}
		while (input!=RIGHT_ARROW && input!=LEFT_ARROW && input!=UP_ARROW && input!=DOWN_ARROW){
			input = receive();
		}
		//sprintf(debug_str,"scanned value is - %d",input);
		//send(debug,debug_str);
		move(player,input);
		draw_digger(*player);
	  }
}

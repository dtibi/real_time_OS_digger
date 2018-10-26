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
	if(direction != (*player).direction) //check if the wanted move direction is diffrent from the current
		(*player).direction = direction; //TODO: add here call the redraw digger
	
	if (!move_is_possible((*player).x,(*player).y,direction, 1)) return;
	
	else{
		if (getNextPixelType((*player).x, (*player).y, direction) == 2) //diamond found
		{
			//add point to score;
		}
	
		else if (getNextPixelType((*player).x, (*player).y, direction) == 3) //gold sack found
		{
			if(direction==UP_ARROW || direction==DOWN_ARROW) 
				return;
			
			else if(direction==RIGHT_ARROW && move_is_possible((*player).x + WIDTH,(*player).y, direction, 1)) 
			{
				//draw empty
				draw_bag((*player).y/HEIGHT, ((*player).x + WIDTH*2)/WIDTH);
				gameMap.currentLevel[(*player).y/HEIGHT][((*player).x + WIDTH*2)/WIDTH] = 3;
				if(move_is_possible((*player).x + WIDTH*2,(*player).y, DOWN_ARROW, 0))//check if there is no dirt under
				{
					draw_empty((*player).y-1, (*player).x + WIDTH-2);
					gameMap.currentLevel[(*player).y/HEIGHT][((*player).x + WIDTH)/WIDTH] = 0;
					goldFalling((*player).x + WIDTH*2, (*player).y);
				}
			}
			
			else if(direction==LEFT_ARROW && move_is_possible((*player).x - WIDTH,(*player).y, direction, 1)) 
			{
				//draw empty
				draw_bag((*player).y/HEIGHT, ((*player).x - WIDTH*2)/WIDTH);
				gameMap.currentLevel[(*player).y/HEIGHT][((*player).x - WIDTH*2)/WIDTH] = 3;
				if(move_is_possible((*player).x - WIDTH*2,(*player).y, DOWN_ARROW, 0))//check if there is no dirt under
				{
					draw_empty((*player).y-1, (*player).x - WIDTH-2);
					gameMap.currentLevel[(*player).y/HEIGHT][((*player).x - WIDTH)/WIDTH] = 0;
					goldFalling((*player).x - WIDTH*2, (*player).y);
				}
			}
		}
		
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

void goldFalling(int x, int y)
{
	int k, l;
	
	draw_bag(y/HEIGHT, x/WIDTH);
	gameMap.currentLevel[(y)/HEIGHT][x/WIDTH] = 3;
	while(move_is_possible(x, y, DOWN_ARROW, 0))
	{	
		sleep(1);
		draw_empty(y-1, x-2);
		gameMap.currentLevel[(y)/HEIGHT][x/WIDTH] = 0;
		
		y = y + HEIGHT;
		draw_bag(y/HEIGHT, x/WIDTH);
		gameMap.currentLevel[(y)/HEIGHT][x/WIDTH] = 3;
	}
	
	
	//sprintf(debug_str,"falling finished");
	//send(debug,debug_str);
	
	//check if fall on someone
		//yes- kill it
	//drop money
		
	return;
}

void move_digger(Digger *player){	
	int scanned=0;
	while(1){
		scanned = receive();
		move(player,scanned);
		send(map_moves_pid,0);
	  }
}

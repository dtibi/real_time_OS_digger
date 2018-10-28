#include "digger.h"
#include "map.h"
#include "myints.h"

//Create digger
Digger create_digger()
{
	Digger player;
	player.x = 8;
	player.y = 7;
	player.direction = LEFT_ARROW;
	player.is_alive=1;
	player.lives=3;
	return player;
}

void move_digger(Digger *player,int direction)
{	
	int x=(*player).x,y=(*player).y,p_direction=(*player).direction;
	//sprintf(debug_str,"x - %d , y %d , direction %d - %d" ,x , y ,direction , get_object_in_direction(x, y, direction));
	//send(debug,debug_str);
	if(direction != p_direction) {//check if the wanted move direction is diffrent from the current
		(*player).direction = direction; //TODO: add here call the redraw digger
		draw_digger(*player);
	}
	if (!move_is_possible(x,y,direction, 1)) return;
	if (get_object_in_direction(x, y, direction) == 2) //diamond found
	{
		//add point to score;
	}
	else if (get_object_in_direction(x, y, direction) == 3) //gold sack found
	{
		if(direction==UP_ARROW || direction==DOWN_ARROW) 
			return;
		
		else if(direction==RIGHT_ARROW && move_is_possible(x + 1,y, RIGHT_ARROW, 1)) 
		{
			//draw empty
			draw_bag(y, x+2);
			draw_empty(y, x+1,1);
			if(move_is_possible(x + 2,y, DOWN_ARROW, 0))//check if there is no dirt under
			{
				draw_empty(y, x+1,1);
			}
		}
		
		else if(direction==LEFT_ARROW && move_is_possible(x - 1,y, LEFT_ARROW, 1)) 
		{
			//draw empty
			draw_bag(y, x-2);
			draw_empty(y, x-1,1);
			if(move_is_possible(x - 2,y, DOWN_ARROW, 0))//check if there is no dirt under
			{
				
				draw_empty(y, x-1,1);
			}
		} else {
			
		}
	}
	
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
	if(x!=(*player).x || y!=(*player).y) {
		draw_empty(y,x,1);
		//sprintf(debug_str,"diger moved from(%d,%d) to (%d,%d)",x,y,(*player).x,(*player).y);
		//send(debug,debug_str);
	}
	draw_digger(*player);
}

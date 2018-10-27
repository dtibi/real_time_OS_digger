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
	int x=(*player).x,y=(*player).y,p_direction=(*player).direction,coords[2];
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
				coords[0]=x+2;coords[1]=y;
				send(gold_falling_pid);
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
				coords[0]=x-2;coords[1]=y;
				send(gold_falling_pid);
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
	send(gold_falling_pid);
	if(x!=(*player).x || y!=(*player).y) draw_empty(y,x,1);
}



void run_digger(Digger *player){	
	int input=0;
	while(1){
		while(input!=TIME_TIK && (*player).is_alive){
			input = receive();
		}
		while (input!=RIGHT_ARROW && input!=LEFT_ARROW && input!=UP_ARROW && input!=DOWN_ARROW && (*player).is_alive){
			input = receive();
		}
		if(!(*player).is_alive) {digger_death_flow();}
		//sprintf(debug_str,"scanned value is - %d",input);
		//send(debug,debug_str);
		move_digger(player,input);
		draw_digger(*player);
	  }
}

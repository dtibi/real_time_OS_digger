#include "digger.h"
#include "map.h"
#include "myints.h"

//Create digger
Digger create_digger() {
	Digger player;
	
	player.x = 8;
	player.y = 7;
	player.direction = LEFT_ARROW;
	player.is_alive = 1;
	player.lives = 3;
	
	return player;
}

void move_digger(Digger *player, int direction) {	
	int x = (*player).x, y = (*player).y, p_direction = (*player).direction;
	if(direction != p_direction) { //check if the wanted move direction is diffrent from the current
		(*player).direction = direction;
		upd_draw_digger(*player);
	}
	
	if (!move_is_possible(x, y, direction, 1))
		return;

	if (get_object_in_direction(x, y, direction) == DIAMOND) { //diamond found
		//add point to score;
	}
	
	else if (get_object_in_direction(x, y, direction) == GOLD_BAG) //gold sack found
	{
		if(direction == UP_ARROW || direction == DOWN_ARROW) 
			return;
		
		else if(direction == RIGHT_ARROW && move_is_possible(x + 1, y, RIGHT_ARROW, 1)) {
			upd_draw_bag(y, x+2);
			upd_draw_empty(y, x+1,1);
			if(move_is_possible(x + 2,y, DOWN_ARROW, 0)) //check if there is no dirt under
				upd_draw_empty(y, x+1,1);
		}
		
		else if(direction == LEFT_ARROW && move_is_possible(x - 1, y, LEFT_ARROW, 1))  {
			//draw empty
			upd_draw_bag(y, x-2);
			upd_draw_empty(y, x-1,1);
			if(move_is_possible(x - 2,y, DOWN_ARROW, 0)) { //check if there is no dirt under
				upd_draw_empty(y, x-1, 1);
				send(gold_falling_pid);
			}
		} 
		else {
			
		}
	}
	
	switch (direction) {
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
	
	if(x != (*player).x || y != (*player).y) {
		upd_draw_empty(y,x,1);
		send(gold_falling_pid);
	}
	
	x = (*player).x;
	y = (*player).y;
	
	upd_draw_digger(*player);
}


void digger_death_flow(){
	//need to kill all enemys so they stop moving
	enemys[0].is_alive=0;
	
	player.lives--;
	if(player.lives==0) {
		restart_game();
		return;
	}
	send(sound_effects_pid,0);
	upd_draw_grave(player.y,player.x);
	upd_draw_empty(player.y,player.x,1);
	player.x=8;
	player.y=7;
	player.is_alive=1;
	
	//need to delete all enemys from map and respon them with time.
	upd_draw_empty(enemys[0].y,enemys[0].x,1);
	upd_draw_empty(enemys[1].y,enemys[1].x,1);
	upd_draw_empty(enemys[2].y,enemys[2].x,1);
	
}

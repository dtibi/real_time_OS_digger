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
	player.score = 0;
	
	return player;
}

void restart_digger(Digger* player) {
	(*player).x = 8;
	(*player).y = 7;
	(*player).direction = LEFT_ARROW;
	(*player).is_alive = 1;
}

void move_digger(Digger *player, int direction) {	
	int x = (*player).x, y = (*player).y, p_direction = (*player).direction, obj_in_direction;
	if(direction != p_direction) { //check if the wanted move direction is diffrent from the current
		(*player).direction = direction;
		upd_draw_digger(*player);
	}
	obj_in_direction = get_object_in_direction(x, y, direction);
	
	
	if (!move_is_possible(x, y, direction, 1))
		return;

	if (obj_in_direction == DIAMOND) { //diamond found
		send(score_lives_pid, DIAMOND_SCORE);
		sprintf(debug_str,"diamonds amount - %d", gameMap.diamond_amount - 1);
		send(debug,debug_str);
		gameMap.diamond_amount--;
		if(gameMap.diamond_amount == 0) next_level(); //all the diamonds were taken
	}
	else if (obj_in_direction == GOLD_BAG) //gold sack found
	{
		if(direction == UP_ARROW || direction == DOWN_ARROW) 
			return;
		
		else if(direction == RIGHT_ARROW && move_is_possible(x + 1, y, RIGHT_ARROW, 1)) {
			upd_draw_bag(y, x+2);
			upd_draw_empty(y, x+1,1);
			if(move_is_possible(x + 2,y, DOWN_ARROW, 0)) {//check if there is no dirt under
				upd_draw_empty(y, x+1,1);
				gameMap.level_map[y][x+2]=MOVING_GOLD_BAG;//this is done to prevent digger from pushing the bag any more
				resume(create(gold_falling, INITSTK, INITPRIO+1, "gold_falling", 2,y,x+2));
			}
		}
		
		else if(direction == LEFT_ARROW && move_is_possible(x - 1, y, LEFT_ARROW, 1))  {
			upd_draw_bag(y, x-2);
			upd_draw_empty(y, x-1,1);
			if(move_is_possible(x - 2,y, DOWN_ARROW, 0)) { //check if there is no dirt under
				upd_draw_empty(y, x-1, 1);
				gameMap.level_map[y][x-2]=MOVING_GOLD_BAG;//this is done to prevent digger from pushing the bag any more
				resume(create(gold_falling, INITSTK, INITPRIO+1, "gold_falling", 2,y,x-2));
			}
		} 
		else {
			
		}
	}
	else if (obj_in_direction >= GOLD+2 && obj_in_direction <= GOLD+5) {//gold nugets found
		send(score_lives_pid,(obj_in_direction-GOLD)*GOLD_NUGGER_SCORE);
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
		if(get_object_in_direction(x,y,UP_ARROW)==GOLD_BAG) {
			//upd_draw_dirt(y-1,x);
			resume(create(gold_falling, INITSTK, INITPRIO+1, "gold_falling", 2,y-1,x));
		}
	}
	
	x = (*player).x;
	y = (*player).y;
	
	//sprintf(debug_str,"x - %d , y %d , up:%d down:%d right:%d left:%d" ,x , y , get_object_in_direction(x, y, UP_ARROW), get_object_in_direction(x, y, DOWN_ARROW), get_object_in_direction(x, y, RIGHT_ARROW), get_object_in_direction(x, y, LEFT_ARROW));
	//send(debug,debug_str);
	
	upd_draw_digger(*player);
}


void digger_death_flow(){
	//need to kill all enemys so they stop moving
	enemys[0].is_alive=0;
	player.lives--;
	send(score_lives_pid,-1);
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

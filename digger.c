#include "digger.h"
#include "map.h"
#include "nobin.h"
#include "myints.h"
#include <stdio.h>

//Create digger
Digger create_digger() {
	Digger player;
	
	player.x = 8;
	player.y = 7;
	player.direction = LEFT_ARROW;
	player.is_alive = 1;
	player.lives = 3;
	player.score = 0;
	player.last_time_shot = 0;
	
	return player;
}

void restart_digger(Digger* player) {
	(*player).x = 8;
	(*player).y = 7;
	(*player).direction = LEFT_ARROW;
}

void move_digger(Digger *player, int direction) {	
	int x = (*player).x, y = (*player).y, p_direction = (*player).direction,obj_in_direction, gold_pid;
	if(direction != p_direction) { //check if the wanted move direction is diffrent from the current
		(*player).direction = direction;
		upd_draw_digger(*player);
	}
	obj_in_direction = get_object_in_direction(y, x, direction);
	
	
	if (!move_is_possible(y,x, direction, 1))
		return;

	if (obj_in_direction == DIAMOND) { //diamond found
		send(score_lives_pid, DIAMOND_SCORE);
		if(count_diamonds() - 1 == 0) next_level(); //all the diamonds were taken
	}
	else if (obj_in_direction == GOLD_BAG) //gold sack found
	{
		if(direction == UP_ARROW || direction == DOWN_ARROW) 
			return;
		
		else if(direction == RIGHT_ARROW && move_is_possible( y, x + 1 , RIGHT_ARROW, 1)) {
			if (get_object_in_direction(y, x + 1, RIGHT_ARROW) == GOLD_BAG) return;
			upd_draw_bag(y, x+2);
			upd_draw_empty(y, x+1,1);
			if(move_is_possible(y,x + 2, DOWN_ARROW, 0)) {//check if there is no dirt under
				upd_draw_empty(y, x+1,1);
				gameMap.level_map[y][x+2]=MOVING_GOLD_BAG;//this is done to prevent digger from pushing the bag any more
				resume(create(gold_falling, INITSTK, INITPRIO+1, "gold_falling", 2,y,x+2));
			}
		}
		
		else if(direction == LEFT_ARROW && move_is_possible( y , x - 1 , LEFT_ARROW, 1))  {
			if (get_object_in_direction(y, x - 1, LEFT_ARROW) == GOLD_BAG) return;
			upd_draw_bag(y, x-2);
			upd_draw_empty(y, x-1,1);
			if(move_is_possible(y,x - 2, DOWN_ARROW, 0)) { //check if there is no dirt under
				upd_draw_empty(y, x-1, 1);
				gameMap.level_map[y][x-2]=MOVING_GOLD_BAG;//this is done to prevent digger from pushing the bag any more
				resume(create(gold_falling, INITSTK, INITPRIO+1, "gold_falling", 2,y,x-2));
			}
		} 
		else {
			
		}
	}
	else if (obj_in_direction >= GOLD+2 && obj_in_direction <= GOLD+5) {//gold nugets found
		send(score_lives_pid,(obj_in_direction-GOLD)*GOLD_NUGGET_SCORE);
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
		if(get_object_in_direction(y,x,UP_ARROW)==GOLD_BAG) {
			//upd_draw_dirt(y-1,x);
			gold_pid = create(gold_falling, INITSTK, INITPRIO+1, "gold_falling", 2,y-1,x);
			resume(create(shake_bag,INITSTK,INITPRIO+1,"gold_shaking",3,y-1,x,gold_pid));
		}
	}
	
	x = (*player).x;
	y = (*player).y;
	
	upd_draw_digger(*player);
}


void digger_death_flow(){
	kill(nobbin_creator_pid);
	sleept(0);
	kill_all_enemys();
	player.lives--;
	send(score_lives_pid,-1);
	if(player.lives <=0) {
		restart_game();
		return;
	}
	send(sound_effects_pid,0);
	upd_draw_grave(player.y,player.x);
	sleept(15);
	upd_draw_empty(player.y,player.x,1);
	player.x=8;
	player.y=7;
	player.is_alive=1;
	create_enemys();
	resume(nobbin_creator_pid = create(nobbin_creator,INITSTK,INITPRIO,"nobbin_creator",0));
}

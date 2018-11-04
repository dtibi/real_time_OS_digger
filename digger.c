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

void restart_digger() {
	player.x = 8;
	player.y = 7;
	player.direction = LEFT_ARROW;
	upd_draw_digger(player);
}

void move_digger( int direction) {	
	int x = player.x, y = player.y, p_direction = player.direction,obj_in_direction, gold_pid,ps;
	int deltaX=0, deltaY=0, i;
	sleept((int)((SECONDT/FACTOR)+(SECONDT/FACTOR)*gameMap.digger_speed));
	if(direction != p_direction) { //check if the wanted move direction is diffrent from the current
		player.direction = direction;
		upd_draw_digger(player);
	}
	obj_in_direction = get_object_in_direction(y, x, direction);
	
	
	if (!move_is_possible(y,x, direction, 1))
		return;

	if (obj_in_direction == DIAMOND) { //diamond found
		send(score_lives_pid, DIAMOND_SCORE);
		if(count_diamonds() - 1 == 0) next_level(); //all the diamonds were taken
	}
	
	if (obj_in_direction == CHERRY) { //cherry found
		send(score_lives_pid, CHERRY_SCORE);
		crazy_mode = 1;
	}
	
	if((obj_in_direction == HOBBIN || obj_in_direction == NOBBIN) && crazy_mode) {
		
		if(direction == LEFT_ARROW)
			deltaX = -1;
		else if(direction == UP_ARROW)
			deltaY = -1;
		else if(direction == RIGHT_ARROW)
			deltaX = 1;
		else
			deltaY = 1;
		disable(ps);
		for(i = 0; i < ENEMY_COUNT; i++){
			if((enemys[i].y == player.y + deltaY) && (enemys[i].x == player.x + deltaX)) {
					upd_draw_empty(enemys[i].y, enemys[i].x, 1);
					send(score_lives_pid, DEAD_ENEMY_SCORE);
					kill_enemy(i);
					if(number_of_live_enemys() == 0 && all_enemys_created) next_level();
			}
		}
		restore(ps);
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
				resume(gold_pid=create(gold_falling, INITSTK, INITPRIO+1, "g_fall_r", 2,y,x+2));
				if (gold_pid==SYSERR) printf("ERROR! while creating g_fall_l");
			}
		}
		
		else if(direction == LEFT_ARROW && move_is_possible( y , x - 1 , LEFT_ARROW, 1))  {
			if (get_object_in_direction(y, x - 1, LEFT_ARROW) == GOLD_BAG) return;
			upd_draw_bag(y, x-2);
			upd_draw_empty(y, x-1,1);
			if(move_is_possible(y,x - 2, DOWN_ARROW, 0)) { //check if there is no dirt under
				upd_draw_empty(y, x-1, 1);
				gameMap.level_map[y][x-2]=MOVING_GOLD_BAG;//this is done to prevent digger from pushing the bag any more
				resume(gold_pid = create(gold_falling, INITSTK, INITPRIO+1, "g_fall_l", 2,y,x-2));
				if (gold_pid==SYSERR) printf("ERROR! while creating g_fall_l");
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
				player.x --;
			break;
		case RIGHT_ARROW:
				player.x ++;
			break;
		case DOWN_ARROW:
				player.y ++;
			break;
		case UP_ARROW:
				player.y --;
			break;
	}
	
	if(x != player.x || y != player.y) {
		upd_draw_empty(y,x,1);
		if(get_object_in_direction(y,x,UP_ARROW)==GOLD_BAG) {
			upd_draw_dirt(y-1,x);
			gold_pid = create(gold_falling, INITSTK, INITPRIO+1, "g_fall_d", 2,y-1,x);
			if (gold_pid==SYSERR) printf("ERROR! while creating g_fall_d");
			resume(gold_pid = create(shake_bag,INITSTK,INITPRIO+1,"gold_shaking",3,y-1,x,gold_pid));
			if (gold_pid==SYSERR) printf("ERROR! while creating shake_bag");
		}
	}
	
	x = player.x;
	y = player.y;
	
	upd_draw_digger(player);
}

void digger_death_flow(){
	int ps;
	disable(ps);
	kill_all_enemys();
	//reseting enemys counter
	gameMap.monster_max_amount = monster_max_count[gameMap.level_id];;
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
	restore(ps);
}

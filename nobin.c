#include "digger.h"
#include "nobin.h"
#include "map.h"
#include "myints.h"
int all_enemys_created = 0;


Enemy create_enemy(int i) { 
	int ps;
	Enemy enemy;
	char* pname;
	disable(ps);
	enemy.x = 14;
	enemy.y = 0;
	enemy.direction = LEFT_ARROW;
	enemy.is_hobin = 0;
	enemy.last_time_hobin=time_from_start;
	sprintf(pname,"nobbin_%d",i);
	resume(enemys_pid[i] = create(move_nobbin,INITSTK,INITPRIO,pname,1,i));
	enemys_proccess_is_alive[i]=1;
	restore(ps);
	return enemy;
}
				
void move_nobbin(int i){
	int  direction, obj_in_direction,ps;
	while(1) {
		sleept((int)((SECONDT/FACTOR)+(SECONDT/FACTOR)*gameMap.monster_speed));
		disp_draw_pixel_with_char(0,50+i,RED_BG, ' ');
		if (!enemys[i].is_hobin) {
			if((time_from_start-enemys[i].last_time_hobin)/SECONDT >= gameMap.monster_become_angry_time) {
				enemys[i].is_hobin=1;
				enemys[i].last_time_hobin=time_from_start;
			}

		} else {
			if((time_from_start-enemys[i].last_time_hobin)/SECONDT >= gameMap.monster_angry_for_time) {
				enemys[i].is_hobin=0;
				enemys[i].last_time_hobin=time_from_start;
			}
		}
		direction = find_direction_to_digger(enemys[i]);
		
		if(direction!=0) enemys[i].direction=direction;
		if(!move_is_possible(enemys[i].y,enemys[i].x,direction,enemys[i].is_hobin)) continue;
		obj_in_direction = get_object_in_direction(enemys[i].x, enemys[i].y, direction);
		disable(ps);
		if (obj_in_direction == DIAMOND && count_diamonds() - 1 == 0) next_level();  //all the diamonds were taken
		restore(ps);
		upd_draw_empty(enemys[i].y, enemys[i].x, 1);
		if (gameMap.level_map[enemys[i].y][enemys[i].x]==DIGGER && !crazy_mode) {
			disable(ps);
			player.is_alive=0;
			enemys_proccess_is_alive[i]=0;
			restore(ps);
			return;
		}
		if (get_object_in_direction(enemys[i].y, enemys[i].x,direction)==DIGGER && !crazy_mode)	{
			disable(ps);
			player.is_alive=0;
			enemys_proccess_is_alive[i]=0;
			restore(ps);
			return;
		}
		
		upd_draw_empty(enemys[i].y, enemys[i].x, 1);
		switch (direction) {
			case LEFT_ARROW:
				enemys[i].x--;
				break;
			case RIGHT_ARROW:
				enemys[i].x++;
				break;
			case DOWN_ARROW:
				enemys[i].y++;
				break;
			case UP_ARROW:
				enemys[i].y--;
				break;
		}
		
		if(enemys[i].is_hobin) upd_draw_hobbin(enemys[i].y,enemys[i].x,enemys[i].direction);
		else upd_draw_nobbin(enemys[i].y,enemys[i].x);
		disp_draw_pixel_with_char(0,50+i,BLACK_BG, ' ');
	}
}

int find_direction_to_digger(Enemy enemy) {
	int can_right, can_left, can_up, can_down, path_amount;
	int len_right = MAX_PATH_LEN, len_left = MAX_PATH_LEN, len_up = MAX_PATH_LEN, len_down = MAX_PATH_LEN, min_path_index;
	int next_move;
	
	can_right = move_is_possible(enemy.y, enemy.x, RIGHT_ARROW, enemy.is_hobin);
	can_left = move_is_possible(enemy.y, enemy.x, LEFT_ARROW, enemy.is_hobin);
	can_up = move_is_possible(enemy.y, enemy.x, UP_ARROW, enemy.is_hobin);
	can_down = move_is_possible(enemy.y, enemy.x, DOWN_ARROW, enemy.is_hobin);
	path_amount = can_right + can_left + can_up + can_down;
	
	if(is_digger_next_to_me(enemy.y, enemy.x) && !crazy_mode) {
		player.is_alive = 0;
		return is_digger_next_to_me(enemy.y, enemy.x);
	}
	
	if(path_amount == 1) {
		if (can_right) return RIGHT_ARROW;
		if (can_left) return LEFT_ARROW;
		if (can_up) return UP_ARROW;
		if (can_down) return DOWN_ARROW;
	}
	
	else if (path_amount == 2 ) {
		if (can_up && enemy.direction != DOWN_ARROW) return UP_ARROW;
		if (can_down && enemy.direction != UP_ARROW) return DOWN_ARROW;
		if (can_right && enemy.direction != LEFT_ARROW) return RIGHT_ARROW;
		if (can_left && enemy.direction != RIGHT_ARROW) return LEFT_ARROW;
	}
	
	else if(path_amount == 3 || path_amount == 4) {
		
		if (can_up && enemy.direction != DOWN_ARROW) len_up = find_path_to_digger_len(enemy.x, enemy.y - 1, UP_ARROW);
		if (can_down && enemy.direction != UP_ARROW) len_down = find_path_to_digger_len(enemy.x, enemy.y + 1, DOWN_ARROW);
		if (can_right && enemy.direction != LEFT_ARROW) len_right = find_path_to_digger_len(enemy.x + 1, enemy.y, RIGHT_ARROW);
		if (can_left && enemy.direction != RIGHT_ARROW) len_left = find_path_to_digger_len(enemy.x - 1, enemy.y, LEFT_ARROW);
		
		if(crazy_mode) next_move =  max_index(len_up, len_down, len_right, len_left);
		else  next_move = min_index(len_up, len_down, len_right, len_left);
		
		if(next_move == 1) return UP_ARROW;
		if(next_move == 2) return DOWN_ARROW;
		if(next_move == 3) return RIGHT_ARROW;
		if(next_move == 4) return LEFT_ARROW;
	}
	
	return 0;
}

int find_path_to_digger_len(int xE, int yE, int direction) {
	int can_right, can_left, can_up, can_down, path_amount;
	int path_len = 0;
	int rand,max_iterations=150;
	
	while (!is_digger_next_to_me(yE, xE)) {
		if (max_iterations-- < 0) return 999;
		can_right = move_is_possible(yE,xE,  RIGHT_ARROW, 0);
		can_left = move_is_possible(yE,xE,  LEFT_ARROW, 0);
		can_up = move_is_possible(yE,xE,  UP_ARROW, 0);
		can_down = move_is_possible(yE,xE,  DOWN_ARROW, 0);
		path_amount = can_right + can_left + can_up + can_down;
		
		if (path_amount == 0) return 999;
		
		if (path_amount == 1 || path_amount == 2 ) {
			if (can_up && direction != DOWN_ARROW) {
				path_len++;
				yE--;
				direction = UP_ARROW;
			}
			
			else if (can_down && direction != UP_ARROW) {
				path_len++;
				yE++;
				direction = DOWN_ARROW;
			}
			
			else if (can_right && direction != LEFT_ARROW) {
				path_len++;
				xE++;
				direction = RIGHT_ARROW;
			}
			
			else if (can_left && direction != RIGHT_ARROW) {
				path_len++;
				xE--;
				direction = LEFT_ARROW;
			}
			
			else return 999;
		}
		
		else if (path_amount == 3 || path_amount == 4) {
			rand = time_from_start%4;
			
			try_up:
			if (rand == 0) {
				if (can_up && direction != DOWN_ARROW) {
					yE--;
					direction = UP_ARROW;
				} else rand++;
			}
			
			if(rand == 1) {
				if (can_down && direction != UP_ARROW) {
					yE++;
					direction = DOWN_ARROW;
				} else rand++;
			}
			
			if(rand == 2) {
				if (rand == 2 && can_right && direction != LEFT_ARROW) {
					xE++;
					direction = RIGHT_ARROW;
				}else rand ++;
			}
			
			if(rand == 3) {
				if (rand == 3 && can_left && direction != RIGHT_ARROW) {
					xE--;
					direction = LEFT_ARROW;
				}
				else {
					rand = 0;
					goto try_up;
				}
			}
			path_len++;
			
		}
	}
	
	if (is_digger_next_to_me(yE, xE)) return path_len + 1;
	return 999;
}

int min_index(int v1, int v2, int v3, int v4) {
	if(v1 <= v2 && v1 <= v3 && v1 <= v4) return 1;
	else if(v2 <= v1 && v2 <= v3 && v2 <= v4) return 2;
	else if(v3 <= v1 && v3 <= v2 && v3 <= v4) return 3;
	else if(v4 <= v1 && v4 <= v2 && v4 <= v3) return 4;
	
	return 0;
}

int max_index(int v1, int v2, int v3, int v4) {
	if(v1 >= v2 && v1 >= v3 && v1 >= v4) return 1;
	else if(v2 >= v1 && v2 >= v3 && v2 >= v4) return 2;
	else if(v3 >= v1 && v3 >= v2 && v3 >= v4) return 3;
	else if(v4 >= v1 && v4 >= v2 && v4 >= v3) return 4;
	
	return 0;
}

void nobbin_creator(){
	int i,lowest_dead_nobbin=-1,n,ps;

	all_enemys_created = 0;
	while(gameMap.monster_max_amount>0) {
		disp_draw_pixel_with_char(0, 69, BABY_BG, ' ');
		n = number_of_live_enemys();
		if (n<gameMap.monster_start_amount){
			lowest_dead_nobbin = get_lowest_dead_nobbin();
			if(lowest_dead_nobbin>=0) {
				disable(ps);
				enemys[lowest_dead_nobbin] = create_enemy(lowest_dead_nobbin);
				gameMap.monster_max_amount--;
				restore(ps);
			}
		}
		disp_draw_pixel_with_char(0, 69, BLACK_BG, ' ');
		sleept(SECONDT*5);
	}
	all_enemys_created = 1;
}

int get_lowest_dead_nobbin() {
	int i,ps;
	disable(ps);
	for(i=0;i<ENEMY_COUNT;i++)
		if (!enemys_proccess_is_alive[i]) return i;
	restore(ps);
	return -1;
}

void kill_enemy(int i){
	kill(enemys_pid[i]);
	wakeup(enemys_pid[i]);
	enemys_proccess_is_alive[i]=0;
}

void kill_all_enemys() {
	int i,ps;
	disable(ps);
	for(i = 0; i < ENEMY_COUNT; i++){
		if(enemys_proccess_is_alive[i]==1) {
			upd_draw_empty(enemys[i].y,enemys[i].x,1);
			kill_enemy(i);	
		}
	}
	restore(ps);
}

int number_of_live_enemys() {
	int i, ret = 0,ps;
	disable(ps);
	for(i = 0; i < ENEMY_COUNT; i++) {
		if (enemys_proccess_is_alive[i] == 1) ret++;
	}
	restore(ps);
	return ret;
}
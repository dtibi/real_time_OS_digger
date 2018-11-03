#include "digger.h"
#include "nobin.h"
#include "map.h"
#include "myints.h"

Enemy create_enemy(Digger *d) { 
	Enemy enemy;
	
	enemy.x = 14;
	enemy.y = 0;
	enemy.direction = LEFT_ARROW;
	enemy.digger = d;
	enemy.is_alive = 0;
	enemy.is_hobin = 0;
	
	return enemy;
}

Enemy create_enemy_by_val(int y, int x, Digger *d, int direction)
{
	Enemy enemy;
	
	enemy.x = x;
	enemy.y = y;
	enemy.direction = direction;
	enemy.digger = d;
	enemy.is_alive = 0;
	enemy.is_hobin = 0;
	
	return enemy;
}

Enemy copy_enemy(Enemy to_copy) {
	Enemy enemy;
	
	enemy.x = to_copy.x;
	enemy.y = to_copy.y;
	enemy.direction = to_copy.direction;
	enemy.digger = to_copy.digger;
	enemy.is_alive = to_copy.is_alive;
	enemy.is_hobin = to_copy.is_hobin;
	
	return enemy;
}
				
void move_nobbins(){
	int i, direction, obj_in_direction;
	for(i=0; i<NOBBIN_COUNT; i++) {
		if(enemys[i].is_alive == 1) {
			direction = find_direction_to_digger(enemys[i]);
			obj_in_direction = get_object_in_direction(enemys[i].x, enemys[i].y, direction);
			
			sprintf(debug_str, "diamonds = %d", count_diamonds());
			send(debug,debug_str);
			
			if (obj_in_direction == DIAMOND && count_diamonds() - 1 == 0) next_level();  //all the diamonds were taken
			
			if(direction!=0)
				enemys[i].direction=direction;
			
			if (gameMap.level_map[enemys[i].y][enemys[i].x]==DIGGER) player.is_alive=0;
			if (get_object_in_direction(enemys[i].y, enemys[i].x,direction)==DIGGER) {
				player.is_alive=0;
				break;
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
			
			if(direction!=0) enemys[i].direction=direction;
			
			if(enemys[i].is_hobin)
				upd_draw_hobbin(enemys[i].y,enemys[i].x,enemys[i].direction);
			else
				upd_draw_nobbin(enemys[i].y,enemys[i].x);
		}
	}
}

int find_direction_to_digger(Enemy enemy) {
	int can_right, can_left, can_up, can_down, path_amount;
	int len_right = MAX_PATH_LEN, len_left = MAX_PATH_LEN, len_up = MAX_PATH_LEN, len_down = MAX_PATH_LEN, min_path_index;
	
	can_right = move_is_possible(enemy.y, enemy.x, RIGHT_ARROW, enemy.is_hobin);
	can_left = move_is_possible(enemy.y, enemy.x, LEFT_ARROW, enemy.is_hobin);
	can_up = move_is_possible(enemy.y, enemy.x, UP_ARROW, enemy.is_hobin);
	can_down = move_is_possible(enemy.y, enemy.x, DOWN_ARROW, enemy.is_hobin);
	path_amount = can_right + can_left + can_up + can_down;
	
	if(is_digger_next_to_me(enemy.y, enemy.x)) return is_digger_next_to_me(enemy.y, enemy.x);
	
	else if(path_amount == 1) {
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
		
		if (can_up && enemy.direction != DOWN_ARROW) len_up = find_path_to_digger_len_iterative(enemy.x, enemy.y - 1, UP_ARROW);
		if (can_down && enemy.direction != UP_ARROW) len_down = find_path_to_digger_len_iterative(enemy.x, enemy.y + 1, DOWN_ARROW);
		if (can_right && enemy.direction != LEFT_ARROW) len_right = find_path_to_digger_len_iterative(enemy.x + 1, enemy.y, RIGHT_ARROW);
		if (can_left && enemy.direction != RIGHT_ARROW) len_left = find_path_to_digger_len_iterative(enemy.x - 1, enemy.y, LEFT_ARROW);
		
		min_path_index = min_index(len_up, len_down, len_right, len_left);
		if(min_path_index == 1) return UP_ARROW;
		if(min_path_index == 2) return DOWN_ARROW;
		if(min_path_index == 3) return RIGHT_ARROW;
		if(min_path_index == 4) return LEFT_ARROW;
	}
	
	return 0;
}

int find_path_to_digger_len_iterative(int xE, int yE, int direction) {
	int can_right, can_left, can_up, can_down, path_amount;
	int path_len = 0;
	int rand;
	
	while (!is_digger_next_to_me(yE, xE)) {
		
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
			rand = tod%4;
			
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

void create_enemys() {
	int i;
	for(i = 0; i < ENEMY_COUNT; i++)
		enemys[i] = create_enemy((Digger*)&player);
	enemys[0].is_alive=1;
	upd_draw_empty(enemys[0].y,enemys[0].x,1);
}

void kill_all_enemys() {
	int i;
	for(i = 0; i < ENEMY_COUNT; i++) {
		enemys[i].is_alive=0;
		upd_draw_empty(enemys[i].y, enemys[i].x, 1);
	}
	
}
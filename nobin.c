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

Enemy create_enemy_by_val(int x, int y, Digger *d, int direction)
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
	int i;
	char direction;
	for(i=0; i<NOBBIN_COUNT; i++) {
		if(enemys[i].is_alive == 1) {
			direction = find_direction_to_digger(enemys[i]);
			
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
			
			if(direction!=0)
				enemys[i].direction=direction;
			
			upd_draw_nobbin(enemys[i].y,enemys[i].x);
			if (gameMap.level_map[enemys[i].y][enemys[i].x]==DIGGER) player.is_alive=0;
		}
	}
}

int find_direction_to_digger(Enemy enemy) {
	int can_right, can_left, can_up, can_down;
	int path_amount;
	int len_right = 999, len_left = 999, len_up = 999, len_down = 999, min_path_index = 0;
	
	can_right = move_is_possible(enemy.x, enemy.y, RIGHT_ARROW, enemy.is_hobin);
	can_left = move_is_possible(enemy.x, enemy.y, LEFT_ARROW, enemy.is_hobin);
	can_up = move_is_possible(enemy.x, enemy.y, UP_ARROW, enemy.is_hobin);
	can_down = move_is_possible(enemy.x, enemy.y, DOWN_ARROW, enemy.is_hobin);
	path_amount = can_right + can_left + can_up + can_down;
	
	if(get_object_in_direction(enemy.x, enemy.y, RIGHT_ARROW) == DIGGER) return RIGHT_ARROW;
	if(get_object_in_direction(enemy.x, enemy.y, LEFT_ARROW) == DIGGER) return LEFT_ARROW;
	if(get_object_in_direction(enemy.x, enemy.y, DOWN_ARROW) == DIGGER) return DOWN_ARROW;
	if(get_object_in_direction(enemy.x, enemy.y, UP_ARROW) == DIGGER) return UP_ARROW;
	
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
		
		if (can_up && enemy.direction != DOWN_ARROW) len_up = find_path_to_digger_len_iterative(enemy.x, enemy.y - 1, UP_ARROW);//find_path_to_digger_len(enemy.x, enemy.y - 1, UP_ARROW, enemy.x, enemy.y - 1, 0, 0, 0, 0);
		if (can_down && enemy.direction != UP_ARROW) len_down = find_path_to_digger_len_iterative(enemy.x, enemy.y + 1, DOWN_ARROW);//find_path_to_digger_len(enemy.x, enemy.y + 1, DOWN_ARROW, enemy.x, enemy.y + 1, 0, 0, 0, 0);
		if (can_right && enemy.direction != LEFT_ARROW) len_right = find_path_to_digger_len_iterative(enemy.x + 1, enemy.y, RIGHT_ARROW);//find_path_to_digger_len(enemy.x + 1, enemy.y, RIGHT_ARROW, enemy.x + 1, enemy.y, 0, 0, 0, 0);
		if (can_left && enemy.direction != RIGHT_ARROW) len_left = find_path_to_digger_len_iterative(enemy.x - 1, enemy.y, LEFT_ARROW);//find_path_to_digger_len(enemy.x - 1, enemy.y, LEFT_ARROW, enemy.x - 1, enemy.y, 0, 0, 0, 0);
		
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
	int rand, selcted_random = 0;
	
	while (!is_digger_next_to_me(xE, yE)) {
		
		can_right = move_is_possible(xE, yE, RIGHT_ARROW, 0);
		can_left = move_is_possible(xE, yE, LEFT_ARROW, 0);
		can_up = move_is_possible(xE, yE, UP_ARROW, 0);
		can_down = move_is_possible(xE, yE, DOWN_ARROW, 0);
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
			while (selcted_random == 0) {
				rand = tod%4;
				
				if (rand ==0 && can_up && direction != DOWN_ARROW) {
					path_len++;
					yE--;
					direction = UP_ARROW;
					selcted_random =1;
				}
				if (rand == 1 && can_down && direction != UP_ARROW) {
					path_len++;
					yE++;
					direction = DOWN_ARROW;
					selcted_random =1;
				}
				if (rand == 2 && can_right && direction != LEFT_ARROW) {
					path_len++;
					xE++;
					direction = RIGHT_ARROW;
					selcted_random =1;
				}
				
				if (rand == 3 && can_left && direction != RIGHT_ARROW) {
					path_len++;
					xE--;
					direction = LEFT_ARROW;
					selcted_random =1;
				}
			}
			selcted_random =0;
		}
	}
	
	if (is_digger_next_to_me(xE, yE)) return path_len + 1;
	return 999;
}

/*
int find_path_to_digger_len(int xE, int yE, int direction, int start_x, int start_y, int tryed_up, int tryed_down, int tryed_right, int tryed_left) {
	int can_right, can_left, can_up, can_down, path_amount;
	int len_right = 9999, len_left = 9999, len_up = 9999, len_down = 9999;
	int min_path_index = 0;
	
	if(get_object_in_direction(xE, yE, RIGHT_ARROW) == DIGGER) return 1;
	if(get_object_in_direction(xE, yE, LEFT_ARROW) == DIGGER) return 1;
	if(get_object_in_direction(xE, yE, DOWN_ARROW) == DIGGER) return 1;
	if(get_object_in_direction(xE, yE, UP_ARROW) == DIGGER) return 1;
	
	can_right = move_is_possible(xE, yE, RIGHT_ARROW, 0);
	can_left = move_is_possible(xE, yE, LEFT_ARROW, 0);
	can_up = move_is_possible(xE, yE, UP_ARROW, 0);
	can_down = move_is_possible(xE, yE, DOWN_ARROW, 0);
	path_amount = can_right + can_left + can_up + can_down;
	
	if (path_amount == 1 || path_amount == 2 ) {
		if (can_up && direction != DOWN_ARROW) return 1+ find_path_to_digger_len(xE, yE - 1, UP_ARROW, start_x, start_y, tryed_up, tryed_down, tryed_right, tryed_left);
		if (can_down && direction != UP_ARROW) return 1+ find_path_to_digger_len(xE, yE + 1, DOWN_ARROW, start_x, start_y, tryed_up, tryed_down, tryed_right, tryed_left);
		if (can_right && direction != LEFT_ARROW) return 1+ find_path_to_digger_len(xE + 1, yE, RIGHT_ARROW, start_x, start_y, tryed_up, tryed_down, tryed_right, tryed_left);
		if (can_left && direction != RIGHT_ARROW) return 1+ find_path_to_digger_len(xE - 1, yE, LEFT_ARROW, start_x, start_y, tryed_up, tryed_down, tryed_right, tryed_left);
		return 999;
	}
	
	else if (path_amount == 3  || path_amount == 4) {
		if (!tryed_up && can_up && direction != DOWN_ARROW) len_up = 1 + find_path_to_digger_len(xE, yE - 1, UP_ARROW, xD, yD, xE, yE, 1, tryed_down, tryed_right, tryed_left);
		if (!tryed_down && can_down && direction != UP_ARROW) len_down = 1 + find_path_to_digger_len(xE, yE + 1, DOWN_ARROW, xD, yD, xE, yE, tryed_up, 1, tryed_right, tryed_left);
		if (!tryed_right && can_right && direction != LEFT_ARROW) len_right = 1 + find_path_to_digger_len(xE + 1, yE, RIGHT_ARROW, xD, yD, xE, yE, tryed_up, tryed_down, 1, tryed_left);
		if (!tryed_left &&  can_left && direction != RIGHT_ARROW) len_left = 1 + find_path_to_digger_len(xE - 1, yE, LEFT_ARROW, xD, yD, xE, yE, tryed_up, tryed_down, tryed_right, 1);
		
		min_path_index = min_index(len_up, len_down, len_right, len_left);
		if(min_path_index == 1) return 1 + find_path_to_digger_len(xE, yE - 1, UP_ARROW, xD, yD, xE, yE, 0, 0, 0, 0);
		if(min_path_index == 2) return 1 + find_path_to_digger_len(xE, yE + 1, DOWN_ARROW, xD, yD, xE, yE, 0, 0, 0, 0);
		if(min_path_index == 3) return 1 + find_path_to_digger_len(xE + 1, yE, RIGHT_ARROW, xD, yD, xE, yE, 0, 0, 0, 0);
		if(min_path_index == 4) return 1 + find_path_to_digger_len(xE - 1, LEFT_ARROW, yE, xD, yD, xE, yE, 0, 0, 0, 0);
	}
	return 8888;
}
*/

int min_index(int v1, int v2, int v3, int v4) {
	if(v1 <= v2 && v1 <= v3 && v1 <= v4) return 1;
	else if(v2 <= v1 && v2 <= v3 && v2 <= v4) return 2;
	else if(v3 <= v1 && v3 <= v2 && v3 <= v4) return 3;
	else if(v4 <= v1 && v4 <= v2 && v4 <= v3) return 4;
	
	return 0;
}
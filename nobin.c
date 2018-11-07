#include "digger.h"
#include "nobin.h"
#include "map.h"
#include "myints.h"

int all_enemys_created = 0;

//Create and return enemy with initial values
Enemy create_enemy() { 
	Enemy enemy;
	enemy.x = 14;
	enemy.y = 0;
	enemy.direction = LEFT_ARROW;
	enemy.is_hobin = 0;
	enemy.last_time_hobin = tod;
	enemy.is_alive = 0;
	return enemy;
}

//move spasific nobin (nobin in index i)
void move_nobbin(int i) {
	int  direction, obj_in_direction, ps;
	
	disp_draw_pixel_with_char(0,50+i,RED_BG, ' ');
	if (!enemys[i].is_hobin) { //if the enemy is nobin
		if((tod - enemys[i].last_time_hobin) / SECONDT >= gameMap.monster_become_angry_time) { //check if the nobin need to become hobin
			enemys[i].is_hobin = 1;
			enemys[i].last_time_hobin = tod;
		}

	} 
	else { //if the enemy is hobin
		if((tod - enemys[i].last_time_hobin)/SECONDT >= gameMap.monster_angry_for_time) { //check if the hobin need to become nobin
			enemys[i].is_hobin=0;
			enemys[i].last_time_hobin = tod;
		}
	}
	
	direction = find_direction_to_digger(enemys[i]); 

	if(!move_is_possible(enemys[i].y, enemys[i].x, direction, enemys[i].is_hobin)) { //check if direction is possible, if not- dont make any move
		disp_draw_pixel_with_char(0, 50 + i, BLACK_BG, ' ');
		return;
	}
	enemys[i].direction = direction;
	
	obj_in_direction = get_object_in_direction(enemys[i].x, enemys[i].y, direction);
	if (obj_in_direction == DIAMOND && count_diamonds() - 1 == 0) disp_next_level();  //all the diamonds were taken
	else if (get_object_in_direction(enemys[i].y, enemys[i].x, direction) == DIGGER && !crazy_mode) player.is_alive = 0; //the digger is located in the direction the enemy going
	else if (gameMap.level_map[enemys[i].y][enemys[i].x] == DIGGER && !crazy_mode) player.is_alive = 0; //the digger already made move and now he is located in the same spot as the enemy
	
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
	
	if(enemys[i].is_hobin) upd_draw_hobbin(enemys[i].y, enemys[i].x, enemys[i].direction);
	else upd_draw_nobbin(enemys[i].y,enemys[i].x);
	disp_draw_pixel_with_char(0, 50 + i, BLACK_BG, ' ');
}

//find direction to digger from spasific enemy
int find_direction_to_digger(Enemy enemy) {
	int can_right, can_left, can_up, can_down, path_amount;
	int len_right = MAX_PATH_LEN, len_left = MAX_PATH_LEN, len_up = MAX_PATH_LEN, len_down = MAX_PATH_LEN;
	int next_move = 0;
	int diff_x, diff_y, abs_diff_x, abs_diff_y;
	
	//check what are to posilbe paths
	can_right = move_is_possible(enemy.y, enemy.x, RIGHT_ARROW, enemy.is_hobin);
	can_left = move_is_possible(enemy.y, enemy.x, LEFT_ARROW, enemy.is_hobin);
	can_up = move_is_possible(enemy.y, enemy.x, UP_ARROW, enemy.is_hobin);
	can_down = move_is_possible(enemy.y, enemy.x, DOWN_ARROW, enemy.is_hobin);
	path_amount = can_right + can_left + can_up + can_down;
	
	if(is_digger_next_to_me(enemy.y, enemy.x) && !crazy_mode) {
		player.is_alive = 0;
		return is_digger_next_to_me(enemy.y, enemy.x);
	}
	
	if (enemy.is_hobin) {
		//calc the diffrent beatween player location and enemy location
		diff_x = enemy.x - player.x;
		diff_y = enemy.y - player.y;
		
		//check what is bigger, the diffrent beatween the x cordination or the y
		abs_diff_x = diff_x;
		abs_diff_y = diff_y;
		if(diff_x < 0) abs_diff_x = - diff_x;
		if(diff_y < 0) abs_diff_y = - diff_y;
		
		//if the diffrent beatween the x cordination is bigger the hobin need to move right or left
		if(abs_diff_x > abs_diff_y) {
			if (diff_x > 0 && can_left) next_move = LEFT_ARROW;
			else if(can_right) next_move = RIGHT_ARROW;
		} 
		//if the diffrent beatween the y cordination is bigger the hobin need to move down or up
		else { // abs_diff_y >= abs_diff_x
			if (diff_y > 0 && can_up) next_move = UP_ARROW;
			else if(can_down) next_move = DOWN_ARROW;
		}
		
		if(!crazy_mode) return next_move;
		//if the digger ate chrrey, go the opsite way
		else {
			switch(next_move) {
				case LEFT_ARROW:
					return RIGHT_ARROW;
				case RIGHT_ARROW:
					return LEFT_ARROW;
				case UP_ARROW:
					return DOWN_ARROW;
				case DOWN_ARROW:
					return UP_ARROW;
				default:
					return 0;
			}
		}
	}
	
	else { //the enemy is nobin
		if(path_amount == 1) { //if the nobin can go only one way, go that way
			if (can_right) return RIGHT_ARROW;
			if (can_left) return LEFT_ARROW;
			if (can_up) return UP_ARROW;
			if (can_down) return DOWN_ARROW;
		}
		
		else if (path_amount == 2 ) { //if the nobin can have two posible paths, he will keep going the direcion he was moving (the nobin will not go back) 
			if (can_up && enemy.direction != DOWN_ARROW) return UP_ARROW;
			if (can_down && enemy.direction != UP_ARROW) return DOWN_ARROW;
			if (can_right && enemy.direction != LEFT_ARROW) return RIGHT_ARROW;
			if (can_left && enemy.direction != RIGHT_ARROW) return LEFT_ARROW;
		}
		
		else if(path_amount == 3 || path_amount == 4) { //if the nobin got into crossroads, calc the len of any path (but not the path that he come from - the nobin will not go back)
			if (can_up && enemy.direction != DOWN_ARROW) len_up = find_path_to_digger_len(enemy.x, enemy.y - 1, UP_ARROW);
			if (can_down && enemy.direction != UP_ARROW) len_down = find_path_to_digger_len(enemy.x, enemy.y + 1, DOWN_ARROW);
			if (can_right && enemy.direction != LEFT_ARROW) len_right = find_path_to_digger_len(enemy.x + 1, enemy.y, RIGHT_ARROW);
			if (can_left && enemy.direction != RIGHT_ARROW) len_left = find_path_to_digger_len(enemy.x - 1, enemy.y, LEFT_ARROW);
			
			if(!crazy_mode)  next_move = min_index(len_up, len_down, len_right, len_left); //in normal mode mark next move as the shortest path
			else next_move =  max_index(len_up, len_down, len_right, len_left); //in crazy mode mark next move as the longest path
			
			if(next_move == 1) return UP_ARROW;
			if(next_move == 2) return DOWN_ARROW;
			if(next_move == 3) return RIGHT_ARROW;
			if(next_move == 4) return LEFT_ARROW;
		}
	}
	return 0;
}

//return the len of the path to the digger from spasific cordinations
int find_path_to_digger_len(int xE, int yE, int direction) {
	int can_right, can_left, can_up, can_down, path_amount;
	int path_len = 0;
	int rand, time_to_live = MAX_PATH_LEN;
	
	//while the digger is not next to the cordinations, and there is still time to live
	while (!is_digger_next_to_me(yE, xE) && time_to_live > 0) {
		time_to_live--;
		
		//check what are to posilbe paths
		can_right = move_is_possible(yE,xE,  RIGHT_ARROW, 0);
		can_left = move_is_possible(yE,xE,  LEFT_ARROW, 0);
		can_up = move_is_possible(yE,xE,  UP_ARROW, 0);
		can_down = move_is_possible(yE,xE,  DOWN_ARROW, 0);
		path_amount = can_right + can_left + can_up + can_down;
		
		//there are no posible path
		if (path_amount == 0) return 999;
		
		//if there are 1 ot two posible paths take the path that dont get you back (if exsist)
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
		
		//if there are 3 or 4 posible paths take randomaly one of the posible path(that will not get you back)
		else if (path_amount == 3 || path_amount == 4) {
			rand = tod % 4;
			
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
	
	if (is_digger_next_to_me(yE, xE)) return path_len + 1; //if the while stop becouse the digger is next to the enemy
	return 999; //if the while stop becouse it done MAX_PATH_LEN iterations
}

//get the index of the minimal val
int min_index(int v1, int v2, int v3, int v4) {
	if(v1 <= v2 && v1 <= v3 && v1 <= v4) return 1;
	else if(v2 <= v1 && v2 <= v3 && v2 <= v4) return 2;
	else if(v3 <= v1 && v3 <= v2 && v3 <= v4) return 3;
	else if(v4 <= v1 && v4 <= v2 && v4 <= v3) return 4;
	
	return 0;
}

//get the index of the max val
int max_index(int v1, int v2, int v3, int v4) {
	if(v1 >= v2 && v1 >= v3 && v1 >= v4) return 1;
	else if(v2 >= v1 && v2 >= v3 && v2 >= v4) return 2;
	else if(v3 >= v1 && v3 >= v2 && v3 >= v4) return 3;
	else if(v4 >= v1 && v4 >= v2 && v4 >= v3) return 4;
	
	return 0;
}

//create evry 5 second new nobin until the max amount of nobins for the level as created. 
//if the max amount of nobins on map is currently alive so nobins wont be created until one of them died
void nobbin_creator() {
	int i, lowest_dead_nobbin = -1;

	while(1) {
		sleept(SECONDT*5);
		while(gameMap.monster_max_amount > 0) {
			disp_draw_pixel_with_char(0, 69, BABY_BG, ' ');
			all_enemys_created = 0;
			if (number_of_live_enemys() < gameMap.monster_start_amount){
				lowest_dead_nobbin = get_lowest_dead_nobbin();
				if(lowest_dead_nobbin >= 0) {
					enemys[lowest_dead_nobbin] = create_enemy();
					enemys[lowest_dead_nobbin].is_alive = 1;
					gameMap.monster_max_amount--;
				}
			}
			disp_draw_pixel_with_char(0, 69, BLACK_BG, ' ');
			sleept(SECONDT*5);
		}
		all_enemys_created = 1;
	}
}

//return the loweset index of dead nobin. return -1 if all the nobins are alive
int get_lowest_dead_nobbin() {
	int i;
	for(i = 0; i < ENEMY_COUNT; i++)
		if (!enemys[i].is_alive) return i;
	return -1;
}

//mark all the live enemys as died
void kill_all_enemys() {
	int i;
	for(i = 0; i < ENEMY_COUNT; i++){
		if(enemys[i].is_alive) {
			upd_draw_empty(enemys[i].y, enemys[i].x, 1);
			enemys[i].is_alive = 0; //kil enemy	
		}
	}
}

//count the number of enemys alive
int number_of_live_enemys() {
	int i, ret = 0, ps;
	disable(ps);
	
	for(i = 0; i < ENEMY_COUNT; i++)
		if (enemys[i].is_alive) ret++;
	
	restore(ps);
	return ret;
}
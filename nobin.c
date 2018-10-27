#include "digger.h"
#include "nobin.h"
#include "map.h"
#include "myints.h"


void move_nobbins(){
	int i,j,timer;
	char direction;
	while(1) {
		receive();
		for(i=0;i<NOBBIN_COUNT;i++){
			if(enemys[i].is_alive){
				direction = find_direction_to_digger(enemys[i]);
				//sprintf(debug_str,"nobbin direction: %d | LEFT=%d , RIGHT=%d , UP=%d , DOWN=%d",direction, LEFT_ARROW,RIGHT_ARROW,UP_ARROW,DOWN_ARROW);
				//send(debug,debug_str);
				draw_empty(enemys[i].y,enemys[i].x,1);
				switch (direction)
				{
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
				enemys[i].direction=direction;
				draw_nobbin(enemys[i]);
				draw_area(enemys[i].y,enemys[i].x);
			}
		}
		
	}
}
char find_direction_to_digger(Nobbin n){
	//char dir[4] = { UP_ARROW , RIGHT_ARROW , DOWN_ARROW , LEFT_ARROW };
	int diff_x = n.x-(*(n.digger)).x, diff_y=n.y-(*(n.digger)).y,abs_diff_x,abs_diff_y,can_right,can_left,can_up,can_down;
	if(diff_x<0) abs_diff_x = -1*diff_x;
	if(diff_y<0) abs_diff_y = -1*diff_y;
	if(move_is_possible(n.x,n.y, n.direction, n.is_hobbin)) return n.direction;
	can_right = move_is_possible(n.x,n.y, RIGHT_ARROW, n.is_hobbin);
	can_left = move_is_possible(n.x,n.y, LEFT_ARROW, n.is_hobbin);
	can_up = move_is_possible(n.x,n.y, UP_ARROW, n.is_hobbin);
	can_down = move_is_possible(n.x,n.y, DOWN_ARROW, n.is_hobbin);
	if(n.direction==RIGHT_ARROW)can_left=0;
	if(n.direction==LEFT_ARROW)can_right=0;
	if(n.direction==UP_ARROW)can_down=0;
	if(n.direction==DOWN_ARROW)can_up=0;
	
	if(abs_diff_x > abs_diff_y) {
		if (diff_x>0) {
			if(can_right) return RIGHT_ARROW;
		}
		else {
			if(can_left) return LEFT_ARROW;
		}
	} else {
		if (diff_y>0) {
			if(can_up) return UP_ARROW;
		}
		else {
			if(can_down) return DOWN_ARROW;
		}
	}
	
	if (can_right) return RIGHT_ARROW;
	else if(can_left) return LEFT_ARROW;
	else if(can_up) return UP_ARROW;
	else if(can_down) return DOWN_ARROW;
	
	return 0;
}

Nobbin create_nobbin(Digger *d){ 
	Nobbin n;
	n.x = 14;
	n.y = 0;
	n.direction = LEFT_ARROW;
	n.digger = d;
	n.is_alive=0;
	n.is_hobbin=0;
	return n;
}


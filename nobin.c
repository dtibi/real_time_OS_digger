#include "digger.h"
#include "nobin.h"

void move_nobbin(){

}
char find_direction_to_digger(Nobbin n){
	int diff_x = n.x-(*(n.digger)).x, diff_y=n.y-(*(n.digger)).y,abs_diff_x,abs_diff_y;
	if(diff_x<0) abs_diff_x = -1*diff_x;
	if(diff_y<0) abs_diff_y = -1*diff_y;
	if(abs_diff_x > abs_diff_y) {
		if (diff_x>0) return 'r';
		else return 'l';
	} else {
		if (diff_y>0) return 'u';
		else return 'd';
	}
}

Nobbin create_nobbin(Digger *d){ 
	Nobbin n;
	n.x = 73;
	n.y = 2;
	n.direction = 'l';
	n.digger = d;
	return n;
}


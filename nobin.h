#ifndef NOBIN_FILE
#define NOBIN_FILE
#include "digger.h"

typedef struct enemy {
	Digger *digger;
	int x;
	int y;
	char direction; 
	char is_alive;
	int is_hobin;
} Enemy;

void move_enemies();
int find_direction_to_digger(Enemy enemy);
int find_path_to_digger_len(int xE, int yE, int direction, int start_x, int start_y, int tryed_up, int tryed_down, int tryed_right, int tryed_left);
Enemy create_enemy(Digger *d);

#endif

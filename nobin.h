#ifndef NOBIN_FILE
#define NOBIN_FILE
#include "digger.h"

#define MAX_PATH_LEN 127 //ROWS * COLUMNS - 1
#define NOBBIN_COUNT 5

typedef struct enemy {
	Digger *digger;
	int x;
	int y;
	char direction; 
	char is_alive;
	int is_hobin;
} Enemy;

void move_enemies();
void kill_all_enemys();
void create_enemys();
int find_direction_to_digger(Enemy enemy);
int find_path_to_digger_len(int xE, int yE, int direction);
Enemy create_enemy(Digger *d);

#endif

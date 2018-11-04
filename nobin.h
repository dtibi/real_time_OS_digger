#ifndef NOBIN_FILE
#define NOBIN_FILE
#include "digger.h"

#define MAX_PATH_LEN 127 //ROWS * COLUMNS - 1
#define ENEMY_COUNT 15

extern int all_enemys_created;

typedef struct enemy {
	Digger *digger;
	int x;
	int y;
	char direction; 
	int is_hobin;
	long last_time_hobin;
} Enemy;

void move_nobbin(int i);
void move_enemies();
void kill_all_enemys();
int find_direction_to_digger(Enemy enemy);
int find_path_to_digger_len(int xE, int yE, int direction);
Enemy create_enemy(int i);
void nobbin_creator();
int number_of_live_enemys();

#endif

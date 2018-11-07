#ifndef NOBIN_FILE
#define NOBIN_FILE
#include "digger.h"

#define MAX_PATH_LEN 127 //ROWS * COLUMNS - 1
#define ENEMY_COUNT 15

extern int all_enemys_created;

typedef struct enemy {
	int x;
	int y;
	char direction; 
	int is_hobin;
	long last_time_hobin;
	char is_alive;
} Enemy;

Enemy create_enemy();
void move_nobbin(int i);
int find_direction_to_digger(Enemy enemy);
int find_path_to_digger_len(int xE, int yE, int direction);
int min_index(int v1, int v2, int v3, int v4);
int max_index(int v1, int v2, int v3, int v4);
void nobbin_creator();
int get_lowest_dead_nobbin();
void kill_all_enemys();
int number_of_live_enemys();

#endif

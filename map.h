#ifndef MAP_FILE
#define MAP_FILE

#include "digger.h"
#include "nobin.h"

#define ROWS 8
#define COLUMNS 16
#define ROWS_PIXELS 25
#define COLUMNS_PIXELS 80

#define WIDTH 5
#define HEIGHT 3 

#define RED 4
#define GREEN 2

#define BLACK_BG 7
#define BLUE_BG 16
#define GREEN_BG 38
#define BABY_BG 48
#define RED_BG 64
#define PURPLE_BG 80
#define BROWN_BG 96
#define GRAY_BG 0x70
#define BROWN_ON_RED 70
#define GREEN_ON_BLACK 2
#define BROWN_ON_BLACK 6
#define GRAY_ON_BROWN 103
#define RED_ON_GREEN 36
#define BROWN_ON_GREEN 38

#define EMPTY 0
#define DIRT 1
#define DIAMOND 2
#define GOLD_BAG 3
#define MOVING_GOLD_BAG 4
#define CHERRY 5
#define FIREBALL 6
#define GOLD 10
#define NOBBIN 88
#define HOBBIN 89
#define DIGGER 99
#define DEAD_DIGGER 100


#define DIAMOND_SCORE 250
#define GOLD_NUGGET_SCORE 100
#define DEAD_ENEMY_SCORE 250
#define CHERRY_SCORE 1000

#define NUMBER_OF_LEVELS 3

#define start_address 0xB800

typedef struct map {
	char level_map[ROWS][COLUMNS];
	int level_id;
	char pixel_map[ROWS_PIXELS][COLUMNS_PIXELS][2];
	char refresh_map[ROWS][COLUMNS];
	int diamond_amount;
	int monster_start_amount;
	int monster_max_amount;
	int monster_become_angry_time;
	int monster_angry_for_time;
	int digger_reload_time;
	} Map;

extern Map gameMap;
extern Digger player;
extern Enemy enemys[ENEMY_COUNT];
extern int crazy_mode;
extern char* debug_str;


//unsigned static int start_address = 0xB800; //B800h
static char levels [NUMBER_OF_LEVELS][ROWS][COLUMNS] = {{
	{0 , 1 , 3 , 1 , 3 , 1 , 1 , 1 , 1 , 1 , 1 , 1 , 1 , 0 , 0 , 0},
	{0 , 2 , 2 , 1 , 1 , 1 , 1 , 1 , 1 , 1 , 1 , 1 , 1 , 0 , 3 , 1},
	{0 , 2 , 2 , 1 , 1 , 2 , 3 , 1 , 1 , 1 , 1 , 1 , 1 , 0 , 1 , 1},
	{0 , 2 , 2 , 3 , 1 , 2 , 1 , 1 , 1 , 1 , 1 , 1 , 1 , 0 , 2 , 2},
	{0 , 1 , 1 , 1 , 1 , 2 , 1 , 1 , 3 , 1 , 1 , 1 , 1 , 0 , 2 , 2},
	{0 , 0 , 0 , 0 , 1 , 2 , 1 , 1 , 1 , 1 , 1 , 1 , 1 , 0 , 1 , 1},
	{2 , 1 , 1 , 0 , 0 , 1 , 1 , 1 , 1 , 1 , 1 , 1 , 1 , 0 , 1 , 2},
	{2 , 2 , 1 , 1 , 0 , 0 , 0 , 0 , 0 , 0 , 0 , 0 , 0 , 0 , 2 , 2} 
}, {
	{0 , 0 , 0 , 0 , 0 , 0 , 0 , 0 , 1 , 1 , 3 , 1 , 3 , 1 , 1 , 0 },
	{1 , 1 , 3 , 2 , 2 , 3 , 1 , 0 , 1 , 2 , 2 , 2 , 2 , 2 , 1 , 0 },
	{1 , 1 , 2 , 2 , 2 , 2 , 1 , 0 , 1 , 1 , 1 , 1 , 1 , 1 , 1 , 0 },
	{1 , 1 , 2 , 2 , 2 , 2 , 1 , 0 , 1 , 3 , 1 , 1 , 0 , 0 , 0 , 0 },
	{1 , 1 , 1 , 2 , 2 , 1 , 1 , 0 , 1 , 2 , 2 , 1 , 0 , 1 , 1 , 1 },
	{1 , 1 , 1 , 3 , 3 , 1 , 1 , 0 , 2 , 2 , 2 , 2 , 0 , 2 , 2 , 2 },
	{2 , 1 , 2 , 1 , 1 , 1 , 1 , 0 , 1 , 2 , 2 , 1 , 0 , 2 , 2 , 2 },
	{2 , 1 , 2 , 2 , 1 , 1 , 1 , 0 , 0 , 0 , 0 , 0 , 0 , 1 , 1 , 1 } 
}, {
	{0 , 0 , 0 , 0 , 0 , 3 , 1 , 3 , 1 , 3 , 0 , 0 , 0 , 0 , 0 , 0},
    {1 , 3 , 3 , 1 , 0 , 2 , 2 , 1 , 2 , 1 , 0 , 2 , 2 , 2 , 2 , 2},
	{2 , 2 , 2 , 2 , 0 , 2 , 2 , 1 , 2 , 1 , 0 , 2 , 2 , 2 , 2 , 2},
	{2 , 2 , 2 , 2 , 0 , 0 , 0 , 0 , 0 , 0 , 0 , 1 , 1 , 2 , 2 , 1},
	{1 , 2 , 2 , 1 , 1 , 2 , 1 , 0 , 1 , 2 , 1 , 1 , 1 , 2 , 2 , 1},
	{1 , 2 , 2 , 1 , 1 , 2 , 1 , 0 , 1 , 2 , 1 , 1 , 1 , 1 , 1 , 1},
	{2 , 1 , 1 , 1 , 1 , 2 , 1 , 0 , 1 , 2 , 1 , 1 , 1 , 1 , 1 , 2},
	{2 , 2 , 1 , 1 , 1 , 2 , 1 , 0 , 1 , 2 , 1 , 1 , 1 , 1 , 2 , 2} 
}};
	
int monster_count [NUMBER_OF_LEVELS] = { 5, 5, 10 };
int monster__max_count [NUMBER_OF_LEVELS] = { 8, 10, 15 };
int become_hobin [NUMBER_OF_LEVELS] = { 90,60,60  };
int hobin_time [NUMBER_OF_LEVELS] = { 15,20,30  };
int digger_time [NUMBER_OF_LEVELS] = { 2,4,7};
void setup_clean_screen();
void disp_draw_map();
void next_level();
void create_map(int level_id);

int  get_object_in_direction(int y, int x, int direction);
void disp_draw_pixel_with_char(int row, int col, char color,char ch);
void upd_draw_diamond(int i,int j);
void upd_draw_dirt(int i,int j);
void upd_draw_bag(int i,int j);
void upd_draw_bag_moving(int i,int j,int direction);
void upd_draw_empty(int i,int j,int update_map);
void upd_draw_nobbin(int i, int j);
void upd_draw_hobbin(int i, int j,int direction);
void disp_draw_area(int y, int x);
void disp_draw_lives(int lives);
void score_lives_updater();
void disp_draw_score(int score);
void upd_draw_digger(Digger player);
void disp_draw_cube(int i,int j);
int  move_is_possible(int y, int x, int direction, int i_can_dig);
void gold_falling(int y, int x);
int  is_current_pixel_digger(int y, int x);
void fireball_advance(int y, int x, int direction);
void refresh_map();
void draw_debug_line(char* str);
void refresh_debug_map();
void restart_game();
#endif

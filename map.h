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

#define DIGGER 99
#define EMPTY 0
#define NOBBIN 88
#define GOLD_BAG 3
#define MOVING_GOLD_BAG 4
#define GOLD 6
#define DIRT 1
#define DIAMOND 2
#define HOBBIN 89
#define DEAD_DIGGER 100

#define NOBBIN_COUNT 5
#define start_address 0xB800

typedef struct map {
	char level_map[ROWS][COLUMNS];
	char pixel_map[ROWS_PIXELS][COLUMNS_PIXELS][2];
	char refresh_map[ROWS][COLUMNS];
	} Map;

extern Map gameMap;
extern Digger player;
extern Nobbin enemys[NOBBIN_COUNT];
extern char* debug_str;


//unsigned static int start_address = 0xB800; //B800h
static char level_1[ROWS_PIXELS][COLUMNS_PIXELS];
static char level_0[ROWS][COLUMNS] = {
	0 , 1 , 3 , 1 , 3 , 1 , 1 , 1 , 1 , 1 , 1 , 1 , 1 , 0 , 0 , 0 ,
    0 , 2 , 2 , 1 , 1 , 1 , 1 , 1 , 1 , 1 , 1 , 1 , 1 , 0 , 3 , 1 ,
	0 , 2 , 2 , 1 , 1 , 2 , 3 , 1 , 1 , 1 , 1 , 1 , 1 , 0 , 1 , 1 ,
	0 , 2 , 2 , 3 , 1 , 2 , 1 , 1 , 1 , 1 , 1 , 1 , 1 , 0 , 2 , 2 ,
	0 , 1 , 1 , 1 , 1 , 2 , 1 , 1 , 3 , 1 , 1 , 1 , 1 , 0 , 2 , 2 ,
	0 , 0 , 0 , 0 , 1 , 2 , 1 , 1 , 1 , 1 , 1 , 1 , 1 , 0 , 1 , 1 ,
	2 , 1 , 1 , 0 , 0 , 1 , 1 , 1 , 1 , 1 , 1 , 1 , 1 , 0 , 1 , 2 ,
	2 , 2 , 1 , 1 , 0 , 0 , 0 , 0 , 0 , 0 , 0 , 0 , 0 , 0 , 2 , 2 
};

	
void setup_clean_screen();
void disp_draw_map();

int get_object_in_direction(int x, int y, int direction);
void disp_draw_pixel(int row, int col, char color);
void disp_draw_pixel_with_char(int row, int col, char color,char ch);
void upd_draw_diamond(int i,int j);
void upd_draw_dirt(int i,int j);
void upd_draw_bag(int i,int j);
void upd_draw_bag_moving(int i,int j,int direction);
void upd_draw_empty(int i,int j,int update_map);
void upd_draw_nobbin(int i, int j);
void disp_draw_area(int y, int x);
void upd_draw_digger(Digger player);
void disp_draw_cube_area(int i,int j);
int move_is_possible(int x,int y, int direction, int i_can_dig);
void gold_falling(int x, int y);
void refresh_map();
void draw_debug_line(char* str);
void refresh_debug_map();
void restart_game();
#endif

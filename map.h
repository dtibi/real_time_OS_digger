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
#define GRAY_BG 112
#define BROWN_ON_RED 70
#define GREEN_ON_BLACK 2
#define BROWN_ON_BLACK 6
#define GRAY_ON_BROWN 103

#define NOBBIN_COUNT 5
#define start_address 0xB800

typedef struct map {
	char level_map[ROWS][COLUMNS];
	char pixel_map[ROWS_PIXELS][COLUMNS_PIXELS][2];
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

	
void clean_screen();
void create_map();

int getNextPixelType(int x, int y, int direction);
void draw_pixel(int row, int col, char color);
void draw_pixel_with_char(int row, int col, char color,char ch);
void draw_diamond(int i,int j);
void draw_dirt(int i,int j);
void draw_bag(int i,int j);
void draw_empty(int i,int j,int update_map);
void draw_nobbin(Nobbin n);
void draw_area(int y, int x);
void draw_digger(Digger player);
int move_is_possible(int x,int y, int direction, int i_can_dig);
void refresh_map();
void draw_debug_line(char* str);
void refresh_debug_map();

#endif

#ifndef MAP_FILE
#define MAP_FILE

#include "digger.h"

#define ROWS 8
#define COLUMNS 16
#define ROWS_PIXELS 25
#define COLUMNS_PIXELS 80

#define WIDTH 5 //note WIDTH = floor(COLUMNS_PIXELS/COLUMNS)
#define HEIGHT 3 //note HEIGHT = floor(ROWS_PIXELS/ ROWS)

#define BLACK_BG 7
#define BLUE_BG 16
#define GREEN_BG 38
#define BABY_BG 48
#define RED_BG 64
#define PURPLE_BG 80
#define BROWN_BG 98
#define GRAY_BG 112
#define BROWN_ON_RED 70
#define GREEN_ON_BLACK 2
#define BROWN_ON_BLACK 6
#define GRAY_ON_BROWN 103

typedef struct map {
	char currentLevel[ROWS][COLUMNS];
	char current_map[ROWS_PIXELS][COLUMNS_PIXELS][2];
	} Map;

extern volatile Digger player;
extern volatile Map gameMap;
extern char* str;
static int start_address = 47104; //B800h
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
Map create_map();

int getNextPixelType(int x, int y, int direction);
void draw_pixel(int row, int col, char color);
void draw_diamond(unsigned int i,unsigned int j);
void draw_dirt(unsigned int i,unsigned int j);
void draw_bag(unsigned int i,unsigned int j);
void draw_empty(unsigned int i,unsigned int j);
int move_is_possible(int x, int y, int direction, int i_can_dig);
void draw_dig(unsigned int i,unsigned int j);
void refresh_map();

#endif

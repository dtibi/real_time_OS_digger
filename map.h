#ifndef MAP_FILE
#define MAP_FILE

#include "digger.h"
#include "nobin.h"

#define WIDTH 5
#define HEIGHT 3
#define ROWS 8
#define COLUMNS 16
#define ROWS_PIXELS 25
#define COLUMNS_PIXELS 80

#define RED 4
#define BLACK_BG 0
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

extern volatile Digger player;
extern volatile Nobbin enemys[NOBBIN_COUNT];
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


typedef struct map {
	char id;
	char dirt[25][80];
	} Map;

	
void clean_screen();
void create_map();

void draw_pixel(int row, int col, char color);
void draw_diamond(unsigned int i,unsigned int j);
void draw_dirt(unsigned int i,unsigned int j);
void draw_bag(unsigned int i,unsigned int j);
void draw_empty(unsigned int i,unsigned int j);
void draw_nobbins(Nobbin n[NOBBIN_COUNT]);
void draw_area(int y, int x);
int move_is_possible(int x,int y, int direction, int i_can_dig);
void draw_dig(unsigned int i,unsigned int j);
void refresh_map(Digger *player);

#endif

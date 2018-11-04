#include "map.h"
#include "digger.h"
#include "nobin.h"
#include "myints.h"
#include "nobin.h"

volatile Digger player;
Map gameMap;
int crazy_mode;
volatile Enemy enemys[ENEMY_COUNT];
char* debug_str;
int enemys_pid[ENEMY_COUNT];
int enemys_int[ENEMY_COUNT];
int enemys_proccess_is_alive[ENEMY_COUNT];


void draw_debug_line(char *str) {
	int i,str_len;
	
	str_len = (strlen(str) > COLUMNS_PIXELS) ? COLUMNS_PIXELS : strlen(str);
	for (i = 0; i < str_len && i < COLUMNS_PIXELS; i++)
		disp_draw_pixel_with_char(0, i, 2, str[i]);
	
	for(; i < COLUMNS_PIXELS; i++)
		disp_draw_pixel_with_char(0, i, 2, ' ');
}

void setup_clean_screen() {
	asm {
	MOV AH, 0  // Select function = 'Set mode'
	MOV AL, 3  // 80 by 25 color image
	INT 10h // Adapter initialized. Page 0 displayed	

	MOV AX, 0B800h
	MOV ES, AX
	MOV DI, 0
	MOV AL, ' '
	MOV AH, 7
	MOV CX, 2000
	CLD
	REP STOSW
	}
}

void disp_draw_pixel_with_char(int row, int col, char color, char ch) {
	unsigned int screen_address = start_address + row * 10; 
	int col_2 = col * 2;
	char c = ch;
	if (row < 0 || row >= ROWS_PIXELS || col < 0 || col >= COLUMNS_PIXELS) {
		printf("WARNING! skipping illegal screen address  x:%d , y:%d color:%d , char:%c \n", col, row,color,ch);
		return;
	}
	
	asm {
		PUSH AX
		PUSH ES
		PUSH DI
		MOV AX, screen_address
		MOV ES, AX
		MOV DI, col_2
		MOV AL, c
		MOV AH, color
		MOV ES:[DI], AX
		POP DI
		POP ES
		POP AX
	} 
}

/*  upd_draw_diamond(int i, int j)
	                       _____
 diamond will look like:  |_   _| and colored (GREEN)
                            |_|
*/
void upd_draw_diamond(int i, int j) {
	int row_pixel = row_2_pixel(i), column_pixel = column_2_pixel(j), k, l;
	upd_draw_dirt(i, j);
	gameMap.level_map[i][j] = DIAMOND;
	gameMap.pixel_map[row_pixel + 1][column_pixel + 1][0] = ' ';
	gameMap.pixel_map[row_pixel + 1][column_pixel + 2][0] = ' ';
	gameMap.pixel_map[row_pixel + 1][column_pixel + 3][0] = ' ';
	gameMap.pixel_map[row_pixel + 2][column_pixel + 2][0] = ' ';
	gameMap.pixel_map[row_pixel + 1][column_pixel + 1][1] = GREEN_BG;
	gameMap.pixel_map[row_pixel + 1][column_pixel + 2][1] = GREEN_BG;
	gameMap.pixel_map[row_pixel + 1][column_pixel + 3][1] = GREEN_BG;
	gameMap.pixel_map[row_pixel + 2][column_pixel + 2][1] = GREEN_BG;
	gameMap.refresh_map[i][j] = 1;
}

/* upd_draw_bag(unsigned int i, unsigned int j)
	                              w
Gold bags will look like:		 / \
						   		|_$_|
*/
void upd_draw_bag(int i, int j) {
	int row_pixel = row_2_pixel(i), column_pixel = column_2_pixel(j);
	
	gameMap.pixel_map[row_pixel][column_pixel + 2][0] = 'w';
	gameMap.pixel_map[row_pixel + 1][column_pixel + 1][0] = '/';
	gameMap.pixel_map[row_pixel + 1][column_pixel + 2][0] = ' ';
	gameMap.pixel_map[row_pixel + 1][column_pixel + 3][0] = '\\';
	gameMap.pixel_map[row_pixel + 2][column_pixel + 1][0] = ' ';
	gameMap.pixel_map[row_pixel + 2][column_pixel + 2][0] = '$';
	gameMap.pixel_map[row_pixel + 2][column_pixel + 3][0] = ' ';
	gameMap.pixel_map[row_pixel + 2][column_pixel + 1][1] = GRAY_BG;
	gameMap.pixel_map[row_pixel + 2][column_pixel + 2][1] = GRAY_BG;
	gameMap.pixel_map[row_pixel + 2][column_pixel + 3][1] = GRAY_BG;
	gameMap.pixel_map[row_pixel][column_pixel + 2][1] = GRAY_ON_BROWN;
	gameMap.pixel_map[row_pixel + 1][column_pixel + 1][1] = GRAY_ON_BROWN;
	gameMap.pixel_map[row_pixel + 1][column_pixel + 2][1] = GRAY_BG;
	gameMap.pixel_map[row_pixel + 1][column_pixel + 3][1] = GRAY_ON_BROWN;
	
	gameMap.level_map[i][j] = GOLD_BAG;
	gameMap.refresh_map[i][j] = 1;
}

void upd_draw_bag_moving(int i,int j,int direction){
	int row_pixel = row_2_pixel(i), column_pixel = column_2_pixel(j);
	gameMap.level_map[i][j]=MOVING_GOLD_BAG;
	if(direction==DOWN_ARROW){
		gameMap.level_map[i+1][j]=MOVING_GOLD_BAG;
		while(row_pixel<=row_2_pixel(i)+HEIGHT){
			gameMap.pixel_map[row_pixel][column_pixel+2][0] = 'w';
			gameMap.pixel_map[row_pixel+1][column_pixel+1][0] = '/';
			gameMap.pixel_map[row_pixel+1][column_pixel+2][0] = ' ';
			gameMap.pixel_map[row_pixel+1][column_pixel+3][0] = '\\';
			gameMap.pixel_map[row_pixel+2][column_pixel+1][0] = ' ';
			gameMap.pixel_map[row_pixel+2][column_pixel+2][0] = '$';
			gameMap.pixel_map[row_pixel+2][column_pixel+3][0] = ' ';
			gameMap.pixel_map[row_pixel+2][column_pixel+1][1] = GRAY_BG;
			gameMap.pixel_map[row_pixel+2][column_pixel+2][1] = GRAY_BG;
			gameMap.pixel_map[row_pixel+2][column_pixel+3][1] = GRAY_BG;
			gameMap.pixel_map[row_pixel][column_pixel+2][1] = GRAY_ON_BROWN;
			gameMap.pixel_map[row_pixel+1][column_pixel+1][1] = GRAY_ON_BROWN;
			gameMap.pixel_map[row_pixel+1][column_pixel+2][1] = GRAY_BG;
			gameMap.pixel_map[row_pixel+1][column_pixel+3][1] = GRAY_ON_BROWN;
			if(row_pixel!=row_2_pixel(i)){
				gameMap.pixel_map[row_pixel-1][column_pixel+2][0] = ' ';
				gameMap.pixel_map[row_pixel][column_pixel+1][0] = ' ';
				gameMap.pixel_map[row_pixel][column_pixel+3][0] = ' ';
				gameMap.pixel_map[row_pixel-1][column_pixel+2][1] = BLACK_BG;
				gameMap.pixel_map[row_pixel][column_pixel+1][1] = BLACK_BG;
				gameMap.pixel_map[row_pixel][column_pixel+3][1] = BLACK_BG;
			} else {
				gameMap.pixel_map[row_pixel][column_pixel+1][0] = ' ';
				gameMap.pixel_map[row_pixel][column_pixel+3][0] = ' ';
				gameMap.pixel_map[row_pixel][column_pixel+1][1] = BLACK_BG;
				gameMap.pixel_map[row_pixel][column_pixel+3][1] = BLACK_BG;
			}
			gameMap.refresh_map[i][j] = 1;
			gameMap.refresh_map[i+1][j] = 1;
			sleept(SECONDT/10);
			row_pixel++;
		}
	} 
	
	gameMap.level_map[i][j] = EMPTY;
	gameMap.refresh_map[i][j] = 1;
}
/* 								  _  _  _
open bag will look like this:    |_||_||_|
								 |*||*||*|
 */
void upd_draw_open_bag(int i, int j, int num){
	int row_pixel = row_2_pixel(i), column_pixel = column_2_pixel(j);
	gameMap.level_map[i][j] = GOLD+num;
	upd_draw_empty(i, j, 0);
	if(num == 3) {
		gameMap.pixel_map[row_pixel + 2][column_pixel][0] = ' ';
		gameMap.pixel_map[row_pixel + 2][column_pixel + 2][0] = ' ';
		gameMap.pixel_map[row_pixel + 2][column_pixel + 4][0] = ' ';
		gameMap.pixel_map[row_pixel + 2][column_pixel][1] = GRAY_BG;
		gameMap.pixel_map[row_pixel + 2][column_pixel + 2][1] = GRAY_BG;
		gameMap.pixel_map[row_pixel + 2][column_pixel + 4][1] = GRAY_BG;
	} else if(num==2) {
		gameMap.pixel_map[row_pixel + 2][column_pixel + 1][0] = ' ';
		gameMap.pixel_map[row_pixel + 2][column_pixel + 3][0] = ' ';
		gameMap.pixel_map[row_pixel + 2][column_pixel + 1][1] = GRAY_BG;
		gameMap.pixel_map[row_pixel + 2][column_pixel + 3][1] = GRAY_BG;
	} else if(num==4) {
		gameMap.pixel_map[row_pixel + 2][column_pixel][0] = ' ';
		gameMap.pixel_map[row_pixel + 2][column_pixel + 2][0] = ' ';
		gameMap.pixel_map[row_pixel + 2][column_pixel + 4][0] = ' ';
		gameMap.pixel_map[row_pixel + 1][column_pixel + 1][0] = ' ';
		gameMap.pixel_map[row_pixel + 1][column_pixel + 1][1] = GRAY_BG;
		gameMap.pixel_map[row_pixel + 2][column_pixel][1] = GRAY_BG;
		gameMap.pixel_map[row_pixel + 2][column_pixel + 2][1] = GRAY_BG;
		gameMap.pixel_map[row_pixel + 2][column_pixel + 4][1] = GRAY_BG;
	} else if(num==5) {
		gameMap.pixel_map[row_pixel + 2][column_pixel][0] = ' ';
		gameMap.pixel_map[row_pixel + 2][column_pixel + 2][0] = ' ';
		gameMap.pixel_map[row_pixel + 2][column_pixel + 4][0] = ' ';
		gameMap.pixel_map[row_pixel + 1][column_pixel + 1][0] = ' ';
		gameMap.pixel_map[row_pixel + 1][column_pixel + 3][0] = ' ';
		gameMap.pixel_map[row_pixel + 1][column_pixel + 3][1] = GRAY_BG;
		gameMap.pixel_map[row_pixel + 1][column_pixel + 1][1] = GRAY_BG;
		gameMap.pixel_map[row_pixel + 2][column_pixel][1] = GRAY_BG;
		gameMap.pixel_map[row_pixel + 2][column_pixel + 2][1] = GRAY_BG;
		gameMap.pixel_map[row_pixel + 2][column_pixel + 4][1] = GRAY_BG;
	}
	gameMap.refresh_map[i][j] = 1;
}

/*
	                       
 fireball will look like:  *   and colored (RED)
                          *** 
						   *
*/
void upd_draw_fireball(int i,int j) {
	int row_pixel = row_2_pixel(i), column_pixel = column_2_pixel(j);
	// updating logical map to indicate fireball presence
	gameMap.level_map[i][j]= FIREBALL;
	// making the pixel empty before drawing fireball
	upd_draw_empty(row_pixel,column_pixel,0);
	// fireball pixel map update
	gameMap.pixel_map[row_pixel][column_pixel+2][0] = '*';
	gameMap.pixel_map[row_pixel][column_pixel+2][1] = RED;
	gameMap.pixel_map[row_pixel+1][column_pixel+1][0] = '*';
	gameMap.pixel_map[row_pixel+1][column_pixel+1][1] = RED;
	gameMap.pixel_map[row_pixel+1][column_pixel+2][0] = '*';
	gameMap.pixel_map[row_pixel+1][column_pixel+2][1] = RED;
	gameMap.pixel_map[row_pixel+1][column_pixel+3][0] = '*';
	gameMap.pixel_map[row_pixel+1][column_pixel+3][1] = RED;
	gameMap.pixel_map[row_pixel+2][column_pixel+2][0] = '*';
	gameMap.pixel_map[row_pixel+2][column_pixel+2][1] = RED;
	
	gameMap.refresh_map[i][j] = 1;
}

void upd_draw_dirt(int i,int j) {
	int row_pixel = row_2_pixel(i), column_pixel = column_2_pixel(j), k, l;
	gameMap.level_map[i][j] = DIRT;
	for (k = row_pixel; k < row_pixel + HEIGHT && k < ROWS_PIXELS; k++) {
		for (l = column_pixel; l < column_pixel + WIDTH && l < COLUMNS_PIXELS; l++) {
			gameMap.pixel_map[k][l][0] = ' ';
			gameMap.pixel_map[k][l][1] = BROWN_BG;
		}
	}
	gameMap.refresh_map[i][j] = 1;
}

void upd_draw_empty(int i, int j, int update_map) {
	int row_pixel = row_2_pixel(i), column_pixel = column_2_pixel(j), k, l;
	if(update_map == 1)
		gameMap.level_map[i][j] = EMPTY;
	
	for (k = row_pixel; k < row_pixel + HEIGHT && k<ROWS_PIXELS; k++){
		for (l = column_pixel; l < column_pixel + WIDTH && l < COLUMNS_PIXELS; l++){
			gameMap.pixel_map[k][l][0] = ' ';
			gameMap.pixel_map[k][l][1] = BLACK_BG;
		}
	}
	gameMap.refresh_map[i][j] = 1;
}

/* 
nobbin should look like:
    _   _
   |_|_|_|
     |_|
	/   \

 */
void upd_draw_nobbin(int i, int j) {
	int x = column_2_pixel(j),y = row_2_pixel(i);
	gameMap.level_map[i][j] = NOBBIN;
	upd_draw_empty(i, j, 0);
	gameMap.pixel_map[y + 1][x + 2][0] = ' ';
	gameMap.pixel_map[y + 1][x + 2][1] =  GREEN_BG;
	gameMap.pixel_map[y][x + 1][0] = '0';
	gameMap.pixel_map[y][x + 1][1] =  BROWN_BG;
	gameMap.pixel_map[y][x + 3][0] = '0';
	gameMap.pixel_map[y][x + 3][1] =  BROWN_BG;
	gameMap.pixel_map[y + 2][x + 1][0] = '/';
	gameMap.pixel_map[y + 2][x + 1][1] =  RED;
	gameMap.pixel_map[y + 2][x + 3][0] = '\\';
	gameMap.pixel_map[y + 2][x + 3][1] =  RED;

	gameMap.refresh_map[i][j] = 1;
}

/* 
hobbin should look like:
     ___       ___
    | 0 |     | 0 |
    |__<|     |>__|
	 _|_       _|_

 */
void upd_draw_hobbin(int i, int j,int direction) {
	int x = column_2_pixel(j),y = row_2_pixel(i);
	gameMap.level_map[i][j] = HOBBIN;
	upd_draw_empty(i, j, 0);
	gameMap.pixel_map[y][x + 1][0] = ' ';
	gameMap.pixel_map[y][x + 1][1] =  GREEN_BG;
	gameMap.pixel_map[y][x + 2][0] = '0';
	gameMap.pixel_map[y][x + 2][1] =  BROWN_ON_GREEN;
	gameMap.pixel_map[y][x + 3][0] = ' ';
	gameMap.pixel_map[y][x + 3][1] =  GREEN_BG;
	gameMap.pixel_map[y + 1][x + 2][0] = ' ';
	gameMap.pixel_map[y + 1][x + 2][1] =  GREEN_BG;
	if(direction == LEFT_ARROW || direction == UP_ARROW){
		gameMap.pixel_map[y + 1][x + 1][0] = '>';
		gameMap.pixel_map[y + 1][x + 1][1] =  RED_ON_GREEN;
		gameMap.pixel_map[y + 1][x + 3][0] = ' ';
		gameMap.pixel_map[y + 1][x + 3][1] =  GREEN_BG;
	} else {
		gameMap.pixel_map[y + 1][x + 1][0] = ' ';
		gameMap.pixel_map[y + 1][x + 1][1] =  GREEN_BG;
		gameMap.pixel_map[y + 1][x + 3][0] = '<';
		gameMap.pixel_map[y + 1][x + 3][1] =  RED_ON_GREEN;
	}
	gameMap.pixel_map[y + 2][x + 1][0] = '_';
	gameMap.pixel_map[y + 2][x + 1][1] =  RED;
	gameMap.pixel_map[y + 2][x + 2][0] = '|';
	gameMap.pixel_map[y + 2][x + 2][1] =  RED;
	gameMap.pixel_map[y + 2][x + 3][0] = '_';
	gameMap.pixel_map[y + 2][x + 3][1] =  RED;
	
	gameMap.refresh_map[i][j] = 1;
}

/* 
cherry should look like:
     
	   
      /\
	 @  @
     
 */
void upd_draw_cherry(int i, int j) {
	int x = column_2_pixel(j),y = row_2_pixel(i);
	gameMap.level_map[i][j] = CHERRY;
	upd_draw_empty(i, j, 0);
	
	gameMap.pixel_map[y+1][x+1][0] = '/';
	gameMap.pixel_map[y+1][x+1][1] = GREEN;
	gameMap.pixel_map[y+1][x+2][0] = '\\';
	gameMap.pixel_map[y+1][x+2][1] = GREEN;
	gameMap.pixel_map[y+2][x][0] = '@';
	gameMap.pixel_map[y+2][x][1] = RED;
	gameMap.pixel_map[y+2][x+3][0] = '@';
	gameMap.pixel_map[y+2][x+3][1] = RED;
	
	gameMap.refresh_map[i][j] = 1;
}

void disp_draw_area(int y, int x) {
	int i, j;
	for(i = row_2_pixel(y - 1); i < row_2_pixel(y + 1) + 1 && i < ROWS_PIXELS; i++) {
		if(i <= 0) continue;
		for(j = column_2_pixel(x - 1); j < column_2_pixel(x + 1) + 1 && j < COLUMNS_PIXELS; j++) {
			if(j < 0) continue;
			disp_draw_pixel_with_char(i, j, gameMap.pixel_map[i][j][1], gameMap.pixel_map[i][j][0]);
		}
	}
}

void upd_draw_digger(Digger player) {
	int x = column_2_pixel(player.x), y = row_2_pixel(player.y);
	int direction = player.direction;
	
	upd_draw_empty(player.y, player.x, 0);
	switch (direction) {
		case RIGHT_ARROW:
			gameMap.pixel_map[y + 1][x + 4][0] = '<';
			gameMap.pixel_map[y + 1][x + 3][0] = 'o';
			gameMap.pixel_map[y + 1][x + 2][0] = 'o';
			gameMap.pixel_map[y + 1][x + 4][1] = GREEN_ON_BLACK;
			gameMap.pixel_map[y + 1][x + 3][1] = BROWN_ON_RED;
			gameMap.pixel_map[y + 1][x + 2][1] = BROWN_ON_RED;
			break;
			
		case LEFT_ARROW:
			gameMap.pixel_map[y + 1][x][0] = '>';
			gameMap.pixel_map[y + 1][x + 1][0] = 'o';
			gameMap.pixel_map[y + 1][x + 2][0] = 'o';
			gameMap.pixel_map[y + 1][x][1] = GREEN_ON_BLACK;
			gameMap.pixel_map[y + 1][x + 1][1] = BROWN_ON_RED;
			gameMap.pixel_map[y + 1][x + 2][1] = BROWN_ON_RED;
			break;
			
		case UP_ARROW:
			gameMap.pixel_map[y][x + 2][0] = 'V';
			gameMap.pixel_map[y + 1][x + 2][0] = '8';
			gameMap.pixel_map[y][x + 2][1] = GREEN_ON_BLACK;
			gameMap.pixel_map[y + 1][x + 2][1] = BROWN_ON_RED;
			break;
			
		case DOWN_ARROW:
			gameMap.pixel_map[y + 2][x + 2][0] = '^';
			gameMap.pixel_map[y + 1][x + 2][0] = '8';
			gameMap.pixel_map[y + 2][x + 2][1] = GREEN_ON_BLACK;
			gameMap.pixel_map[y + 1][x + 2][1] = BROWN_ON_RED;
			break;
		
		default:
			return;
	}
	gameMap.level_map[player.y][player.x] = DIGGER;
}

void upd_draw_grave(int y, int x){
	int column_pixel=column_2_pixel(x),row_pixel=row_2_pixel(y),i,j;
	
	upd_draw_empty(y,x,0);
	gameMap.level_map[player.y][player.x] = DEAD_DIGGER;
	
	gameMap.pixel_map[row_pixel+2][column_pixel+3][0] = ' ';
	gameMap.pixel_map[row_pixel+2][column_pixel+1][0] = ' ';
	gameMap.pixel_map[row_pixel+2][column_pixel+2][0] = ' ';
	gameMap.pixel_map[row_pixel+2][column_pixel+3][1] = GRAY_BG;
	gameMap.pixel_map[row_pixel+2][column_pixel+1][1] = GRAY_BG;
	gameMap.pixel_map[row_pixel+2][column_pixel+2][1] = GRAY_BG;
	gameMap.refresh_map[player.y][player.x] = 1;
	sleept(SECONDT/2);
	
	gameMap.pixel_map[row_pixel+1][column_pixel+3][0] = ' ';
	gameMap.pixel_map[row_pixel+1][column_pixel+1][0] = ' ';
	gameMap.pixel_map[row_pixel+1][column_pixel+2][0] = ' ';
	gameMap.pixel_map[row_pixel+1][column_pixel+3][1] = GRAY_BG;
	gameMap.pixel_map[row_pixel+1][column_pixel+1][1] = GRAY_BG;
	gameMap.pixel_map[row_pixel+1][column_pixel+2][1] = GRAY_BG;
	
	
	gameMap.pixel_map[row_pixel+2][column_pixel+4][0] = ' ';
	gameMap.pixel_map[row_pixel+2][column_pixel+1][0] = 'R';
	gameMap.pixel_map[row_pixel+2][column_pixel+2][0] = 'I';
	gameMap.pixel_map[row_pixel+2][column_pixel+3][0] = 'P';
	gameMap.pixel_map[row_pixel+2][column_pixel+3][1] = GRAY_BG;
	gameMap.pixel_map[row_pixel+2][column_pixel+4][1] = GRAY_BG;
	gameMap.pixel_map[row_pixel+2][column_pixel+1][1] = GRAY_BG;
	gameMap.pixel_map[row_pixel+2][column_pixel+2][1] = GRAY_BG;
	gameMap.pixel_map[row_pixel+2][column_pixel][0] = ' ';
	gameMap.pixel_map[row_pixel+2][column_pixel][1] = GRAY_BG;
	gameMap.refresh_map[player.y][player.x] = 1;
	sleept(SECONDT/2);
	
	gameMap.pixel_map[row_pixel][column_pixel+3][0] = ' ';
	gameMap.pixel_map[row_pixel][column_pixel+1][0] = ' ';
	gameMap.pixel_map[row_pixel][column_pixel+2][0] = ' ';
	gameMap.pixel_map[row_pixel][column_pixel+3][1] = GRAY_BG;
	gameMap.pixel_map[row_pixel][column_pixel+1][1] = GRAY_BG;
	gameMap.pixel_map[row_pixel][column_pixel+2][1] = GRAY_BG;
	
	
	gameMap.pixel_map[row_pixel+1][column_pixel+4][0] = ' ';
	gameMap.pixel_map[row_pixel+1][column_pixel+1][0] = 'R';
	gameMap.pixel_map[row_pixel+1][column_pixel+2][0] = 'I';
	gameMap.pixel_map[row_pixel+1][column_pixel+3][0] = 'P';
	gameMap.pixel_map[row_pixel+1][column_pixel+3][1] = GRAY_BG;
	gameMap.pixel_map[row_pixel+1][column_pixel+4][1] = GRAY_BG;
	gameMap.pixel_map[row_pixel+1][column_pixel+1][1] = GRAY_BG;
	gameMap.pixel_map[row_pixel+1][column_pixel+2][1] = GRAY_BG;
	gameMap.pixel_map[row_pixel+1][column_pixel][0] = ' ';
	gameMap.pixel_map[row_pixel+1][column_pixel][1] = GRAY_BG;
	
	gameMap.pixel_map[row_pixel+2][column_pixel+4][0] = ' ';
	gameMap.pixel_map[row_pixel+2][column_pixel+1][0] = ' ';
	gameMap.pixel_map[row_pixel+2][column_pixel+2][0] = ' ';
	gameMap.pixel_map[row_pixel+2][column_pixel+3][0] = ' ';
	gameMap.pixel_map[row_pixel+2][column_pixel+3][1] = GRAY_BG;
	gameMap.pixel_map[row_pixel+2][column_pixel+4][1] = GRAY_BG;
	gameMap.pixel_map[row_pixel+2][column_pixel+1][1] = GRAY_BG;
	gameMap.pixel_map[row_pixel+2][column_pixel+2][1] = GRAY_BG;
	gameMap.pixel_map[row_pixel+2][column_pixel][0] = ' ';
	gameMap.pixel_map[row_pixel+2][column_pixel][1] = GRAY_BG;
	gameMap.refresh_map[player.y][player.x] = 1;
	sleept(SECONDT*2);
	
	gameMap.level_map[player.y][player.x] = EMPTY;
}

void restart_game() {
	send(terminate_xinu_pid);
}

int move_is_possible(int y, int x, int direction, int i_can_dig) {
	int obj_in_direction = get_object_in_direction(y,x, direction);
	if((direction == UP_ARROW    && y - 1 < 0	  	 ) ||
	   (direction == DOWN_ARROW  && y + 1 >= ROWS	 ) ||
	   (direction == RIGHT_ARROW && x + 1 >= COLUMNS ) ||
	   (direction == LEFT_ARROW  && x - 1 < 0		 ) ) return 0;
	
	if (direction != UP_ARROW && direction != DOWN_ARROW && direction != RIGHT_ARROW && direction != LEFT_ARROW) 
		return 0;
	
	if(i_can_dig && obj_in_direction != MOVING_GOLD_BAG)
		return 1;
	
	else{
		if	(obj_in_direction==EMPTY || obj_in_direction==CHERRY || (obj_in_direction >= GOLD+2 && obj_in_direction <= GOLD+5)) return 1;
	}
	
	return 0;
}

int pixel_2_row(int pixel_index ) {
	if(pixel_index<=0) return 0;
	if(pixel_index>=ROWS_PIXELS)return ROWS-1;
	return ((pixel_index-1)/HEIGHT);
}

int pixel_2_column(int pixel_index) {
	if(pixel_index<=0) return 0;
	if( pixel_index>=COLUMNS_PIXELS) return COLUMNS-1;
	return (pixel_index/WIDTH);
}

// row of screen
int row_2_pixel( int row_index ) {
	if(row_index<0) return 0;
	if(row_index>=ROWS) return ROWS_PIXELS-1;
	return ((row_index*HEIGHT)+1);
}

// offset of screen to be used for register DI
int column_2_pixel(int column_index) {
	if(column_index<=0) return 0;
	if(column_index>=COLUMNS) return COLUMNS_PIXELS-1;
	return (column_index*WIDTH);
}

//return the color of the pixel (BLACK_BG || GREEN_BG || BROWN_BG || )
int get_object_in_direction(int y, int x, int direction)
{	
	if		(direction==UP_ARROW    && (y-1) >= 0) 		return gameMap.level_map[y-1][x]; 
	else if (direction==DOWN_ARROW  && (y+1) < ROWS) 	return gameMap.level_map[y+1][x];
	else if (direction==RIGHT_ARROW && (x+1) < COLUMNS) return gameMap.level_map[y][x+1];
	else if (direction==LEFT_ARROW  && (x-1) >= 0) return gameMap.level_map[y][x-1];
	return -1;
}

int is_digger_next_to_me(int y, int x)
{	
	if		((y-1) >= 0 && gameMap.level_map[y-1][x] == DIGGER) return UP_ARROW; 
	else if ((y+1) < ROWS && gameMap.level_map[y+1][x] == DIGGER)	return DOWN_ARROW;
	else if ((x+1) < COLUMNS && gameMap.level_map[y][x+1] == DIGGER) return RIGHT_ARROW;
	else if ((x-1) >= 0 && gameMap.level_map[y][x-1] == DIGGER) return LEFT_ARROW;
	return 0;
}

void score_lives_updater(){
	int input;
	while(1){
		input = receive();
		continue;
		if(input==-1){
			disp_draw_lives(player.lives);
		} else {
			player.score+=input;
			disp_draw_score(player.score);
		}
	}
	
}

void disp_draw_lives(int lives){
	int start_from=22,i;
	for(i=0;i<COLUMNS_PIXELS-start_from-20;i++)
		disp_draw_pixel_with_char(0,start_from+i,BLACK_BG,' ');
	if(lives == 1){
		disp_draw_pixel_with_char(0,start_from,BROWN_ON_RED,'0');
		disp_draw_pixel_with_char(0,start_from+1,BROWN_ON_RED,'0');
		disp_draw_pixel_with_char(0,start_from+2,GREEN_ON_BLACK,'<');
	} else if(lives == 2) {
		disp_draw_pixel_with_char(0,start_from,BROWN_ON_RED,'0');
		disp_draw_pixel_with_char(0,start_from+1,BROWN_ON_RED,'0');
		disp_draw_pixel_with_char(0,start_from+2,GREEN_ON_BLACK,'<');
		
		disp_draw_pixel_with_char(0,start_from+4,BROWN_ON_RED,'0');
		disp_draw_pixel_with_char(0,start_from+5,BROWN_ON_RED,'0');
		disp_draw_pixel_with_char(0,start_from+6,GREEN_ON_BLACK,'<');
	} else if(lives == 3) {
		disp_draw_pixel_with_char(0,start_from,BROWN_ON_RED,'0');
		disp_draw_pixel_with_char(0,start_from+1,BROWN_ON_RED,'0');
		disp_draw_pixel_with_char(0,start_from+2,GREEN_ON_BLACK,'<');
		
		disp_draw_pixel_with_char(0,start_from+4,BROWN_ON_RED,'0');
		disp_draw_pixel_with_char(0,start_from+5,BROWN_ON_RED,'0');
		disp_draw_pixel_with_char(0,start_from+6,GREEN_ON_BLACK,'<');
		
		disp_draw_pixel_with_char(0,start_from+8,BROWN_ON_RED,'0');
		disp_draw_pixel_with_char(0,start_from+9,BROWN_ON_RED,'0');
		disp_draw_pixel_with_char(0,start_from+10,GREEN_ON_BLACK,'<');
	}
}

void disp_draw_score(int score){
	int i,str_len;
	char* ch_score;
	sprintf(ch_score,"%06d",score);
	str_len = (strlen(ch_score) > COLUMNS_PIXELS) ? COLUMNS_PIXELS : strlen(ch_score);
	for (i = 0; i < str_len && i < COLUMNS_PIXELS; i++)
		disp_draw_pixel_with_char(0, i, 2, ch_score[i]);
}

void disp_draw_map(){
	int i,j;
	for (i=0; i<ROWS; i++) {
		//printf("i:%d-",i);
		for(j=0;j<COLUMNS; j++) {
			//printf("j:%d",j);
			gameMap.refresh_map[i][j]=0;
			if 		(gameMap.level_map[i][j]==DIRT) upd_draw_dirt(i,j);
			else if (gameMap.level_map[i][j]==DIAMOND) upd_draw_diamond(i,j);
			else if (gameMap.level_map[i][j]==GOLD_BAG) {upd_draw_dirt(i,j);upd_draw_bag(i,j);}
			else if (gameMap.level_map[i][j]==EMPTY) {upd_draw_empty(i,j,1);} 
			//else if (gameMap.level_map[i][j]==DIGGER) {upd_draw_digger(player);} 
			else if (gameMap.level_map[i][j]==NOBBIN) {upd_draw_digger(player);} 
			disp_draw_cube(i,j);
		}
		//printf("|\n");
	}
	return;
}

void disp_draw_cube(int i,int j){
	int row_pixel = row_2_pixel(i), column_pixel = column_2_pixel(j), k, l;
	for (k=row_pixel;k<row_pixel+HEIGHT && k<ROWS_PIXELS;k++){
		for (l=column_pixel;l<column_pixel+WIDTH && l<COLUMNS_PIXELS;l++){
			//printf("(%d,%c)",gameMap.pixel_map[k][l][0],gameMap.pixel_map[k][l][1]);
			disp_draw_pixel_with_char(k,l,gameMap.pixel_map[k][l][1],gameMap.pixel_map[k][l][0]);
		}
		//printf("\n");
	}
}

void shake_bag(int y, int x,int pid_to_fall){
	int row_pixel = row_2_pixel(y), column_pixel,j = column_2_pixel(x),p, next=-1;
	int index[4];
	disp_draw_pixel_with_char(0,j+2,BLUE_BG,'$');
	gameMap.level_map[y][x] = MOVING_GOLD_BAG;
	index[0] = j-1; index[1] =  j ; index[2] = j+1 ; index[3] =  j;
	for (p=0;p<4*3;p++){
		column_pixel = index[(++next)%4];
		upd_draw_empty(y,x,0);
		gameMap.pixel_map[row_pixel][column_pixel + 2][0] = 'w';
		gameMap.pixel_map[row_pixel + 1][column_pixel + 1][0] = '/';
		gameMap.pixel_map[row_pixel + 1][column_pixel + 2][0] = ' ';
		gameMap.pixel_map[row_pixel + 1][column_pixel + 3][0] = '\\';
		gameMap.pixel_map[row_pixel + 2][column_pixel + 1][0] = ' ';
		gameMap.pixel_map[row_pixel + 2][column_pixel + 2][0] = '$';
		gameMap.pixel_map[row_pixel + 2][column_pixel + 3][0] = ' ';
		gameMap.pixel_map[row_pixel + 2][column_pixel + 1][1] = GRAY_BG;
		gameMap.pixel_map[row_pixel + 2][column_pixel + 2][1] = GRAY_BG;
		gameMap.pixel_map[row_pixel + 2][column_pixel + 3][1] = GRAY_BG;
		gameMap.pixel_map[row_pixel][column_pixel + 2][1] = GRAY_ON_BROWN;
		gameMap.pixel_map[row_pixel + 1][column_pixel + 1][1] = GRAY_ON_BROWN;
		gameMap.pixel_map[row_pixel + 1][column_pixel + 2][1] = GRAY_BG;
		gameMap.pixel_map[row_pixel + 1][column_pixel + 3][1] = GRAY_ON_BROWN;
		gameMap.refresh_map[y][x] = 1;
		sleept(SECONDT/10);
	}	
	gameMap.refresh_map[y][x] = 1;
	resume(pid_to_fall);
	disp_draw_pixel_with_char(0,j+2,GRAY_BG,' ');
	
}

void gold_falling(int i,int j){
	int x,y,counter,obj,gold_chunks,ps;	
	disp_draw_pixel_with_char(0,column_2_pixel(j)+2,GRAY_BG,'$');
	y=i;
	x=j;
	obj = get_object_in_direction(y,x,DOWN_ARROW);
	if(obj==EMPTY || obj==DIGGER || obj==NOBBIN || obj==HOBBIN){
		gameMap.level_map[i][j]=MOVING_GOLD_BAG;
		counter=0;
		
		while(obj==EMPTY || obj==DIGGER || obj==NOBBIN || obj==HOBBIN){	
			if(obj==DIGGER){
				player.is_alive=0;
				upd_draw_empty(y, x,1);
				sleept(SECONDT/2);
				y+=2;
				counter+=2;
				obj = get_object_in_direction(y,x,DOWN_ARROW);
				continue;
			} else if (obj==NOBBIN || obj==HOBBIN) {
				disable(ps);
				for(i=0;i<ENEMY_COUNT;i++)
					if(enemys[i].x==x && enemys[i].y==y+1){
						upd_draw_empty(y+1,x,1);
						send(score_lives_pid, DEAD_ENEMY_SCORE);
						kill_enemy(i);
						if(number_of_live_enemys() == 0 && all_enemys_created) next_level();
						break;
					}
				restore(ps);
			}
			counter++;
			sleept(0);
			upd_draw_bag_moving(y, x,DOWN_ARROW);
			upd_draw_empty(y, x,1);
			y++;
			obj = get_object_in_direction(y,x,DOWN_ARROW);
		}
		if(counter>1){
			gold_chunks=time_from_start%4+2;
			upd_draw_open_bag(y,x,gold_chunks);
		}
		else upd_draw_bag(y,x);
	}
	disp_draw_pixel_with_char(0,column_2_pixel(j)+2,BLACK_BG,' ');
}

int count_diamonds() {
	int i, j, ret=0;
	for (i=0; i<ROWS; i++) {
		for(j=0;j<COLUMNS; j++) {
			if (gameMap.level_map[i][j] == DIAMOND) ret++;
		}
	}
	return ret;
}

void create_map(int level_id) { //char level_map[ROWS][COLUMNS], int level_id) {
	int i, j;
	for (i=0; i<ROWS; i++) {
		for(j=0;j<COLUMNS; j++) {
			gameMap.level_map[i][j] = levels[level_id][i][j];
		}
	}
	gameMap.monster_start_amount = monster_count[level_id];
	gameMap.monster_max_amount = monster_max_count[level_id];
	gameMap.monster_become_angry_time = become_hobin[level_id];
	gameMap.monster_angry_for_time = hobin_time[level_id];
	gameMap.level_id = level_id;
	gameMap.digger_reload_time=digger_time[level_id];
	gameMap.digger_speed = digger_speed[level_id];
	gameMap.monster_speed = monster_speed[level_id];
	
}

void next_level() {
	gameMap.level_id++;
	
	if(gameMap.level_id < NUMBER_OF_LEVELS) {
		crazy_mode=0;
		if (all_enemys_created==0)kill(nobbin_creator_pid);
		kill_all_enemys();
		
		restart_digger((Digger*)(&player));
		
		setup_clean_screen();
		create_map(gameMap.level_id);
		
		disp_draw_map();
		disp_draw_lives(player.lives);
		disp_draw_score(player.score);
		
		resume(nobbin_creator_pid = create(nobbin_creator,INITSTK,INITPRIO,"nobbin_creator",0));
	}
	else { //the player finished all the levels- won the game! 
		//sprintf(debug_str, "You WON with score of %d!", player.score);
		//send(debug, debug_str);
		sleept(SECONDT*2);
		send(terminate_xinu_pid);
	}
}

void fireball_advance(int y, int x, int direction){
	int deltaX = 0, deltaY = 0, i,ps;
	
	if(direction == LEFT_ARROW)
		deltaX = -1;
	else if(direction == UP_ARROW)
		deltaY = -1;
	else if(direction == RIGHT_ARROW)
		deltaX = 1;
	else
		deltaY = 1;
	
	while(move_is_possible(y,x, direction, 0)){
		
		if(gameMap.level_map[y][x] != DIGGER)
			upd_draw_empty(y,x,1);
		x = x + deltaX;
		y = y + deltaY;
		upd_draw_fireball(y,x);
		sleept(SECONDT/9);
	}
	
	if(get_object_in_direction(y,x, direction) == NOBBIN || get_object_in_direction(y,x, direction) == HOBBIN){
		for(i = 0; i < ENEMY_COUNT; i++){
			if((enemys[i].y == y + deltaY) && (enemys[i].x == x + deltaX)){
					
					send(score_lives_pid, DEAD_ENEMY_SCORE);			
					send(sound_effects_pid,1);
					upd_draw_empty(y + deltaY, x + deltaX,1);
					upd_draw_cherry(enemys[i].y, enemys[i].x);
					disable(ps);
					kill_enemy(i);
					if(number_of_live_enemys() == 0 && all_enemys_created) next_level();
					restore(ps);
					break;
			}
		}
	}
	if(gameMap.level_map[y][x] != DIGGER)
		upd_draw_empty(y,x,1);
}


void refresh_debug_map(){
	while(1){
		draw_debug_line((char*)receive());
	}
}


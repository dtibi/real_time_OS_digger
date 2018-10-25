#include <conf.h>
#include "map.h"
#include "digger.h"
#include "nobin.h"
#include "myints.h"


char current_map[ROWS_PIXELS][COLUMNS_PIXELS][2];
volatile Digger player;
volatile Nobbin enemys[NOBBIN_COUNT];
char* debug_str;

void draw_debug_line(char *str){
	int i,str_len;
	str_len = (strlen(str)>COLUMNS_PIXELS) ? COLUMNS_PIXELS : strlen(str);
	//printf("draw_debug_line");
	for (i=0;i<str_len;i++){
		draw_pixel_with_char(0,i,2,str[i]);
	}
}

void clean_screen(){
	asm{
	MOV 		AH,0          // Select function = 'Set mode'
	MOV 		AL,3          // 80 by 25 color image
	INT 		10h           // Adapter initialized. Page 0 displayed	

	MOV             AX,0B800h
	MOV             ES,AX
	MOV             DI,0
	MOV             AL,' '
	MOV             AH,7
	MOV             CX,2000
	CLD
	REP             STOSW

	}
}
void draw_pixel_with_char(int row, int col, char color,char ch){
	int screen_address = start_address + row*10 , col_2 = col*2;
	char c = ch;
	asm{
		MOV             AX,screen_address
		MOV             ES,AX
		MOV             DI,col_2
		MOV             AL,c
		MOV             AH,color
		MOV             ES:[DI], AX
	}
	current_map[row][col][0] = ch;
	current_map[row][col][1] = color;
}

void draw_pixel(int row, int col, char color){
	int screen_address = start_address + row*10 , col_2 = col*2;
	
	asm{
		MOV             AX,screen_address
		MOV             ES,AX
		MOV             DI,col_2
		MOV             AL,' '
		MOV             AH,color
		MOV             ES:[DI], AX
	}
	
	current_map[row][col][0] = ' ';
	current_map[row][col][1] = color;
}

/*  draw_diamond(unsigned int i,unsigned int j)
	                       _____
 diamond will look like:  |_   _| and colored (GREEN)
                            |_|
*/
void draw_diamond(unsigned int i,unsigned int j) {
	int row_pixel = row_2_pixel(i), column_pixel = column_2_pixel(j);
	draw_dirt(i,j);
	draw_pixel_with_char(row_pixel+1,column_pixel+1,GREEN_BG, ' ');
	draw_pixel_with_char(row_pixel+1,column_pixel+2,GREEN_BG, ' ');
	draw_pixel_with_char(row_pixel+1,column_pixel+3,GREEN_BG, ' ');
	draw_pixel_with_char(row_pixel+2,column_pixel+2,GREEN_BG,' ');
}

/* draw_bag(unsigned int i,unsigned int j)
	                              w
Gold bags will look like:		 / \
						   		|_$_|
*/
void draw_bag(unsigned int i,unsigned int j){
	int row_pixel = row_2_pixel(i), column_pixel = column_2_pixel(j),k, l;
	draw_dirt(i,j);
	draw_pixel_with_char(row_pixel,column_pixel+2,GRAY_ON_BROWN,'w');
	draw_pixel_with_char(row_pixel+1,column_pixel+1,GRAY_ON_BROWN,'/');
	draw_pixel_with_char(row_pixel+1,column_pixel+2,GRAY_BG,' ');
	draw_pixel_with_char(row_pixel+1,column_pixel+3,GRAY_ON_BROWN,'\\');
	draw_pixel_with_char(row_pixel+2,column_pixel+1,GRAY_BG,' ');
	draw_pixel_with_char(row_pixel+2,column_pixel+2,GRAY_BG,'$');
	draw_pixel_with_char(row_pixel+2,column_pixel+3,GRAY_BG,' ');
	
}

void draw_dirt(unsigned int i,unsigned int j){
	int row_pixel = row_2_pixel(i), column_pixel = column_2_pixel(j), k, l;
	for (k=0;k<HEIGHT;k++)
		for (l=0;l<WIDTH;l++) {
			draw_pixel(row_pixel+k,column_pixel+l,BROWN_BG);
		}
}

void clean_nobbin(unsigned int i,unsigned int j){
	int row_pixel = row_2_pixel(i), column_pixel = column_2_pixel(j), k, l;
		for (k=0;k<HEIGHT;k++)
			for (l=0;l<WIDTH;l++) {
				draw_pixel(row_pixel+k,column_pixel+l,BROWN_BG);
			}
}

void draw_empty(unsigned int i,unsigned int j){
	int row = row_2_pixel(i), col= column_2_pixel(j),k,l;
	for (k=0;k<HEIGHT;k++)
		for (l=0;l<WIDTH;l++)
			draw_pixel(row+k,col+l,BLACK_BG);
}
/* 
nobbin should look like:
    _   _
   |_|_|_|
     |_|
	/   \

 */
void draw_nobbins(Nobbin n[NOBBIN_COUNT]){
	int i,j,x,y;
	for(i=0;i<NOBBIN_COUNT;i++){
		if(n[i].is_alive) {
			x = n[i].x;
			y = n[i].y;
			//printf("x = %d , y = %d",x,y);
			if(!n[i].is_hobbin){
				draw_dig(y,x);
				current_map[y][x][0] = ' ';
				current_map[y][x][1] =  GREEN_BG;
				current_map[y-1][x-1][0] = '0';
				current_map[y-1][x-1][1] =  BROWN_BG;
				current_map[y-1][x+1][0] = '0';
				current_map[y-1][x+1][1] =  BROWN_BG;
				current_map[y+1][x-1][0] = '/';
				current_map[y+1][x-1][1] =  RED;
				current_map[y+1][x+1][0] = '\\';
				current_map[y+1][x+1][1] =  RED;
			} else {
				
			}
			draw_area(y,x);
		}
	}
	
}

void draw_dig(unsigned int i,unsigned int j){
	int row = i-1, col= j-3,k,l;
	for (k=0;k<=HEIGHT;k++)
		for (l=0;l<=WIDTH;l++){
			current_map[row+k][col+l][0] = ' ';
			current_map[row+k][col+l][1] = BLACK_BG;
		}
}

void draw_area(int y, int x){
	int i,j;
	for(i=y-2;i>=0 && i<y+2 && i<ROWS_PIXELS;i++)
		for(j=x-4;j>=0 && j<x+4 && j<COLUMNS_PIXELS;j++)
			draw_pixel_with_char(i,j,current_map[i][j][1],current_map[i][j][0]);
}

void draw_digger(Digger player){
	int x=player.x,y=player.y,i,j;
	int direction = player.direction;
	
	switch (direction) {
		case RIGHT_ARROW:
			draw_dig(y,x);
			current_map[y][x][0] = '<';
			current_map[y][x-1][0] = 'o';
			current_map[y][x-2][0] = 'o';
			current_map[y][x][1] = GREEN_ON_BLACK;
			current_map[y][x-1][1] = BROWN_ON_RED;
			current_map[y][x-2][1] = BROWN_ON_RED;
			draw_area(y,x);
			break;
		case LEFT_ARROW:
			draw_dig(y,x);
			current_map[y][x][0] = '>';
			current_map[y][x+1][0] = 'o';
			current_map[y][x+2][0] = 'o';
			current_map[y][x][1] = GREEN_ON_BLACK;
			current_map[y][x+1][1] = BROWN_ON_RED;
			current_map[y][x+2][1] = BROWN_ON_RED;
			draw_area(y,x);
			break;
		case UP_ARROW:
			draw_dig(y,x);
			current_map[y][x][0] = 'V';
			current_map[y+1][x][0] = '8';
			current_map[y][x][1] = GREEN_ON_BLACK;
			current_map[y+1][x][1] = BROWN_ON_RED;
			draw_area(y,x);
			break;
		case DOWN_ARROW:
			draw_dig(y,x);
			current_map[y][x][0] = '^';
			current_map[y-1][x][0] = '8';
			current_map[y][x][1] = GREEN_ON_BLACK;
			current_map[y-1][x][1] = BROWN_ON_RED;
			draw_area(y,x);
			break;
	}
}

/* void draw_fire_ball(FireBall fb){
	
} */

int move_is_possible(int x,int y, int direction, int i_can_dig){
	if ((direction==UP_ARROW && (y-1)<=0) || 
		(direction==DOWN_ARROW  && (y+1)>=ROWS_PIXELS) || 
		(direction==RIGHT_ARROW && ((x+2)>=COLUMNS_PIXELS)) || 
		(direction==LEFT_ARROW && (x-2)<0)) return 0;
	if (direction!=UP_ARROW && direction!=DOWN_ARROW && direction!=RIGHT_ARROW && direction!=LEFT_ARROW) return 0;
	
	if		(direction==UP_ARROW    && (y-(HEIGHT/2)-1)<=0) return 0;
	else if (direction==DOWN_ARROW  && (y+(HEIGHT/2)+1)>=ROWS_PIXELS) return 0;
	else if (direction==RIGHT_ARROW && (x+(WIDTH/2)+1)>=COLUMNS_PIXELS) return 0;
	else if (direction==LEFT_ARROW  && (x-(WIDTH/2)-1)<=0) return 0;
	
	if(i_can_dig){
		return 1;
	} else {
		if(		 direction==UP_ARROW    && current_map[y-2][x][1]==BLACK_BG) return 1;
		else if (direction==DOWN_ARROW  && current_map[y+2][x][1]==BLACK_BG) return 1;
		else if (direction==RIGHT_ARROW && current_map[y][x+3][1]==BLACK_BG) return 1;
		else if (direction==LEFT_ARROW  && current_map[y][x-3][1]==BLACK_BG) return 1;
	return 0;
	}
}

int pixel_2_row( unsigned int pixel_index ) {
	if (pixel_index==0) return 0;
	return ((pixel_index-1)/HEIGHT);
}

int pixel_2_column(unsigned int pixel_index) {
	return (pixel_index/WIDTH);
}

// row of screen
int row_2_pixel( unsigned int row_index ) {
	return ((row_index*HEIGHT)+1);
}

// offset of screen to be used for register DI
int column_2_pixel(unsigned int column_index) {
	return (column_index*WIDTH);
}

//return the color of the pixel (BLACK_BG || GREEN_BG || BROWN_BG || )
int getNextPixelType(int x, int y, int direction, char level[ROWS][COLUMNS])
{	
	if		(direction==UP_ARROW   ) return level[(y-2) / (ROWS_PIXELS/ROWS)][x / (COLUMNS_PIXELS/COLUMNS)]; 
	else if (direction==DOWN_ARROW ) return level[y + 2 / (ROWS_PIXELS/ROWS)][x / (COLUMNS_PIXELS/COLUMNS)];
	else if (direction==RIGHT_ARROW) return level[y / (ROWS_PIXELS/ROWS)][(x+4) / (COLUMNS_PIXELS/COLUMNS)];
	else if (direction==LEFT_ARROW ) return level[y / (ROWS_PIXELS/ROWS)][(x-4) / (COLUMNS_PIXELS/COLUMNS)];
	return 0;
}

void create_map(){
	int i,j;
	
	for (i=0; i<ROWS; i++) {
		for(j=0;j<COLUMNS; j++) {
			if (level_0[i][j]==1) draw_dirt(i,j);
			else if(level_0[i][j]==2) draw_diamond(i,j);
			else if (level_0[i][j]==3) draw_bag(i,j);
			else if (level_0[i][j]==0) draw_empty(i,j);
		}
	}
}

void refresh_map(){
	create_map();
	//printf("x = %d , y = %d , dir = %c", (*player).x, (*player).y, (*player).direction);
	draw_digger(player);
	draw_nobbins(&enemys);
	while(1) {
		receive();
		draw_digger(player);
		draw_nobbins(&enemys);
	}
}

void refresh_debug_map(){
	while(1){
		draw_debug_line((char*)receive());
	}
}


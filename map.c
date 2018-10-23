#include <conf.h>
#include "map.h"
#include "digger.h"

char current_map[ROWS_PIXELS][COLUMNS_PIXELS][2];
char* str = "               ";

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

//						    ______
//diamond will look like:  |_    _| and colored (Tchelet)
//  	                     |__|
void draw_diamond(unsigned int i,unsigned int j) {
	int row_pixel = row_2_pixel(i), column_pixel = column_2_pixel(j);
	draw_dirt(i,j);
	draw_pixel(row_pixel+1,column_pixel+1,GREEN_BG);
	draw_pixel(row_pixel+1,column_pixel+2,GREEN_BG);
	draw_pixel(row_pixel+1,column_pixel+3,GREEN_BG);
	draw_pixel(row_pixel+1,column_pixel+4,GREEN_BG);
	draw_pixel(row_pixel+2,column_pixel+2,GREEN_BG);
	draw_pixel(row_pixel+2,column_pixel+3,GREEN_BG);
}

void draw_bag(unsigned int i,unsigned int j){
	int row_pixel = row_2_pixel(i), column_pixel = column_2_pixel(j),k, l;
	draw_dirt(i,j);
	for (k=0;k<HEIGHT-1;k++)
		for (l=1;l<WIDTH-1;l++)
			draw_pixel(row_pixel+k,column_pixel+l,GRAY_BG);
	draw_pixel_with_char(row_pixel,column_pixel+2,GRAY_BG,'$');
	draw_pixel_with_char(row_pixel,column_pixel+3,GRAY_BG,'$');
	draw_pixel_with_char(row_pixel+1,column_pixel+2,GRAY_BG,'$');
	draw_pixel_with_char(row_pixel+1,column_pixel+3,GRAY_BG,'$');
}

void draw_dirt(unsigned int i,unsigned int j){
	int row_pixel = row_2_pixel(i), column_pixel = column_2_pixel(j), k, l;
	for (k=0;k<HEIGHT;k++)
		for (l=0;l<WIDTH;l++)
			draw_pixel(row_pixel+k,column_pixel+l,BROWN_BG);
}

void draw_empty(unsigned int i,unsigned int j){
	int row = row_2_pixel(i), col= column_2_pixel(j),k,l;
	for (k=0;k<HEIGHT;k++)
		for (l=0;l<WIDTH;l++)
			draw_pixel(row+k,col+l,BLACK_BG);
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
	for(i=y-2;i>=0 && i<y+2;i++)
		for(j=x-4;j>=0 && j<x+4;j++)
			draw_pixel_with_char(i,j,current_map[i][j][1],current_map[i][j][0]);
}

void draw_digger(Digger player){
	int x=player.x,y=player.y,i,j;
	char direction = player.direction;
	sprintf(str,"x = %d , y = %d " , x , y);
	switch (direction) {
		case 'r':
			draw_dig(y,x);
			current_map[y][x][0] = '<';
			current_map[y][x-1][0] = 'o';
			current_map[y][x-2][0] = 'o';
			current_map[y][x][1] = GREEN_ON_BLACK;
			current_map[y][x-1][1] = BROWN_ON_RED;
			current_map[y][x-2][1] = BROWN_ON_RED;
			draw_area(y,x);
			break;
		case 'l':
			draw_dig(y,x);
			current_map[y][x][0] = '>';
			current_map[y][x+1][0] = 'o';
			current_map[y][x+2][0] = 'o';
			current_map[y][x][1] = GREEN_ON_BLACK;
			current_map[y][x+1][1] = BROWN_ON_RED;
			current_map[y][x+2][1] = BROWN_ON_RED;
			draw_area(y,x);
			break;
		case 'u':
			draw_dig(y,x);
			current_map[y][x][0] = 'V';
			current_map[y+1][x][0] = '8';
			current_map[y][x][1] = GREEN_ON_BLACK;
			current_map[y+1][x][1] = BROWN_ON_RED;
			draw_area(y,x);
			break;
		case 'd':
			draw_dig(y,x);
			current_map[y][x][0] = '^';
			current_map[y-1][x][0] = '8';
			current_map[y][x][1] = GREEN_ON_BLACK;
			current_map[y-1][x][1] = BROWN_ON_RED;
			draw_area(y,x);
			break;
	}
}

int pixel_2_row( unsigned int pixel_index ) {
	return ((pixel_index+1)/3);
}

int pixel_2_column(unsigned int pixel_index) {
	return ((pixel_index+1)/6);
}

// row of screen
int row_2_pixel( unsigned int row_index ) {
	return ((row_index*3)+1);
}

// offset of screen to be used for register DI
int column_2_pixel(unsigned int column_index) {
	return ((column_index*6)+1);
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

void refresh_map(Digger *player){
	create_map();
	//printf("x = %d , y = %d , dir = %c", (*player).x, (*player).y, (*player).direction);
	draw_digger(*player);
	while(1) {
		draw_digger(*player);
		move_digger(player);
	}
}


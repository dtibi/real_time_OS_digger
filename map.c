#include <conf.h>
#include "map.h"

char pixel_lvl_0[25][80];
char **level_pixels;

void clean_screen(){
	asm{
	MOV 		AH,0          // Select function = 'Set mode'
	MOV 		AL,3          // 80 by 25 color image
	INT 		10h           // Adapter initialized. Page 0 displayed	

	MOV             AX,0B800h
	MOV             ES,AX
	MOV             DI,0
	MOV             AL,' '
	MOV             AH,BLACK_BG
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
}

/*  draw_diamond(unsigned int i,unsigned int j)
	                       ______
 diamond will look like:  |_    _| and colored (GREEN)
                            |__|
*/
void draw_diamond(unsigned int i,unsigned int j) {
	int row_pixel = row_2_pixel(i), column_pixel = column_2_pixel(j);
	draw_dirt(i,j);
	draw_pixel_with_char(row_pixel,column_pixel+1,BROWN_BG, '_');
	draw_pixel_with_char(row_pixel,column_pixel+2,BROWN_BG, '/');
	draw_pixel_with_char(row_pixel,column_pixel+3,BROWN_BG, '\\');
	draw_pixel_with_char(row_pixel,column_pixel+4,BROWN_BG, '_');
	draw_pixel_with_char(row_pixel+1,column_pixel+1,GREEN_BG,'|');
	draw_pixel(row_pixel+1,column_pixel+2,GREEN_BG);
	draw_pixel(row_pixel+1,column_pixel+3,GREEN_BG);
	draw_pixel_with_char(row_pixel+1,column_pixel+4,GREEN_BG,'|');
	draw_pixel_with_char(row_pixel+2,column_pixel+2,GREEN_BG,'\\');
	draw_pixel_with_char(row_pixel+2,column_pixel+3,GREEN_BG,'/');
	level_pixels[row_pixel+1][column_pixel+1]=2;
	level_pixels[row_pixel+1][column_pixel+2]=2;
	level_pixels[row_pixel+1][column_pixel+3]=2;
	level_pixels[row_pixel+1][column_pixel+4]=2;
	level_pixels[row_pixel+2][column_pixel+2]=2;
	level_pixels[row_pixel+2][column_pixel+3]=2;
}

/* draw_bag(unsigned int i,unsigned int j)
	                             _,_,_,_
Gold bags will look like:		|__$$__|
						   		|__$$__|
*/
void draw_bag(unsigned int i,unsigned int j){
	int row_pixel = row_2_pixel(i), column_pixel = column_2_pixel(j),k, l;
	draw_dirt(i,j);
	for (k=0;k<HEIGHT-1;k++)
		for (l=1;l<WIDTH-1;l++){
			draw_pixel(row_pixel+k,column_pixel+l,GRAY_BG);
			level_pixels[row_pixel+k][column_pixel+l]=3;
		}
	draw_pixel_with_char(row_pixel,column_pixel+2,GRAY_BG,'$');
	draw_pixel_with_char(row_pixel,column_pixel+3,GRAY_BG,'$');
	draw_pixel_with_char(row_pixel+1,column_pixel+2,GRAY_BG,'$');
	draw_pixel_with_char(row_pixel+1,column_pixel+3,GRAY_BG,'$');
}

void draw_dirt(unsigned int i,unsigned int j){
	int row_pixel = row_2_pixel(i), column_pixel = column_2_pixel(j), k, l;
	for (k=0;k<HEIGHT;k++)
		for (l=0;l<WIDTH;l++) {
			draw_pixel(row_pixel+k,column_pixel+l,BROWN_BG);
			level_pixels[row_pixel+k][column_pixel+l]=1;
		}
}

void draw_empty(unsigned int i,unsigned int j){
	int row_pixel = row_2_pixel(i), column_pixel = column_2_pixel(j), k, l;
	for (k=0;k<HEIGHT;k++)
		for (l=0;l<WIDTH;l++){
			draw_pixel(row_pixel+k,column_pixel+l,BLACK_BG);
			level_pixels[row_pixel+k][column_pixel+l]=0;
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

void create_map(int pixel_lvl[ROWS_PIXELS][COLUMNS_PIXELS]){
	int i,j;
	level_pixels = (char**)malloc(sizeof(char*)*ROWS);
	for (i=1; i<ROWS; i++) level_pixels[i] = pixel_lvl[i];
	for (i=1; i<ROWS_PIXELS; i++) draw_pixel(i,0,BROWN_BG);
	for (i=1; i<ROWS_PIXELS; i++) draw_pixel(i,79,BROWN_BG);
	for (i=0; i<ROWS; i++) {
		for(j=0;j<COLUMNS; j++) {
			if (level_0[i][j]==1) draw_dirt(i,j);
			else if(level_0[i][j]==2) draw_diamond(i,j);
			else if (level_0[i][j]==3) draw_bag(i,j);
			else if (level_0[i][j]==0) draw_empty(i,j);
		}
	}
}


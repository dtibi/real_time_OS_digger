#include <conf.h>
#include "map.h"
#include "digger.h"

char dirt_map[ROWS_PIXELS][COLUMNS_PIXELS];
char map[ROWS][COLUMNS];
char level_0[ROWS][COLUMNS] = {
	0 , 1 , 3 , 1 , 3 , 1 , 1 , 1 , 1 , 1 , 0 , 0 , 0 ,
    0 , 2 , 2 , 1 , 1 , 1 , 1 , 1 , 1 , 1 , 0 , 3 , 1 ,
	0 , 2 , 2 , 1 , 1 , 2 , 3 , 1 , 1 , 1 , 0 , 1 , 1 ,
	0 , 2 , 2 , 3 , 1 , 2 , 1 , 1 , 1 , 1 , 0 , 2 , 2 ,
	0 , 1 , 1 , 1 , 1 , 2 , 1 , 1 , 3 , 1 , 0 , 2 , 2 ,
	0 , 0 , 0 , 0 , 1 , 2 , 1 , 1 , 1 , 1 , 0 , 1 , 1 ,
	2 , 1 , 1 , 0 , 0 , 1 , 1 , 1 , 1 , 1 , 0 , 1 , 2 ,
	2 , 2 , 1 , 1 , 0 , 0 , 0 , 0 , 0 , 0 , 0 , 2 , 2 
};

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
		}
}

void draw_empty(unsigned int i,unsigned int j){
	int row_pixel = row_2_pixel(i), column_pixel = column_2_pixel(j), k, l;
	for (k=0;k<HEIGHT;k++)
		for (l=0;l<WIDTH;l++){
			draw_pixel(row_pixel+k,column_pixel+l,BLACK_BG);
			dirt_map[row_pixel+k][column_pixel+l]=0;
		}
}


void draw_digger(Digger d){
	int i=d.y,j=d.x;
	switch(d.direction) {
		case 'u':
			draw_pixel_with_char(i,j,GREEN_ON_BLACK,'V');
			draw_pixel_with_char(i+1,j,BROWN_ON_RED,'8');
			break;
		case 'd':
			draw_pixel_with_char(i,j,GREEN_ON_BLACK,'n');
			draw_pixel_with_char(i-1,j,BROWN_ON_RED,'8');
			break;
		case 'r':
			draw_pixel_with_char(i,j,GREEN_ON_BLACK,'<');
			draw_pixel_with_char(i,j-1,BROWN_ON_RED,'o');
			draw_pixel_with_char(i,j-2,BROWN_ON_RED,'o');
			break;
		case 'l':
			draw_pixel_with_char(i,j,GREEN_ON_BLACK,'>');
			draw_pixel_with_char(i,j+1,BROWN_ON_RED,'o');
			draw_pixel_with_char(i,j+2,BROWN_ON_RED,'o');
	}
	//printf("x = %d , y = %d",d.x,d.y);
}


void draw_map(){
	int i,j;
	for (i=0; i<ROWS; i++) {
		for(j=0;j<COLUMNS; j++) {
			if(map[i][j]==2) draw_diamond(i,j);
			else if (map[i][j]==3) draw_bag(i,j);
		}
	}
	
	for (i=1; i<ROWS_PIXELS; i++) 
		for(j=0;j<COLUMNS_PIXELS; j++) 
			if (dirt_map[i][j]==1) 
				draw_pixel(i,j,BROWN_BG);
			else 
				draw_pixel(i,j,BLACK_BG);
}
/* void draw_nobbin(Nobbin n){
	int i=n.x,j=n.y;
	
	draw_pixel_with_char(i,j,RED_ON_BLACK,'<');
	draw_pixel_with_char(i,j-1,RED_BG,'o');
	draw_pixel_with_char(i,j-2,BROWN_BG,'E');
	
} */


/* void draw_fire_ball(FireBall fb){
	
} */

int move_is_possible(int x,int y, char direction, int i_can_dig){
	switch(direction) {
		case 'u':
			if ((y-3)<0) return 0;
			break;
		case 'd':
			if ((y+2)>ROWS_PIXELS) return 0;
			break;
		case 'r':
			if ((x+4)>COLUMNS_PIXELS) return 0;
			break;
		case 'l':
			if ((x-4)<0) return 0;
			break;
	}
	if(i_can_dig){
		return 1;
	} else {
		switch(direction) {
			case 'u':
				if (dirt_map[y-2][x]==0) return 1;
				break;
			case 'd':
				if (dirt_map[y+2][x]==0) return 1;
				break;
			case 'r':
				if (dirt_map[y][x+4]==0) return 1;
				break;
			case 'l':
				if (dirt_map[y][x-4]==0) return 1;
				break;
		}
	}
	return 0;
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

void create_map(Digger player){
	int i,j;
	
	//update dirt_map for movement control
	for(j=0;j<COLUMNS_PIXELS; j++) dirt_map[0][j]=0;
	for (i=1; i<ROWS_PIXELS; i++) 
		for(j=0;j<COLUMNS_PIXELS; j++) dirt_map[i][j]=1;
	
	//padding first and last column
	for (i=1; i<ROWS_PIXELS; i++) draw_pixel(i,0,BROWN_BG);
	for (i=1; i<ROWS_PIXELS; i++) draw_pixel(i,79,BROWN_BG);
	
	//initial drawing of map
	for (i=0; i<ROWS; i++) {
		for(j=0;j<COLUMNS; j++) {
			map[i][j]=level_0[i][j];
			printf("%d",map[i][j]);
			if (level_0[i][j]==1) draw_dirt(i,j);
			else if(level_0[i][j]==2) draw_diamond(i,j);
			else if (level_0[i][j]==3) draw_bag(i,j);
			else if (level_0[i][j]==0) draw_empty(i,j);
		}
		printf("\n");
	}
	
	draw_digger(player);
}




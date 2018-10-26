#include <conf.h>
#include "map.h"
#include "digger.h"
#include "nobin.h"
#include "myints.h"

volatile Digger player;
volatile Map gameMap;
volatile Nobbin enemys[NOBBIN_COUNT];
char* debug_str;

void draw_debug_line(char *str){
	int i,str_len;
	str_len = (strlen(str)>COLUMNS_PIXELS) ? COLUMNS_PIXELS : strlen(str);
	for (i=0;i<str_len;i++){
		draw_pixel_with_char(0,i,2,str[i]);
	}
	for(i=i;i<COLUMNS_PIXELS;i++){
		draw_pixel_with_char(0,i,2,' ');
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
	if (screen_address < start_address || screen_address > start_address+999) {
		printf("illegal address trying to wrote to screen: %x", screen_address);
		return;
	}
	if (col_2 < 0 || col_2 > 160) {
		printf("illegal offset trying to wrote to screen: %d", col_2);
		return;
	}
	
	asm{
		PUSH AX
		PUSH ES
		PUSH DI
		MOV             AX,screen_address
		MOV             ES,AX
		MOV             DI,col_2
		MOV             AL,c
		MOV             AH,color
		MOV             ES:[DI], AX
		POP DI
		POP ES
		POP AX
	}
	gameMap.current_map[row][col][0] = ch;
	gameMap.current_map[row][col][1] = color;
}

void draw_pixel(int row, int col, char color){
	int screen_address = start_address + row*10 , col_2 = col*2;
	if (screen_address < start_address || screen_address > start_address+250) {
		printf("illegal address trying to wrote to screen: %x", screen_address);
		return;
	}
	asm{
		PUSH AX
		PUSH ES
		PUSH DI
		MOV             AX,screen_address
		MOV             ES,AX
		MOV             DI,col_2
		MOV             AL,' '
		MOV             AH,color
		MOV             ES:[DI], AX
		POP DI
		POP ES
		POP AX
	}
	

	gameMap.current_map[row][col][0] = ' ';
	gameMap.current_map[row][col][1] = color;
}

/*  draw_diamond(unsigned int i,unsigned int j)
	                       _____
 diamond will look like:  |_   _| and colored (GREEN)
                            |_|
*/
void draw_diamond(unsigned int i,unsigned int j) {
	int row_pixel = row_2_pixel(i), column_pixel = column_2_pixel(j);
	draw_dirt(i,j);
	gameMap.currentLevel[i][j]=2;
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
	gameMap.currentLevel[i][j]=3;
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
	gameMap.currentLevel[i][j]=1;
	for (k=0;k<HEIGHT;k++)
		for (l=0;l<WIDTH;l++) {
			draw_pixel(row_pixel+k,column_pixel+l,BROWN_BG);
		}
}

void draw_empty(unsigned int i,unsigned int j){
	int row_pixel = row_2_pixel(i), column_pixel = column_2_pixel(j), k, l;
	gameMap.currentLevel[i][j]=0;
	for (k=0;k<HEIGHT;k++){
		for (l=0;l<WIDTH;l++)
			draw_pixel(row_pixel+k,column_pixel+l,BLACK_BG);
	}
}
/* 
nobbin should look like:
    _   _
   |_|_|_|
     |_|
	/   \

 */
void draw_nobbin(Nobbin n){
	int i,j,x=n.x,y=n.y;
	if(n.is_alive) {
		gameMap.currentLevel[pixel_2_row(y)][pixel_2_column(x)]=88;
		if(!n.is_hobbin){
			draw_dig(y,x);
			gameMap.current_map[y][x][0] = ' ';
			gameMap.current_map[y][x][1] =  GREEN_BG;
			gameMap.current_map[y-1][x-1][0] = '0';
			gameMap.current_map[y-1][x-1][1] =  BROWN_BG;
			gameMap.current_map[y-1][x+1][0] = '0';
			gameMap.current_map[y-1][x+1][1] =  BROWN_BG;
			gameMap.current_map[y+1][x-1][0] = '/';
			gameMap.current_map[y+1][x-1][1] =  RED;
			gameMap.current_map[y+1][x+1][0] = '\\';
			gameMap.current_map[y+1][x+1][1] =  RED;
		} else {
			
		}
		draw_area(y,x);
	}
}

void draw_dig(unsigned int i,unsigned int j){
	int row = i-(HEIGHT/2), col= j-(WIDTH/2),k,l;
	
	gameMap.currentLevel[pixel_2_row(i)][pixel_2_column(j)] = 0;
	
	for (k=row - HEIGHT;k<row+(3*HEIGHT) && k<ROWS_PIXELS ; k=k+HEIGHT){
		if(k<=0)continue;
		for (l=col - WIDTH;l<col+(3*WIDTH) && l<COLUMNS_PIXELS;l=l+WIDTH) {
			if(l<0) continue;
			if (gameMap.currentLevel[pixel_2_row(k)][pixel_2_column(l)]==1) draw_dirt(pixel_2_row(k),pixel_2_column(l));
			else if(gameMap.currentLevel[pixel_2_row(k)][pixel_2_column(l)]==2) draw_diamond(pixel_2_row(k),pixel_2_column(l));
			else if (gameMap.currentLevel[pixel_2_row(k)][pixel_2_column(l)]==3) draw_bag(pixel_2_row(k),pixel_2_column(l));
			else if (gameMap.currentLevel[pixel_2_row(k)][pixel_2_column(l)]==0) draw_empty(pixel_2_row(k),pixel_2_column(l));
			else if (gameMap.currentLevel[pixel_2_row(k)][pixel_2_column(l)]==99) draw_digger(player);
			else if (gameMap.currentLevel[pixel_2_row(k)][pixel_2_column(l)]==88) ;//draw_nobbin(enemys);
		}
	}
}

void draw_area(int y, int x){
	int i,j;
	for(i=y-HEIGHT;i<y+HEIGHT && i<ROWS_PIXELS;i++){
		if(i<=0) continue;
		for(j=x-WIDTH;j<x+WIDTH && j<COLUMNS_PIXELS;j++){
			if(j<0) continue;
			draw_pixel_with_char(i,j,gameMap.current_map[i][j][1],gameMap.current_map[i][j][0]);
		}
	}
}

void draw_digger(Digger player){
	int x=player.x,y=player.y;
	int direction = player.direction;
	gameMap.currentLevel[pixel_2_row(y)][pixel_2_column(x)] = 99;
	switch (direction) {
		case RIGHT_ARROW:
			draw_dig(y,x);
			gameMap.current_map[y][x+2][0] = '<';
			gameMap.current_map[y][x+1][0] = 'o';
			gameMap.current_map[y][x][0] = 'o';
			gameMap.current_map[y][x+2][1] = GREEN_ON_BLACK;
			gameMap.current_map[y][x+1][1] = BROWN_ON_RED;
			gameMap.current_map[y][x][1] = BROWN_ON_RED;
			draw_area(y,x);
			break;
		case LEFT_ARROW:
			draw_dig(y,x);
			gameMap.current_map[y][x-2][0] = '>';
			gameMap.current_map[y][x-1][0] = 'o';
			gameMap.current_map[y][x][0] = 'o';
			gameMap.current_map[y][x-2][1] = GREEN_ON_BLACK;
			gameMap.current_map[y][x-1][1] = BROWN_ON_RED;
			gameMap.current_map[y][x][1] = BROWN_ON_RED;
			draw_area(y,x);
			break;
		case UP_ARROW:
			draw_dig(y,x);
			gameMap.current_map[y-1][x][0] = 'V';
			gameMap.current_map[y][x][0] = '8';
			gameMap.current_map[y-1][x][1] = GREEN_ON_BLACK;
			gameMap.current_map[y][x][1] = BROWN_ON_RED;
			draw_area(y,x);
			break;
		case DOWN_ARROW:
			draw_dig(y,x);
			gameMap.current_map[y+1][x][0] = '^';
			gameMap.current_map[y][x][0] = '8';
			gameMap.current_map[y+1][x][1] = GREEN_ON_BLACK;
			gameMap.current_map[y][x][1] = BROWN_ON_RED;
			draw_area(y,x);
			break;
	}
}

/* void draw_fire_ball(FireBall fb){
	
} */

int move_is_possible(int x,int y, int direction, int i_can_dig){
	//sprintf(debug_str,"move_is_possible(x=%d,y=%d,direction=%d,can_dog=%d",x,y,direction,i_can_dig);
	//send(debug,debug_str);
	
	if((direction==UP_ARROW    && (y/HEIGHT)-1 < 0	  	 ) ||
	   (direction==DOWN_ARROW  && (y/HEIGHT)+2 > ROWS	 ) ||
	   (direction==RIGHT_ARROW && (x/WIDTH)+2  > COLUMNS ) ||
	   (direction==LEFT_ARROW  && (x/WIDTH)-1  < 0		 ) ) return 0;
	
	if (direction!=UP_ARROW && direction!=DOWN_ARROW && direction!=RIGHT_ARROW && direction!=LEFT_ARROW) return 0;
	
	if(i_can_dig)
		return 1;
	
	else{
		if		(direction==UP_ARROW    && gameMap.currentLevel[pixel_2_row(y)-1][pixel_2_column(x)]==0) return 1;
		else if (direction==DOWN_ARROW  && gameMap.currentLevel[pixel_2_row(y)+1][pixel_2_column(x)]==0) return 1;
		else if (direction==RIGHT_ARROW && gameMap.currentLevel[pixel_2_row(y)][pixel_2_column(x)+1]==0) return 1;
		else if (direction==LEFT_ARROW  && gameMap.currentLevel[pixel_2_row(y)][pixel_2_column(x)-1]==0) return 1;
	}
	
	return 0;
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
int getNextPixelType(int x, int y, int direction)
{	
	if		(direction==UP_ARROW    && (pixel_2_row(y)-1) >= 0) 		return gameMap.currentLevel[pixel_2_row(y)-1][pixel_2_column(x)]; 
	else if (direction==DOWN_ARROW  && (pixel_2_row(y)+1) < ROWS) 		return gameMap.currentLevel[pixel_2_row(y)+1][pixel_2_column(x)];
	else if (direction==RIGHT_ARROW && (pixel_2_column(x)+1) < COLUMNS) return gameMap.currentLevel[pixel_2_row(y)][pixel_2_column(x)+1];
	else if (direction==LEFT_ARROW  && (pixel_2_column(x)-1) < COLUMNS) 	return gameMap.currentLevel[pixel_2_row(y)][pixel_2_column(x)-1];
	return -1;
}

Map create_map(){
	int i,j;
	for (i=0; i<ROWS; i++) {
		for(j=0;j<COLUMNS; j++) {
			if (gameMap.currentLevel[i][j]==1) draw_dirt(i,j);
			else if(gameMap.currentLevel[i][j]==2) draw_diamond(i,j);
			else if (gameMap.currentLevel[i][j]==3) {draw_dirt(i,j);draw_bag(i,j);}
			else if (gameMap.currentLevel[i][j]==0) draw_empty(i,j);
		}
	}
	
	return gameMap;
}

void refresh_debug_map(){
	while(1){
		draw_debug_line((char*)receive());
	}
}


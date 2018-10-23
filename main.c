#include <conf.h>
#include <kernel.h>
#include "map.h"
#include "digger.h"
#include "myints.h"

Digger player;
int need_refresh = 0;

void refresh_map();
void move_digger(Digger *player);


/*------------------------------------------------------------------------
 *----------------------   Main Digger Program ---------------------------
 *------------------------------------------------------------------------
 */
xmain()
{
	
	
	//void interrupt (*Int9Save) (void);
	
	//Int9Save = getvect(0x09);
	setvect(9, MyISR9);

	create_digger(&player);
	
	resume (create(move_digger,INITSTK,INITPRIO,"cons",0));
	resume (create(refresh_map,INITSTK,INITPRIO,"dons",0));
	
	//background_music();
	
	//move(&player);
	
	//sleep(10);
	//setvect(9, Int9Save);
	//xdone();
	return;
}

void refresh_map() {
	create_map(player);
	while(1){
		while(!need_refresh);
		draw_map();
	}
}

void move_digger(Digger *player)
{
	while(1){
		switch (scan)
		{
			case 80: //down
				if((*player).direction != 'd')
					(*player).direction = 'd';
				else if((*player).y != ROWS)
					(*player).y--;
				break;
			
			case 72: //up
				if((*player).direction != 'u')
					(*player).direction = 'u';
				if((*player).y != 0)
					(*player).y++;
				break;
			
			case 75: //left
				if((*player).direction != 'l')
					(*player).direction = 'l';
				if((*player).x != 0)
					(*player).x--;
				break;
			
			case 77: //right
				if((*player).direction != 'r')
					(*player).direction = 'r';
				if((*player).x != COLUMNS)
					(*player).x++;
				break;
		}
	}
}
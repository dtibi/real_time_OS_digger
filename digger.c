#include <stdio.h>
#include <dos.h>
#include "digger.h"
#include "map.h"

void create_digger(Digger *player)
{
	*player.x = 40;
	*player.y = 24;
}

void move(Digger *player)
{
	while(scan != 1)
	{
		switch (scan)
		{
			case 80: //down
				if(*player.y != ROWS)
					*player.y--;
				break;
			
			case 72: //up
				if(*player.y != 0)
					*player.y++;
				break;
			
			case 75: //left
				if(*player.x != 0)
					*player.x--;
				break;
			
			case 77: //right
				if(*player.x != COLUMNS)
					*player.x++;
				break;
			
			case 57: //space
				printf("space\n");
				break;
		}
	}
}
#include <conf.h>
#include <kernel.h>
#include "map.h"
#include "digger.h"
#include "myints.h"

Digger player;
int need_refresh = 0;

void refresh_map();
void move_digger(Digger *player);


Digger player;

/*------------------------------------------------------------------------
 *----------------------   Main Digger Program ---------------------------
 *------------------------------------------------------------------------
 */
xmain()
{
	create_digger(&player);
	Int9Save = getvect(0x09);
	Int8Save = getvect(0x08);

	setvect(9, MyISR9);
	//setvect(8, MyISR8);

	resume(create(refresh_map,INITSTK,INITPRIO,"refresh_map",1,&player));
	//resume(create(refresh_debug,INITSTK,INITPRIO,"refresh_debug",1,&player));
	while(1);
	
	//sleep(10);
	//setvect(9, Int9Save);
	//xdone();
	return;
}

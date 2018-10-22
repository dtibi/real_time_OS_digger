/* ex1.c - xmain */

#include <conf.h>
#include "map.h"
#include "sound.h"
#include "digger.h"

void interrupt (*Int9Save) (void);

/*------------------------------------------------------------------------
 *----------------------   Main Digger Program ---------------------------
 *------------------------------------------------------------------------
 */
xmain()
{
	Digger player;
	Int9Save = getvect(0x09);
	setvect(9, MyISR9);

	create_digger(&player);
	create_map();
	//background_music();
	
	move(&player);
	
	sleep(10);
	
	xdone();
	return;
}


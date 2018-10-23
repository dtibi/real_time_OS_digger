#include <conf.h>
#include <kernel.h>
#include "myints.h"
#include "map.h"
#include "sound.h"
#include "digger.h"

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
	
	xdone();
	return;
}

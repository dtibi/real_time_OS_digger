#include <conf.h>
#include "myints.h"
#include "map.h"
#include "sound.h"
#include "digger.h"

/*------------------------------------------------------------------------
 *----------------------   Main Digger Program ---------------------------
 *------------------------------------------------------------------------
 */
xmain()
{
	Digger player;
	
	void interrupt (*Int9Save) (void);
	
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


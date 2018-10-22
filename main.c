#include <conf.h>
#include "map.h"
#include "sound.h"
#include "digger.h"

void interrupt (*Int9Save) (void);
volatile unsigned char scan;

void interrupt KeyBoard_Handler(void)
{	

	asm{
		//PUSHF
		//CALL DWORD PTR Int9Save
		
		in al, 60h    //input
		mov scan, al  // scan code
		
		in al, 61h
		or al, 80h
		out 61h, al
		and al, 7Fh
		out 61h, al
		mov al, 20h //end int
		out 20h, al
	}
}

/*------------------------------------------------------------------------
 *----------------------   Main Digger Program ---------------------------
 *------------------------------------------------------------------------
 */
xmain()
{
	Digger player;
	Int9Save = getvect(0x09);
	setvect(9, KeyBoard_Handler);

	create_digger(&player);
	move(&player);
	
	create_map();
	//background_music();
	sleep(10);
	
	xdone();
	return;
}


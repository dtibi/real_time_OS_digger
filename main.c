#include <conf.h>
#include <kernel.h>
#include "map.h"
#include "digger.h"
#include "myints.h"
#include "nobin.h"
#include "sound.c"

int sched_arr_pid[5] = {-1};
int sched_arr_int[5] = {-1};
int point_in_cycle;
int gcycle_length;
int gno_of_pids;
extern SYSCALL  sleept(int);
extern struct intmap far *sys_imp;

SYSCALL schedule(int no_of_pids, int cycle_length, int pid1, ...)
{
  int i;
  int ps;
  int *iptr;

  disable(ps);

  gcycle_length = cycle_length;
  point_in_cycle = 0;
  gno_of_pids = no_of_pids;

  iptr = &pid1;
  for(i=0; i < no_of_pids; i++)
  {
    sched_arr_pid[i] = *iptr;
    iptr++;
    sched_arr_int[i] = *iptr;
    iptr++;
  } // for
  restore(ps);
  return(OK);

} // schedule 


void kill_xinu(){
	int sec = 1,i;
	receive();
	for(i=0; i < 4; i++){
		kill(sched_arr_pid[i]);
	} 
	restore_ints();
	no_sound();
	clean_screen();
	printf("terminating program in %d second/s", sec);
	//sleep(sec);
	clean_screen();
	
	xdone();
}

/*------------------------------------------------------------------------
 *----------------------   Main Digger Program ---------------------------
 *------------------------------------------------------------------------
 */
xmain()
{
	int i, j;
	player = create_digger();
	for(i=0;i<NOBBIN_COUNT;i++)
		enemys[i] = create_nobbin(&player);
	enemys[0].is_alive = 1;
	draw_nobbin(enemys[0]);
	for (i=0; i<ROWS; i++) {
		for(j=0;j<COLUMNS; j++) {
			gameMap.currentLevel[i][j] = level_0[i][j];
		}
	}
	gameMap = create_map();
	draw_digger(player);
	

	//map_moves_pid = create(refresh_map,INITSTK,INITPRIO,"refresh_map",0);
	digger_move_pid = create(move_digger,INITSTK,INITPRIO+2,"move_digger",1,&player);
	debug = create(refresh_debug_map,INITSTK,INITPRIO,"refresh_debug_map",0);
	terminate_xinu_pid = create(kill_xinu,INITSTK,INITPRIO+3,"kill_Xinu",0);
	move_enemys_pid = create(move_nobbins,INITSTK,INITPRIO+1,"move_nobbins",0);
	bg_sound_pid = create(background_music,INITSTK,INITPRIO-3,"background_sounds",0);


	//resume(map_moves_pid);
	resume(digger_move_pid);
	resume(debug);
	resume(terminate_xinu_pid);
	resume(move_enemys_pid);
	resume(bg_sound_pid);

	setup_interrupts();
	
	
	
    schedule(3,57, map_moves_pid, 0,  debug, 29,move_enemys_pid, 29, digger_move_pid, 29, bg_sound_pid, 0);
	
	return(OK);

} 




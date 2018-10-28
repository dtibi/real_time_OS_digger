#include "map.c"
#include "digger.c"
#include "nobin.c"
#include "sound.c"
#include "myints.c"

int sched_arr_pid[10] = {-1};
int sched_arr_int[10] = {-1};
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
	setup_clean_screen();
	printf("terminating program in %d second/s", sec);
	//sleep(sec);
	setup_clean_screen();
	
	xdone();
	return;
}

/*------------------------------------------------------------------------
 *----------------------   Main Digger Program ---------------------------
 *------------------------------------------------------------------------
 */
xmain()
{
	int i, j;
	player = create_digger();
	for(i=0;i<NOBBIN_COUNT;i++){
		enemys[i] = create_nobbin(&player);
	}
	enemys[0].is_alive = 1; 
	
	for (i=0; i<ROWS; i++) {
		for(j=0;j<COLUMNS; j++) {
			gameMap.level_map[i][j] = level_0[i][j];
		}
	}
	draw_map();
 	draw_digger(player);
	draw_nobbin(enemys[0]);
	
	digger_move_pid = create(run_digger,INITSTK,INITPRIO+2,"move_digger",1,&player);
	debug = create(refresh_debug_map,INITSTK,INITPRIO,"debug_line",0);
	terminate_xinu_pid = create(kill_xinu,INITSTK,INITPRIO+3,"kill_Xinu",0);
	move_enemys_pid = create(move_nobbins,INITSTK,INITPRIO+1,"move_nobbins",0);
	//bg_sound_pid = create(beethoven,INITSTK,INITPRIO-3,"background_sounds",0);
	gold_falling_pid = create(gold_falling,INITSTK,INITPRIO-1,"falling_gold",0);
	sound_effects_pid = create(sound_effects,INITSTK,INITPRIO-1,"sounds",0);


	resume(digger_move_pid);
	resume(debug);
	resume(terminate_xinu_pid);
	resume(move_enemys_pid);
	//resume(bg_sound_pid);
	resume(gold_falling_pid);
	resume(sound_effects_pid);
	
	setup_interrupts();
	
    schedule(7,57,  debug, 29,move_enemys_pid, 29, digger_move_pid, 29, bg_sound_pid, 29,terminate_xinu_pid,29,gold_falling_pid,29, sound_effects_pid,29); 
	return(OK);
}
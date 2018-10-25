#include <conf.h>
#include <kernel.h>
#include "map.h"
#include "digger.h"
#include "myints.h"
#include "nobin.h"

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

} // schedule 


void kill_xinu(){
	int sec = 1;
	receive();
	clean_screen();
	printf("terminating program in %d second/s", sec);
	sleep(sec);
	clean_screen();
	xdone();
}

/*------------------------------------------------------------------------
 *----------------------   Main Digger Program ---------------------------
 *------------------------------------------------------------------------
 */
xmain()
{
	int i;
	player = create_digger();
	for(i=0;i<NOBBIN_COUNT;i++)
		enemys[i] = create_nobbin(&player);
	enemys[0].is_alive = 1;
	Int9Save = getvect(0x09);

	map_moves_pid = create(refresh_map,INITSTK,INITPRIO,"refresh_map",0);
	digger_move_pid = create(move_digger,INITSTK,INITPRIO,"move_digger",1,&player);
	map_debug_pid = create(refresh_debug_map,INITSTK,INITPRIO,"refresh_debug_map",0);
	terminate_xinu_pid = create(kill_xinu,INITSTK,INITPRIO,"kill_Xinu",0);
	

	resume(map_moves_pid);
	resume(digger_move_pid);
	resume(map_debug_pid);
	resume(terminate_xinu_pid);
	
	set_new_int9_newisr();
	
    schedule(3,57, map_moves_pid, 0,  digger_move_pid, 29, map_debug_pid, 29 );
} 



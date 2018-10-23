#include <conf.h>
#include <kernel.h>
#include "map.h"
#include "digger.h"
#include "myints.h"

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

/*------------------------------------------------------------------------
 *----------------------   Main Digger Program ---------------------------
 *------------------------------------------------------------------------
 */
xmain()
{
	int uppid, recvpid;
	
	Int9Save = getvect(0x09);
	Int8Save = getvect(0x08);

	
	recvpid = create(move_digger,INITSTK,INITPRIO,"move_digger",1,&player);
	receiver_pid = recvpid;
	
	set_new_int9_newisr();
	//setvect(8, MyISR8);
	resume(uppid = create(refresh_map,INITSTK,INITPRIO,"refresh_map",0));
	resume(receiver_pid);
	
	
    schedule(2,57, recvpid, 0,  uppid, 29);
} // xmain

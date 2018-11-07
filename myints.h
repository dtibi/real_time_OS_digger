#ifndef MYINTS_FILE
#define MYINTS_FILE

#include <dos.h>
#include <kernel.h>
#include <stdio.h>
#include <conf.h>
#include <io.h>
#include <sleep.h>
#include <proc.h>
#include <butler.h>

#define LEFT_ARROW 75
#define RIGHT_ARROW 77
#define DOWN_ARROW 80
#define UP_ARROW 72
#define SPACE_BAR 57

#define SECONDT 1000
#define LETCH 900
#define FACTOR 2
#define DELTA_SPEED 0.2

extern int num_of_pids, dig_uppid, mon_uppid, dispid, nob_uppid, recvpid, debug, sound_effects_pid, score_lives_pid, terminate_xinu_pid, nobbin_creator_pid, receiver_pid;
extern long tod;
extern int pressed_flag, pressed;
extern INTPROC (*Int9Save)(int);

void kill_xinu(int* sched_arr_pid );
void setup_interrupts();
void restore_ints();
INTPROC MyISR9(int mdevno);

#endif

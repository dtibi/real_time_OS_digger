#ifndef MYINTS_FILE
#define MYINTS_FILE
#include <dos.h>
#include <kernel.h>
#include <stdio.h>
#include <conf.h>
#include <io.h>
#include <sleep.h>
#include <proc.h>

#define LEFT_ARROW 75
#define RIGHT_ARROW 77
#define DOWN_ARROW 80
#define UP_ARROW 72
#define SPACE_BAR 57


#define SECONDT 90
#define LETCH 9700
#define FACTOR 2

extern int num_of_pids,dig_uppid,mon_uppid, dispid, recvpid, debug,sound_effects_pid,score_lives_pid,bg_sound,terminate_xinu_pid,nobbin_creator_pid,receiver_pid;
extern long time_from_start;
extern int num_of_pids,pressed_flag,pressed;

void setup_interrupts();
void restore_ints();
void kill_xinu();
extern INTPROC (*Int9Save)(int);
extern INTPROC (*Int8Save) (int);
INTPROC MyISR9(int mdevno);
INTPROC MyISR8(int mdevno);


#endif
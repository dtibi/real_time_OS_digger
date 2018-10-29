#ifndef MYINTS_FILE
#define MYINTS_FILE
#include <dos.h>
#include <kernel.h>
#include <stdio.h>
#include <conf.h>
#include <io.h>



#define LEFT_ARROW 75
#define RIGHT_ARROW 77
#define DOWN_ARROW 80
#define UP_ARROW 72
#define SPACE_BAR 0x59

//speed of sound delays
#define LONG_DELAY 5
#define SHORT_DELAY 1

extern volatile unsigned long count,delay_timer;
extern volatile unsigned int map_moves_pid, digger_move_pid, debug, terminate_xinu_pid, move_enemys_pid, bg_sound_pid, gold_falling_pid,sound_effects_pid;
extern int receiver_pid;

void setup_interrupts();
void restore_ints();
extern INTPROC (*Int9Save)(int);
extern INTPROC (*Int8Save) (int);
INTPROC MyISR9(int mdevno);
INTPROC MyISR8(int mdevno);


#endif
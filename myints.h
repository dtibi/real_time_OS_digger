#ifndef MYINTS_FILE
#define MYINTS_FILE
#include <kernel.h>

#define LEFT_ARROW 75
#define RIGHT_ARROW 77
#define DOWN_ARROW 80
#define UP_ARROW 72

extern volatile int count,delay_timer;
extern volatile unsigned char scan;
extern volatile unsigned char ascii;
extern volatile unsigned int map_moves_pid, digger_move_pid, debug, terminate_xinu_pid, move_enemys_pid,bg_sound_pid;
extern volatile int keep_playing;

void set_new_int9_newisr();
extern void interrupt (*Int9Save) (void);
extern void interrupt (*Int8Save) (void);
INTPROC MyISR9();
void interrupt MyISR8(void);


#endif
#include "map.c"
#include "digger.c"
#include "nobin.c"
#include "sound.c"
#include "myints.c"


int sched_arr_pid[10];
int sched_arr_int[10];
int point_in_cycle;
int gcycle_length;
int gno_of_pids;


extern SYSCALL  sleept(int);
extern struct intmap far *sys_imp;


char ch_arr;

LOCAL new_pid()
{
	int	pid;			/* process id to return		*/
	int	i;

	for (i=1 ; i<NPROC ; i++) {	/* check all NPROC slots	*/
		if (proctab[i].pstate == PRFREE)
			return i;
	}
	return(SYSERR);
}
 
SYSCALL schedule(int no_of_pids, int cycle_length, int pid1, ...) {
	int i,j;
	int ps;
	int *iptr;

	disable(ps);

	gcycle_length = cycle_length;
	point_in_cycle = 0;
	gno_of_pids = no_of_pids ;
	num_of_pids = no_of_pids ;
	iptr = &pid1;
	for(i=0; i < no_of_pids; i++) {
		sched_arr_pid[i] = *iptr;
		iptr++;
		sched_arr_int[i] = *iptr;
		iptr++;
	}
	
	restore(ps);
	return (OK);
}

void displayer() {
	int x, y, i, j;
	char c;
	for (i = 0; i < ROWS; i++) {
		for(j = 0; j < COLUMNS; j++)
			gameMap.level_map[i][j] = levels[gameMap.level_id][i][j];
	}
	upd_draw_digger(player);
	disp_draw_map();
	while (1) {
		receive();
		
		disp_draw_pixel_with_char(0, 70, GRAY_BG, ' ');
		for (i=0; i<ROWS; i++) {
			for(j = 0; j < COLUMNS; j++){
				if(gameMap.refresh_map[i][j] == 1){
					disp_draw_area(i, j);
					gameMap.refresh_map[i][j] = 0;
				}
			}
		}
		
		/* for (i=0; i<ROWS; i++) {
			for(j=0;j<COLUMNS; j++) {
				if(gameMap.level_map[i][j] == DIGGER) continue;
				c = gameMap.level_map[i][j] + '0';
				disp_draw_pixel_with_char(row_2_pixel(i)+(HEIGHT/2),column_2_pixel(j)+(WIDTH/2),BLACK_BG,c);
			}
		}  */

		
		disp_draw_pixel_with_char(0, 70, BLACK_BG, ' ');
	
	 }
}

void receiver() {
	char temp;
	while(1) {
		temp = receive();
		if(temp!=0) {
			pressed=1;
			ch_arr = temp;
		}
		else pressed=0;
	}
}


void updater() {
	int i, j, direction, button_sc;
	while(1) {
		receive();
		disp_draw_pixel_with_char(0,68,GREEN_BG, ' ');
		if(!player.is_alive) digger_death_flow();
		if(pressed) {
			button_sc = ch_arr;
			if(button_sc == SPACE_BAR){
				if ((time_from_start - player.last_time_shot) / SECONDT >= gameMap.digger_reload_time){
					player.last_time_shot = time_from_start;
					send(sound_effects_pid,4);
					resume(create(fireball_advance, INITSTK, INITPRIO+1, "weapon_fired", 3 ,player.y ,player.x ,player.direction));
					continue;
				}
			} else {
				move_digger(button_sc);
			}
		}
		if (crazy_mode && (tod-player.last_time_cherry)/SECONDT >= gameMap.crazy_mode_time ) {
			stop_crazy_mode();
			crazy_mode = 0;
		}
		disp_draw_pixel_with_char(0,68,BLACK_BG, ' ');
  }
}

void nobbin_updater() {
	int i, j, direction, button_sc;
	while(1) {
		receive();
		sleept((int)((SECONDT/FACTOR)+(SECONDT/FACTOR)*gameMap.monster_speed));
		disp_draw_pixel_with_char(0,66,BROWN_BG, ' ');
		for (i=0;i<ENEMY_COUNT;i++) {	
			if(enemys[i].is_alive) {
				move_nobbin(i);
				sprintf(debug_str,"enemy %d is beeing moved",i);
				send(debug,debug_str);
			}
		}
		disp_draw_pixel_with_char(0,66,BLACK_BG, ' ');
  }
}



xmain() {
	int i,j;
	player = create_digger();
	setup_clean_screen();
	create_map(0);
	
	disp_draw_map();
	disp_draw_lives(player.lives);
	disp_draw_score(player.score);
	
	resume(dispid = create(displayer, INITSTK, INITPRIO, "DISPLAYER", 0));
	resume(recvpid = create(receiver, INITSTK, INITPRIO+3, "RECIVEVER", 0));
	resume(dig_uppid = create(updater, INITSTK, INITPRIO, "dUPDATER", 0));
	resume(nob_uppid = create(nobbin_updater, INITSTK, INITPRIO, "nUPDATER", 0));
	resume(debug = create(refresh_debug_map, INITSTK, INITPRIO + 3, "debug_line",0));
	resume( sound_effects_pid = create(sound_effects,INITSTK,INITPRIO+1,"sound_effects_pid",0));
	resume( score_lives_pid = create(score_lives_updater,INITSTK,INITPRIO+3,"score_lives_updating",0));
	resume( terminate_xinu_pid = create(kill_xinu,INITSTK,INITPRIO+3,"kill_Xinu",1,(int*)&sched_arr_pid));
	resume( nobbin_creator_pid = create(nobbin_creator,INITSTK,INITPRIO,"nobbin_creator",0));
	if (dispid == SYSERR  ||
		recvpid == SYSERR ||
		dig_uppid == SYSERR ||
		debug == SYSERR ||
		sound_effects_pid == SYSERR ||
		score_lives_pid == SYSERR ||
		terminate_xinu_pid == SYSERR ||
		nobbin_creator_pid == SYSERR ) {
			printf("ERROR! could not create 1 of xmain proccesses");
			xdone();
	}
	receiver_pid = recvpid;
	setup_interrupts();
    schedule(4,5, dispid, 1,  dig_uppid, 2, mon_uppid, 3,nob_uppid,4);
	
	return (OK);
}

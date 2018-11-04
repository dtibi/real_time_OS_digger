#include "map.c"
#include "digger.c"
#include "nobin.c"
#include "sound.c"
#include "myints.c"


int sched_arr_pid[ENEMY_COUNT+10];
int sched_arr_int[ENEMY_COUNT+10];
int point_in_cycle;
int gcycle_length;
int gno_of_pids;

extern SYSCALL  sleept(int);
extern struct intmap far *sys_imp;

char ch_arr;

 
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


void digger_updater() {
	int i, j, direction, button_sc;
	while(1) {
		receive();
		sleept(5);
		disp_draw_pixel_with_char(0,68,GREEN_BG, ' ');
		
		if(!player.is_alive) digger_death_flow();
		if(pressed) {
			button_sc = ch_arr;

			if(button_sc == SPACE_BAR){
				if ((time_from_start - player.last_time_shot) / SECONDT >= gameMap.digger_reload_time){
					player.last_time_shot = time_from_start;
					resume(create(fireball_advance, INITSTK, INITPRIO+1, "weapon_fired", 3 ,player.y ,player.x ,player.direction));
					continue;
				}
			} else {
				move_digger(button_sc);
			}
		}
		
		disp_draw_pixel_with_char(0,68,BLACK_BG, ' ');
  }
}

xmain() {
	int i,j;
	char* pname;
	player = create_digger();
	setup_clean_screen();
	create_map(0);
	
	disp_draw_map();
	disp_draw_lives(player.lives);
	disp_draw_score(player.score);
	
	resume(dispid = create(displayer, INITSTK, INITPRIO, "DISPLAYER", 0));
	resume(recvpid = create(receiver, INITSTK, INITPRIO+3, "RECIVEVER", 0));
	resume(dig_uppid = create(digger_updater, INITSTK, INITPRIO+1, "digger_UPDATER", 0));
	resume(debug = create(refresh_debug_map, INITSTK, INITPRIO + 3, "debug_line",0));
	resume( bg_sound = create(beethoven,INITSTK,INITPRIO,"bg_sound",0));
	resume( sound_effects_pid = create(sound_effects,INITSTK,INITPRIO+1,"sound_effects_pid",1,bg_sound));
	resume( score_lives_pid = create(score_lives_updater,INITSTK,INITPRIO+3,"score_lives_updating",0));
	resume( terminate_xinu_pid = create(kill_xinu,INITSTK,INITPRIO+3,"kill_Xinu",1,(int*)&sched_arr_pid));
	resume( nobbin_creator_pid = create(nobbin_creator,INITSTK,INITPRIO,"nobbin_creator",0));
	
	
	receiver_pid = recvpid;
	setup_interrupts();
    schedule(3,4+ENEMY_COUNT, dispid, 1,  dig_uppid, 2, mon_uppid, 3);
	
	return (OK);
}

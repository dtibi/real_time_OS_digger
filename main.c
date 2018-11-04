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
int pressed = 0;

 
SYSCALL schedule(int no_of_pids, int cycle_length, int pid1, ...) {
	int i;
	int ps;
	int *iptr;

	disable(ps);

	gcycle_length = cycle_length;
	point_in_cycle = 0;
	gno_of_pids = no_of_pids;

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
		} */
		
		disp_draw_pixel_with_char(0, 70, BLACK_BG, ' ');
	
	 }
}

void receiver() {
	char temp;
	while(1) {
		temp = receive();
		pressed = 1;
		ch_arr = temp;
	}
}


void updater() {
	int i, j, counter = 1, direction, button_sc;

	while(1) {
		receive();
		sleept(5);
		disp_draw_pixel_with_char(0,68,GREEN_BG, ' ');
		counter++;
		
		if(!player.is_alive) digger_death_flow();
		if(pressed) {
			button_sc = ch_arr;
			pressed = 0;

			if(button_sc == SPACE_BAR){
				if ((tod - player.last_time_shot) / SECONDT >= gameMap.digger_reload_time){
					player.last_time_shot = tod;
					resume(create(fireball_advance, INITSTK, INITPRIO+1, "weapon_fired", 3 ,player.y ,player.x ,player.direction));
					continue;
				}
			} else {
				if(gameMap.digger_speed == 1 || counter%gameMap.digger_speed > 0) move_digger((Digger*)&player,button_sc);
			}
		}
		if(gameMap.digger_speed == 1 || counter%gameMap.digger_speed > 0) move_nobbins();
		
		//after buffer is empty we get here 
		//start moving nobbins 
		/* if(counter%5 > 0) move_nobbins();
		else	counter=1; */
		
		disp_draw_pixel_with_char(0,68,BLACK_BG, ' ');
  }
}

void kill_xinu(){
	int i;
	receive();
	for(i=0; i < 2; i++){
		kill(sched_arr_pid[i]);
	} 
	kill(bg_sound);
	sleept(SECONDT);
	no_sound();
	setup_clean_screen();
	xdone();
	asm INT 27; // terminate xinu
	return;
}

xmain() {
	int i,j;
	player = create_digger();
	create_enemys();
	setup_clean_screen();
	create_map(0);
	
	disp_draw_map();
	disp_draw_lives(player.lives);
	disp_draw_score(player.score);
	
	printf("%d",enemys_alive_count());
	
	resume(dispid = create(displayer, INITSTK, INITPRIO, "DISPLAYER", 0));
	resume(recvpid = create(receiver, INITSTK, INITPRIO, "RECIVEVER", 0));
	resume(uppid = create(updater, INITSTK, INITPRIO, "UPDATER", 0));
	resume(debug = create(refresh_debug_map, INITSTK, INITPRIO + 3, "debug_line",0));
	resume( bg_sound = create(beethoven,INITSTK,INITPRIO,"bg_sound",0));
	resume( sound_effects_pid = create(sound_effects,INITSTK,INITPRIO+1,"sound_effects_pid",1,bg_sound));
	resume( score_lives_pid = create(score_lives_updater,INITSTK,INITPRIO+3,"score_lives_updating",0));
	resume(terminate_xinu_pid = create(kill_xinu,INITSTK,INITPRIO+3,"kill_Xinu",0));
	resume(nobbin_creator_pid = create(nobbin_creator,INITSTK,INITPRIO,"nobbin_creator",0));
	receiver_pid = recvpid;
	setup_interrupts();
    schedule(2,3, dispid, 1,  uppid, 2);
	
	return (OK);
}

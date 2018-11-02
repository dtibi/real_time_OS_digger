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

char ch_arr[2048];
int front = -1;
int rear = -1;

 
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
			gameMap.level_map[i][j] = level_0[i][j];
	}
	upd_draw_digger(player);
	upd_draw_nobbin(enemys[0].y, enemys[0].x);
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
		
		for (i=0; i<ROWS; i++) {
			for(j=0;j<COLUMNS; j++) {
				if(gameMap.level_map[i][j] == DIGGER) continue;
				c = gameMap.level_map[i][j] + '0';
				disp_draw_pixel_with_char(row_2_pixel(i)+(HEIGHT/2),column_2_pixel(j)+(WIDTH/2),BLACK_BG,c);
			}
		}
		
		disp_draw_pixel_with_char(0, 70, BLACK_BG, ' ');
	 }
}

void receiver() {
	char temp;
	while(1) {
		temp = receive();
		disp_draw_pixel_with_char(0, 71, BLACK_BG, ' ');
		rear++;
		ch_arr[rear] = temp;
		if (front == -1)
		   front = 0;
	}
}


void updater() {
	int i, j, counter = 1, direction, button_sc;
	
	enemys[0].is_alive = 1; 

	while(1) {
		receive();
		disp_draw_pixel_with_char(0,68,GREEN_BG, ' ');
		counter++;
		
		while(front != -1) {
			button_sc = ch_arr[front];
			if(front != rear)
			   front++;
			else
				front = rear = -1;
			
			if(button_sc == SPACE_BAR){
				
				if (player.weapon_reloaded == 1){
					//player.weapon_reloaded = 0;
					resume(create(fireball_advance, INITSTK, INITPRIO+1, "weapon_fired", 3 ,player.y ,player.x ,player.direction));
					upd_draw_digger(player);
					continue;
				}
			}
			move_digger((Digger*)&player,button_sc);
			if(counter%5 > 0) move_nobbins();
			else	counter=1;
		}
		if(!player.is_alive) digger_death_flow();

		//after buffer is empty we get here 
		//start moving nobbins 
		if(counter%5 > 0) move_nobbins();
		else	counter=1;
		
		disp_draw_pixel_with_char(0,68,BLACK_BG, ' ');
  }
}

xmain() {
	int i,j;
	player = create_digger();
	for(i = 0; i < ENEMY_COUNT; i++)
		enemys[i] = create_enemy((Digger*)&player);
	enemys[0].is_alive=1;
	//enemys[1].is_alive=1;
	//enemys[2].is_alive=1;
	
	setup_clean_screen();

	for (i=0; i<ROWS; i++) {
		for(j=0;j<COLUMNS; j++) {
			gameMap.level_map[i][j] = level_0[i][j];
		}
	}
	upd_draw_digger(player);
	upd_draw_nobbin(enemys[0].y,enemys[0].x);
	disp_draw_map();
	disp_draw_lives(player.lives);
	disp_draw_score(0);
	
	resume(dispid = create(displayer, INITSTK, INITPRIO, "DISPLAYER", 0));
	resume(recvpid = create(receiver, INITSTK, INITPRIO, "RECIVEVER", 0));
	resume(uppid = create(updater, INITSTK, INITPRIO, "UPDATER", 0));
	resume(debug = create(refresh_debug_map, INITSTK, INITPRIO + 3, "debug_line",0));
	resume( sound_effects_pid = create(sound_effects,INITSTK,INITPRIO+1,"sound_effects_pid",0));
	resume( score_lives_pid = create(score_lives_updater,INITSTK,INITPRIO+3,"score_lives_updating",0));
	receiver_pid = recvpid;
	setup_interrupts();
    schedule(3,3, dispid, 1,  uppid, 0, dispid, 0);
	
	return (OK);
}

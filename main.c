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
int uppid, dispid, recvpid, debug,gold_falling_pid,sound_effects_pid;

int update_buffer[20][2];
int front_buf = -1;
int rear_buf = -1;

char ch_arr[2048];
int front = -1;
int rear = -1;
 
int point_in_cycle;
int gcycle_length;
int gno_of_pids;

 
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
	return (OK);
} // schedule 


void displayer( void )
{
	int x,y,i,j;
	char c;
	while (1){
		
		receive();
		
		disp_draw_pixel(0,70,GRAY_BG);
		for (i=0; i<ROWS; i++) {
			for(j=0;j<COLUMNS; j++) {
				if(gameMap.refresh_map[i][j]==1)disp_draw_area(i,j);
			}
		}
		
		/* for (i=0; i<ROWS; i++) {
			for(j=0;j<COLUMNS; j++) {
				c = gameMap.level_map[i][j] + '0';
				disp_draw_pixel_with_char(row_2_pixel(i)+(HEIGHT/2),column_2_pixel(j)+(WIDTH/2),BLACK_BG,c);
			}
		} */
		
		disp_draw_pixel(0,70,BLACK_BG);
		
	 } //while
} // prntr

void receiver(){
	char temp;
	while(1){
    temp = receive();
	disp_draw_pixel(0,71,BLACK_BG);
    rear++;
    ch_arr[rear] = temp;
    if (front == -1)
       front = 0;
  } // while

} //  receiver


void updater() {
	int i, j,counter=1,direction,button_sc;

	enemys[0].is_alive = 1; 
	
	
	

	while(1){

	receive();
	disp_draw_pixel(0,68,GREEN_BG);
	counter++;
	
	while(front != -1)
   {
		button_sc = ch_arr[front];
		if(front != rear)
		   front++;
		else
			front = rear = -1;
		//if(!player.is_alive) {digger_death_flow();}
		move_digger((Digger*)&player,button_sc);
	} // while(front_ch != -1)
	

	//when buffer is empty we get here 
	//start moving nobbins 
	if(counter%5>0){
		for(i=0;i<NOBBIN_COUNT;i++){
			if(enemys[i].is_alive==1) {
				direction = find_direction_to_digger(enemys[i]);
				//sprintf(debug_str,"nobbin direction: %d | LEFT=%d , RIGHT=%d , UP=%d , DOWN=%d",direction, LEFT_ARROW,RIGHT_ARROW,UP_ARROW,DOWN_ARROW);
				//send(debug,debug_str);
				upd_draw_empty(enemys[i].y,enemys[i].x,1);
				switch (direction)
				{
					case LEFT_ARROW:
						enemys[i].x--;
						break;
					case RIGHT_ARROW:
						enemys[i].x++;
						break;
					case DOWN_ARROW:
						enemys[i].y++;
						break;
					case UP_ARROW:
						enemys[i].y--;
						break;
				}
				if(direction!=0) enemys[i].direction=direction;
				upd_draw_nobbin(enemys[i].y,enemys[i].x);
			}
		}
	} else counter=1;
	
	//sleept(1);
	disp_draw_pixel(0,68,BLACK_BG);
  } // while(1)

} // updater 


xmain() {
	int i,j;
	player = create_digger();
	for(i=0;i<NOBBIN_COUNT;i++)
		enemys[i] = create_nobbin((Digger*)&player);
	enemys[0].is_alive=1;
	enemys[1].is_alive=1;
	enemys[2].is_alive=1;
	setup_clean_screen();
	for (i=0; i<ROWS; i++) {
		for(j=0;j<COLUMNS; j++) {
			gameMap.level_map[i][j] = level_0[i][j];
		}
	}
	upd_draw_digger(player);
	upd_draw_nobbin(enemys[0].y,enemys[0].x);
	disp_draw_map();
	
	resume( dispid = create(displayer, INITSTK, INITPRIO, "DISPLAYER", 0) );
	resume( recvpid = create(receiver, INITSTK, INITPRIO, "RECIVEVER", 0) );
	resume( uppid = create(updater, INITSTK, INITPRIO, "UPDATER", 0) );
	resume( debug = create(refresh_debug_map,INITSTK,INITPRIO+3,"debug_line",0));
	resume( gold_falling_pid = create(gold_falling,INITSTK,INITPRIO,"gold_falling",0));
	resume( sound_effects_pid = create(sound_effects,INITSTK,INITPRIO,"sound_effects_pid",0));
	
	receiver_pid =recvpid;
	setup_interrupts();
    schedule(3,3, dispid, 1,  uppid, 0, dispid, 0);
	return (OK);
} // xmain

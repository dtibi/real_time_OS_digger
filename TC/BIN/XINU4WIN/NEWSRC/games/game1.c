/* game.c - xmain, prntr */

#include <conf.h>
#include <kernel.h>

extern SYSCALL  sleept(int);

/*------------------------------------------------------------------------
 *  xmain  --  example of 2 processes executing the same code concurrently
 *------------------------------------------------------------------------
 */

#define ARROW_NUMBER 5
#define TARGET_NUMBER 4


typedef struct position
{
  int x;
  int y;

}  POSITION;

char display[2001];

char ch_arr[2048];
int front = -1;
int rear = -1;

int point_in_cycle;
int gcycle_length;
int gno_of_pids;


/*------------------------------------------------------------------------
 *  prntr  --  print a character indefinitely
 *------------------------------------------------------------------------
 */



void displayer( void )
{
	while (1)
         {
               receive();
               //sleept(18);
               printf(display);
         } //while
} // prntr

void receiver()
{
  while(1)
  {
    char temp;
    temp = getc(CONSOLE);
    rear++;
    ch_arr[rear] = temp;
    if (front == -1)
       front = 0;
    //getc(CONSOLE);
  } // while

} //  receiver


char display_draft[25][80];
POSITION target_pos[TARGET_NUMBER];
POSITION arrow_pos[ARROW_NUMBER];


void updateter()
{

  int i,j;
  int gun_position;           
  int no_of_arrows;
  int target_disp = 80/TARGET_NUMBER;
  char ch;

  int no_of_targets;

  no_of_arrows = 0;

  no_of_targets = 4;

  gun_position = 39;

  target_pos[0].x = 3;
  target_pos[0].y = 0; 


  for(i=1; i < TARGET_NUMBER; i++)
  {
    target_pos[i].x = i*target_disp;
    target_pos[i].y = 0; 

  } // for
  for(i=0; i < ARROW_NUMBER; i++)
       arrow_pos[i].x =  arrow_pos[i].y = -1;

  while(1)
  {

   receive();

   while(front != -1)
   {
     ch = ch_arr[front];
     if(front != rear)
       front++;
     else
       front = rear = -1;

     if ( (ch == 'a') || (ch == 'A') )
       if (gun_position >= 2 )
              gun_position--;
       else;
     else if ( (ch == 'd') || (ch == 'D') )
       if (gun_position <= 78 )
         gun_position++;
       else;
     else if ( (ch == 'w') || (ch == 'W') )
       if (no_of_arrows < ARROW_NUMBER)
       {
         arrow_pos[no_of_arrows].x = gun_position;
         arrow_pos[no_of_arrows].y = 23;
         no_of_arrows++;

       } // if
   } // while(front != -1)

     ch = 0;
     for(i=0; i < 25; i++)
        for(j=0; j < 80; j++)
            display_draft[i][j] = ' ';  // blank

    display_draft[22][gun_position] = '^';
    display_draft[23][gun_position-1] = '/';
    display_draft[23][gun_position] = '|';
    display_draft[23][gun_position+1] = '\\';
    display_draft[24][gun_position] = '|';

    for(i=0; i < ARROW_NUMBER; i++)
       if (arrow_pos[i].x != -1)
       {
         if (arrow_pos[i].y > 0)
           arrow_pos[i].y--;
           display_draft[arrow_pos[i].y][arrow_pos[i].x] = '^';
           display_draft[arrow_pos[i].y+1][arrow_pos[i].x] = '|';

       } // if

    for(i=0; i < TARGET_NUMBER; i++)
       if (target_pos[i].x != -1)
        {
         if (target_pos[i].y < 22)
              target_pos[i].y++;
         display_draft[target_pos[i].y][target_pos[i].x] = '*';
        } // if

    for(i=0; i < 25; i++)
      for(j=0; j < 80; j++)
        display[i*80+j] = display_draft[i][j];
    display[2000] = '\0';

  } // while(1)

} // updater 

int sched_arr_pid[5] = {-1};
int sched_arr_int[5] = {-1};


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

xmain()
{
        int uppid, dispid, recvpid;
        resume( dispid = create(displayer, INITSTK, INITPRIO, "DISPLAYER", 0) );
        resume( recvpid = create(receiver, INITSTK, INITPRIO+3, "RECIVEVER", 0) );
        resume( uppid = create(updateter, INITSTK, INITPRIO, "UPDATER", 0) );

    
    schedule(2,57, dispid, 0,  uppid, 29);
} // xmain

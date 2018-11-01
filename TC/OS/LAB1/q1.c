#include <stdio.h>
#include <conio.h>
#include <dos.h>
#include <time.h>
#include <string.h>
#include <math.h>

volatile int time_passed=0; //count the amount of times went into h8
volatile int slowness=2; //the slow level
volatile int firts_call_slowE=1; //flag
volatile int keep_waiting=1; //false when user has entered a complete correct password or when time passed
volatile int global_time=0, global_size=0, global_i=0, global_key_pressed=0;
volatile char * global_pass=NULL;

void interrupt (*int8Save) (void);
void interrupt (*int9Save) (void);

int slowE ( char passwd[], int size ,int time );
long slowD ( char passwd[], int size );
void first_setup();

void interrupt h9 (void) {
	char c1=-1;
	int9Save();
	global_key_pressed=!global_key_pressed;
	if(global_key_pressed){
		asm{
			MOV AH,0
			INT 16h
			MOV c1,AL
			MOV AX,0
		}
		time_passed=0;
		if (c1=='q'){
			slowD(global_pass,global_size); //in case we want to quit the program for any reason
		}
		if (global_pass[global_i]==c1) {
			printf("yes global_pass[global_i]==c1 (%c)==(%c)  i=%d , size=%d\n",global_pass[global_i],c1, global_i,global_size );
			global_i++;
			if (global_i==global_size) {//password was correct
				keep_waiting=0;
				global_i=0;
				printf("Corecct pass\n");
			}
		}
		else {
			global_i=0;
		}
	}
}

void interrupt h8 (void)
{
	if(time_passed>global_time*18) {
		printf("Time passed\n");
		keep_waiting=0;
		time_passed=0;
		if(slowness<16)	slowness=slowness*2;
	}

	time_passed++;
	if(time_passed%18==0) {
			printf("second passed slowness=%d time_passed=%d \n",slowness,time_passed);
	}
	int8Save();
	if(time_passed%slowness)
	{
		asm{
			STI
		}
		while(time_passed%slowness);
	}
	if(time_passed%18==0) {
		printf("sleeping for 3 seconds time_passed=%d\n",time_passed);
		asm{
			STI
		}
		sleep(1);
		printf("finished sleeping for 3 seconds\n");
	}
}

int slowE (char passwd[], int size , int time ) {
	if (firts_call_slowE) first_setup();
	global_i=0;
	global_pass = passwd;
	global_size=size;
	global_time=time;
	keep_waiting=1;
	while(keep_waiting);
	return slowness;
}

long slowD(char passwd[] , int size){
	int auth_passed = strcmp(passwd,global_pass);
	if (global_size==size && auth_passed==0){
		setvect(8,int8Save); /* reastoring int 8*/
		setvect(9,int9Save);
		asm {
			mov AH,4ch
			int 21h
		}
	}
	return 0;
}

void first_setup(){
	int8Save = getvect(8);
	int9Save = getvect(9);
	setvect(8,h8);
	setvect(9,h9);
	firts_call_slowE=0;
}

int main()
{
	unsigned long int i, j;
	int counter = 0 ; 
	time_t t1,t2;
	i = j = 0;  
	
	while (1)
	{
		(void) time(&t1);
		if (counter++ == 10)
			slowD("aaaabb" , 6);
		printf("speed : 1 / %d \n", slowE("aaaabb" , 6 ,  5));
		while(j < 10)
		{
			i++;
			if ((i % 1000) == 0)
				j++;
		}// while
		(void) time(&t2);
		printf("\nTotal  Iteration  Time -  = %d secondsn\n", (int)(t2-t1));
	}
	return 1;
}
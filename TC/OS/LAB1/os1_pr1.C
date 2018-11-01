#include <stdio.h>
#include <conio.h>
#include <dos.h>
#include <time.h>
#include <string.h>
#include <math.h>

volatile int global_key_pressed=1; //flag
volatile int time_passed=0; //count the amount of times went into h8
volatile char char_ascii=0; //char we pressed
volatile int slowness=1; //the slow level
volatile int firts_call_slowE=1; //flag
volatile int global_time=0, global_size=0, global_i=0;
volatile char* global_pass;

void interrupt (*int8Save) (void); 
void interrupt (*int9Save) (void);

int slowE ( char passwd[], int size ,int time );
long slowD ( char passwd[], int size );
void first_setup();

void interrupt h9 (void) {
	char scan_code=0;
	(*int9Save)();
	global_key_pressed=!global_key_pressed;
	if(!global_key_pressed) 
		asm {
			jmp Skip1
		}
	asm {
		PUSH AX
		MOV AH,1
		INT 16h
		JZ Skip1
		MOV AH,0
		INT 16h
		MOV char_ascii,AL
		MOV scan_code,AH
		POP AX
	}
	//printf("%d\n",scan_code);
	if (scan_code==75 || scan_code==77){
		slowD((char*)global_pass,global_size);
	} else if (scan_code==72 && slowness<16){//down arrow in keyboard will decrease the speed
		slowness=slowness*2;
		printf("made is slower now slowness is %d\n",slowness);
	} else if (scan_code==80 && slowness>1){//up arrow in keyboard will increase the speed 
		slowness=slowness/2;
		printf("made is faster now slowness is %d\n",slowness);
	} else if ((global_i>=0 && global_i<global_size) && global_pass[global_i]==char_ascii) {
		time_passed=0;
		global_i++;
		//printf("yes!!!  - global_pass[global_i-1]==char_ascii (%c)==(%c)  i=%d , size=%d\n",global_pass[global_i-1],char_ascii, global_i,global_size );
		if (global_i==global_size) {//password was correct
			printf("password was entered - Access Granted\n");
			slowD((char*)global_pass,global_size);
		}
	} else {
		//printf("keypress - \tascii = %c \treseting time and 'i'...\n",char_ascii);
		global_i=0;
		time_passed=0;
	}
	Skip1:
}

void interrupt h8 (void)
{
	(*int8Save)();
	
	time_passed++;
	if(time_passed>global_time*18) {
		//printf("Time passed try password again\n");
		time_passed=0;
		global_i=0;
	}
	
	if(time_passed%slowness)
	{
		asm{
			STI
		}
	}
	while(time_passed%slowness);
}

int slowE (char passwd[], int size , int time ) {
	if (firts_call_slowE) 
		first_setup(passwd,size,time);
	else {
		if(strcmp((char*)passwd,(char*)global_pass)==0 && global_size==size) {
			if(slowness<16)	slowness=slowness*2;
			global_time=time;
		}
	}
	return slowness;
}

long slowD(char passwd[] , int size){
	int auth_passed = strcmp(passwd,(char*)global_pass);
	if (global_size==size && auth_passed==0){
		slowness=1;
		free(global_pass);
		setvect(9,int9Save);
		setvect(8,int8Save); /* reastoring int 8*/
		printf("Exiting to Dos!\n");
		asm {
			mov AH,4ch
			int 21h
		}
	}
	return 0;
}

void first_setup(char passwd[],int size,int time){
	global_pass = (char*)malloc(size*sizeof(char));
	firts_call_slowE=0;
	slowness=slowness*2;
	stpcpy((char*)global_pass , passwd);
	printf("global_pass = %s\n",global_pass);
	global_i=0;
	global_size=size;
	global_time=time;
	int8Save = getvect(8);
	int9Save = getvect(9);
	setvect(8,h8);
	setvect(9,h9);
}

void main()
{
	unsigned long int i, j;
	int counter = 0 ; 
	time_t t1,t2; 
	i=j=0;
	while (1)
	{
		(void) time(&t1);
		if (counter++ == 100)
			slowD("aaaabb" , 6);
		printf("speed : 1 / %d \n", slowE("aaaabb" , 6 ,  5));
		i = 0;
		j = 0; 
		while(j < 5)
		{
			i++;
			if ((i % 10000) == 0){
				j++;
			}
		}// while
		(void) time(&t2);
		printf("\nTotal  Iteration  Time -  = %d secondsn\n", (int)(t2-t1));
	}
}
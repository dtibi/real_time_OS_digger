/* readyear.c */
#include <stdio.h>
#include <dos.h>

#define MAX_ALARMS 30

typedef struct time_stamp
{
  int status;
  int hour;
  int min;
  int sec;
} alarm; //time_stamp

alarm global_alarms[30];
int global_alarm_index=0;

volatile int global_counter=0;
volatile char global_ascii_code = 's';

void setup_interrupts();
void finilize();
void set_alarm(int hour,int min, int second);
void unset_alarm(int index);
int  convert_to_binary(int x);
void readclk(char str[]);
void init_alarms();


void unset_alarm(int index){
	global_alarms[index].status = 0;
}

void set_alarm(int hour,int min, int sec){
	global_alarms[global_alarm_index].status = 1;
	global_alarms[global_alarm_index].hour = hour;
	global_alarms[global_alarm_index].min = min;
	global_alarms[global_alarm_index].sec = sec;
	global_alarm_index++;
	global_counter++;
	global_ascii_code='n';
}

void init_alarms(){
	int i;
	for(i=0.;i<MAX_ALARMS;i++){
		global_alarms[global_alarm_index].status = 0;
		global_alarms[global_alarm_index].hour = 0;
		global_alarms[global_alarm_index].min = 0;
		global_alarms[global_alarm_index].sec = 0;
	}
}

int  convert_to_binary(int x){
 int i;
 int temp, scale, result;
 

  temp =0;
  scale = 1;
  for(i=0; i < 4; i++)
   {
     temp = temp + (x % 2)*scale;
     scale *= 2;
     x = x >> 1;
   } // for

  result = temp;
  temp = 0;

  scale = 1;
  for(i=0; i < 4; i++)
   {
     temp = temp + (x % 2)*scale;
     scale *= 2;
     x = x >> 1;
   } // for

  temp *= 10;
  result = temp + result;
  return result;

} // convert_to_binary

void readclk(char str[]){
  int i;
  int hour, min, sec, weekday, day, month, year;


  hour = min = sec = weekday = day = month = year = 0;

  asm {
   PUSH AX
   MOV AL,0	//getting seconds
   OUT 70h,AL
   IN AL,71h
   MOV BYTE PTR sec,AL
;
   MOV AL,2	//getting minutes
   OUT 70h,AL
   IN AL,71h
   MOV BYTE PTR min,AL
;
   MOV AL,4	//getting hours
   OUT 70h,AL
   IN AL,71h
   MOV BYTE PTR hour,AL
;
   MOV AL,6	//getting weekday
   OUT 70h,AL
   IN AL,71h
   MOV BYTE PTR weekday,AL
;
   MOV AL,7	//getting day (date)
   OUT 70h,AL
   IN AL,71h
   MOV BYTE PTR day,AL
;
   MOV AL,8  //getting month (date)
   OUT 70h,AL
   IN AL,71h
   MOV BYTE PTR month,AL
;
   MOV AL,9  //getting year (date)
   OUT 70h,AL
   IN AL,71h
   MOV BYTE PTR year,AL
;
   POP AX
  } // asm

	sec = convert_to_binary(sec);
	min = convert_to_binary(min);
	hour = convert_to_binary(hour);
	weekday = convert_to_binary(weekday);
	day = convert_to_binary(day);
	month = convert_to_binary(month);
	year = convert_to_binary(year);
	for(i=0;i<global_alarm_index;i++){
		if(global_alarms[i].status && global_alarms[i].hour==hour && global_alarms[i].min==min && global_alarms[i].sec==sec){
			global_ascii_code=-i;
			i=global_alarm_index;
		}
	}

	if(global_ascii_code == 'D'){
		sprintf(str,"%2d:%2d:%2d %2d.%2d.%2d", hour, min, sec, day, month, year);
	}else if (global_ascii_code == 'd'){
		sprintf(str,"%2d:%2d:%2d d=%d         ", hour, min, sec, weekday);
	}else if (global_ascii_code == 'n'){
		sprintf(str,"%2d:%2d:%2d              ", hour, min, sec);
	}else if (global_ascii_code == 'A'){
		if(global_counter>30) finilize();
		printf("\nselect alarm time(hour:minute:second):");
		scanf("%d:%d:%d",&hour,&min,&sec);
		printf("hour=%d , minute=%d, second=%d\n",hour,min,sec);
		set_alarm(hour,min,sec);
		
	}else if (global_ascii_code <= 0){
		sprintf(str ,"ALARM ALARM ALARM!!!!!            %c  ",' ');
		unset_alarm(global_ascii_code);
	}else if (global_ascii_code == 'q' || global_ascii_code == 'Q'){
		finilize();
	} else {
		sprintf(str,"%2d:%2d:%2d                  ", hour, min, sec);
	}

} // readclk

void finilize(){
	//disable_alarm();
	asm {
		mov ah,4ch
		int 21h
	}
}

void main()
{
	char str[60];
	char ascii;
	
	
	while(1)
	{
	putchar(13);
	readclk(str);
	printf(str);
	asm {
		PUSH AX
		MOV AH,1  // Check If key has been pressed
		INT 16h   //
		JNZ pressed
		JMP skip
	} // asm
	pressed:
	asm {
		MOV AH,0
		INT 16h
		MOV ascii,AL
		POP AX
	}
	if(ascii == 'D'){
		global_ascii_code=ascii;
	}else if (ascii == 'd'){
		global_ascii_code=ascii;
	}else if (ascii == 'n'){
		global_ascii_code=ascii;
	}else if (ascii == 'A'){
		global_ascii_code=ascii;
	}else if (ascii == 'q' || ascii == 'Q'){
		global_ascii_code=ascii;
	}
	
	skip:

	} // while


}  // main


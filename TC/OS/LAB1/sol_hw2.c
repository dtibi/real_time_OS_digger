// slidesh.c
#include <stdio.h>
#include <dos.h>


  int scan_code =0;

int int8_wait_time =19;
int int8_count;
void interrupt (*old_isr8)(void);
int int9_count;
void interrupt (*old_isr9)(void);
char data[80];

void interrupt int8handler(void)
{

 asm{
   PUSH AX
   MOV AL,20h
   OUT 20h,AL
   POP AX
  }
  int8_count++;

}// int8handler(void)

void interrupt int9handler(void)
{

  (*old_isr9)();
  asm {
      MOV AH,1
      INT 16h
      JZ Skip1
      MOV AH,0
      INT 16h
      MOV BYTE PTR scan_code,AH
  }

  if (scan_code == 72)
    int8_wait_time--;
  else
  if (scan_code == 80)
    int8_wait_time++;

  sprintf(data, "scan_code = %d, int8_wait_time = %d",
     scan_code, int8_wait_time,0);  

Skip1: // release


}// int9handler(void)

int *b800h;
  void write_data()
  {
    int i, k;

    for(i=0; i < strlen(data); i++)
     {
       b800h[i] = 0x6500 + data[i];        
     } // for
    for( ;i < 80; i++)
      b800h[i] = b800h[i+80];
  }

int main()
{
  int i, j=0;

  int9_count = int8_count = 0;
  old_isr9 = getvect(9);
  old_isr8 = getvect(8);
  setvect(8,int8handler);  
  setvect(9,int9handler);
  asm {
   MOV WORD PTR b800h,0
   MOV WORD PTR b800h[2],0b800h
  } // asm

  asm {
    PUSH AX
    MOV AH,0
    MOV AH,1
    INT 10h
    POP AX
   }

  asm{
           MOV AL,36H // TIMER COMMAND: SELECT CHANNEL 0;
                   //                READ / WRITE LSB-MSB
                   //                MODE 3: BINARY
        OUT 43H,AL
        MOV BX,700  // DESIRED COUNT VALUE FOR 108HZ RESULT
        MOV AL,BL                 //TRANSFER THE LSB 1ST
        OUT 40H,AL
        MOV AL,BH                 //TRANSFER THE MSB SECOND 
        OUT 40H,AL

  } // asm
  
  sprintf(data, "scan_code = %d, int8_wait_time = %d%c",
     scan_code, int8_wait_time,0);  

      for(i=0; i < 25*80; i++)
      {
       b800h[i] = 0x7300;
      }

    while(scan_code != 1)
    {
      int8_count = 0;
      for(i=0; i < 25*80; i++)
      {
      int8_count = 0;
       write_data();
       while((int8_count <=int8_wait_time) && (scan_code != 1) )
              ;
       b800h[i] = 0x133d;
      }
      for(i=0; i < 25*80; i++)
      {
       write_data();
      int8_count = 0;
       while((int8_count <=int8_wait_time) && (scan_code != 1) )
              ;
       b800h[i] = 0x233d;
      }
      int8_count = 0;
      for(i=0; i < 25*80; i++)
      {
       write_data();
      int8_count = 0;
       while((int8_count <=int8_wait_time) && (scan_code != 1) )
              ;
       b800h[i] = 0x433d;
      }
      int8_count = 0;
      for(i=0; i < 25*80; i++)
      {
       write_data();
      int8_count = 0;
       while((int8_count <=int8_wait_time) && (scan_code != 1) )
              ;
       b800h[i] = 0x7300;
      }


    } // while

  setvect(9,old_isr9);  
  setvect(8,old_isr8);  


}//main

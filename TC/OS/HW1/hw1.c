#include <dos.h>
#include <stdio.h>

volatile int counter=0,position=0,sc;
													//counter for INT8h to print 8 spaces every 3 interrupts
													//position to represent the position in byts from witch the spaces start printing
volatile int freq = 1000;				//freq
const int bluebg=30,yellowbg=110,red_on_green=44;	//	30 = 11110 , 110 = 1101110 , 44 = 101100
int startingfreq = 65356;							//frequency to start and end with with
int new108Int8PerSecFreq = 700;
int position_down=1;								//arrow going down or sideways

void interrupt(*Int9Save) (void);					//save the regular interrupt 9
void interrupt (*Int8Save) (void);					//save the regular interrupt 8
void MoveCursor(void);								//this function will move the arrow according to flags
void inputF(void);									//this function will use key input and act on it
void cleanScreen(int screen_color);					//this function cleans the screen and turns it to screen_color
void interrupt MyInt8_Handler(void);				//this is the new INT8h interrupt
void interrupt MyInt9_Handler(void);				//this is the new INT9H interrupt
void checkFreqRange();							//this will update letch acording to op
void restore(void);									//this function restores interrupt and letch also cleans the screen and returns to console
void Changefreq(int x);								//this function changes the frequency, will get 1 for up key and 0 for down
void delete_arrow(int pos);
void main(){
	Changefreq(new108Int8PerSecFreq);
	Int8Save = getvect(8);							//Saving interrupt 8
	setvect(8, MyInt8_Handler);						//setting up new interrupt 8
	Int9Save = getvect(9);							//saving interrupt 9
	setvect(9, MyInt9_Handler);						//setting up new interrupt 9
	asm{
		PUSH			AX
		MOV 			AH,0          // Select function = 'Set mode'
		MOV 			AL,1          // 40 by 25 color image
		INT 			10h           // Adapter initialized. Page 0 displayed	
		pop 			AX
		
		PUSH 			DX
		MOV             DX,3D4h  // Point TO 3D4h - 3D5h port pair
		MOV             AX,000Ah // Cursor start address (0Ah) - Value 0 (00h)
		OUT             DX,AX    // Port(3D4h) = 0Ah, Port(3D5h) = 01h
		MOV             AX,0F0Bh // Cursor end address - Value 15 (0Eh)
		OUT             DX,AX    // Port(3D4h) = 0Bh, Port(3D5h) = 0Eh
		POP 			DX
	}
	cleanScreen(bluebg);									//starts screen in texttual mode with white color
	MoveCursor();
	while(1);				//keep running until escape is pressed
	
										
}

void Changefreq(int x){
	asm{					
		MOV AL,36H 			// TIMER COMMAND: SELECT CHANNEL 0;
		OUT 43H,AL
		MOV BX,x			//x represents the amount if pulses the chip will count until he calls interrupt 8
		DEC BX
		MOV AL,BL           //TRANSFER THE LSB 1ST
		OUT 40H,AL
		MOV AL,BH           //TRANSFER THE MSB SECOND
		OUT 40H,AL
	}
}

void restore(){
	Changefreq(0);									//restore to standart frequency 18.2 / sec
	setvect(8,Int8Save);							//restoring old pointers
	setvect(9,Int9Save);
	asm{
		PUSH 			AX
		mov 			ah,0
		mov 			al,2
		int 			10h
		POP				AX
		
		MOV AH, 4ch
		int 21h
	}
}

void MoveCursor(){
	int new_pos,old_pos;
	if(!position_down)
	{
		if (position%(80)==66) {
			delete_arrow(position);
			position=0;
			position_down=1;
		}		
		else{
			asm{	
			
			MOV              AX,0B800h
			MOV              ES,AX
			MOV              DI,position          
			MOV              AL,' '        
			MOV              AH,BYTE PTR bluebg        
			MOV              CX,1    
			CLD                            
			REP              STOSW         	//mov es:[di],ax -> add di,2 (because of cld)
			
			ADD 			 position,2
			MOV              AX,0B800h   
			MOV              ES,AX         
			MOV              DI,position          
			MOV              AL,'-'        
			MOV              AH,BYTE PTR red_on_green        
			MOV              CX,5    
			CLD                            
			REP              STOSW         	//mov es:[di],ax -> add di,2 (because of cld)
			
			MOV              AX,0B800h   
			MOV              ES,AX         
			MOV              AL,'>'        
			MOV              AH,BYTE PTR red_on_green        
			MOV              CX,1    
			CLD                            
			REP              STOSW         	//mov es:[di],ax -> add di,2 (because of cld)
			}
			new_pos=position/2+6;
		}
	}
	if(position_down){
		old_pos=position-80;
		if ( position == 2000 ) { 
			position=0;
		} 
		delete_arrow(old_pos);
		
		asm{			
				
			MOV              AX,0B800h   
			MOV              ES,AX         
			MOV              DI,position          
			MOV              AL,'-'        
			MOV              AH,BYTE PTR red_on_green        
			MOV              CX,5    
			CLD                            
			REP              STOSW         	//mov es:[di],ax -> add di,2 (because of cld)
			
			MOV              AX,0B800h   
			MOV              ES,AX         
			MOV              AL,'>'        
			MOV              AH,BYTE PTR red_on_green        
			MOV              CX,1    
			CLD                            
			REP              STOSW         	//mov es:[di],ax -> add di,2 (because of cld)
		
		}

		new_pos=position/2+6;
		position=position+80;			//forward position in bytes by 16 = 8 chars ahead. 
		
	}
	asm {
		MOV 			 BX,new_pos
		MOV              DX,3D4h  // Point to 3D4H - 3D5H port pair
		MOV              AL,14    // Address of cursor register pos high byte
		MOV              AH,BH    // Get desired value of cursor pos high byte
		OUT              DX,AX    // Port(3D4h) = 14, Port(3D5h) = Value of BH
		
		MOV              AL,15    // Address of cursor register pos low byte
		MOV              AH,BL    // Get desired value pf cursor pos low byte
		OUT              DX,AX    // Port(3D4h) = 15, Port(3D5h) = Value of BL
	}
}

void inputF(void){							//this function acts accordingly to the input received from the keybord
	switch (sc) {
		case 72:										//up arrow was pressed
			checkFreqRange();				
			freq-=10;
			break;
		case 80:										//Down arrow was pressed
			checkFreqRange();				
			freq+=10;
			break;
		case 77:										//right arrow was pressed
			position_down=0;
			delete_arrow(position-80);
			break;
		case 1:
			restore();							//restore interrupts and frequency
			break;
	}
}

void checkFreqRange(){							//this will update letch acording to op
	if(freq<=1)freq = 2;
	else if (freq>1000) freq = 999;
	//Changefreq(freq);
}

void interrupt MyInt8_Handler(void){				    //creating our own handler for interrupt 8, using th eexisting one
	Int8Save();
	counter++;										//advancing our counter every int 8h
	if(counter>=freq){
		counter=0;
		MoveCursor();
	}
}

void interrupt MyInt9_Handler(void){					//making sure to call the function only when the button was released
	Int9Save();						//calling the original interrupt rutine to take care of ending our new one
	asm{
		push ax										//making sure we won't change the AX register
		in al, 60h									 //input
		mov BYTE PTR sc, al									 //scan code	
		pop ax
	}
	inputF();										//calling the input function
}//using interrupt 9 being called automatically for our advantage.

void cleanScreen(int screen_color){
	asm {
		
	MOV              AX,0B800h   
	MOV              ES,AX         
	MOV              DI,0          
	MOV              AL,' '        
	MOV              AH,BYTE PTR screen_color        
	MOV              CX,1000       
	CLD                            
	REP              STOSW    
	
	}
}

void delete_arrow(int pos){
	asm{		
			PUSH AX
			PUSH ES
			PUSH DI
			PUSH CX
			MOV              AX,0B800h   
			MOV              ES,AX         
			MOV              DI,pos          
			MOV              AL,' '        
			MOV              AH,BYTE PTR bluebg        
			MOV              CX,6    
			CLD                            
			REP              STOSW         	//mov es:[di],ax -> add di,2 (because of cld)	
			POP CX
			POP DI
			POP ES
			POP AX
	}
}
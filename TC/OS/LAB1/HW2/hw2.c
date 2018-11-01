#include <dos.h>
#include <stdio.h>

volatile int p_flag=0,counter=0,position=0;			//p_flag represents Pause/Play, 
													//counter for INT8h to print 8 spaces every 3 interrupts
													//position to represent the position in byts from witch the spaces start printing
volatile unsigned int freq,range=1;					//freq
volatile char sc,color=114;							// 114 = 01110010b = white ; 47 = 00101110b = green
int startingfreq = 65356;							//frequency to start and end with with

void interrupt(*Int9Save) (void);					//save the regular interrupt 9
void interrupt (*Int8Save) (void);					//save the regular interrupt 8
void coloreight(void);								//this function will color 8 spaces on the screen
void inputF(void);									//this function will use key input and act on it
void startscreen(void);								//this function start by coloring the screen and gets rid of the cursor
void interrupt MyInt8_Handler(void);				//this is the new INT8h interrupt
void interrupt MyInt9_Handler(void);				//this is the new INT9H interrupt
void updateLetch(int op);							//this will update letch acording to op
void restore(void);									//this function restores interrupt and letch
void Changefreq(int x);								//this function changes the frequency, will get 1 for up key and 0 for down

int main()
{
	int i;
	Changefreq(startingfreq);						//this will change int8h frequency 
	Int8Save = getvect(8);							//Saving interrupt 8
	setvect(8, MyInt8_Handler);						//setting up new interrupt 8
	Int9Save = getvect(9);							//saving interrupt 9
	setvect(9, MyInt9_Handler);						//setting up new interrupt 9
	startscreen();									//starts screen in texttual mode with white color
	color=47;										//update color to green	
	
	while(sc!=1);									//keep running until escape is pressed
	
	restore();										//restore interrupts and frequency
	return (1);
}

void Changefreq(int x)
{
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

void restore()
{
	Changefreq(0);									//restore to standart frequency 18.2 / sec
	
	setvect(8,Int8Save);							//restoring old pointers
	setvect(9,Int9Save);							
}

void coloreight()
{
	if(position>=2000)		//check if the position in bytes passed 2000 
	{
		position=0;
		(color==114) ? (color=47) : (color=114);
	}
	asm{	
	
	MOV              AX,0B800h   
	MOV              ES,AX         
	MOV              DI,position          
	MOV              AL,' '        
	MOV              AH,color        
	MOV              CX,8    
	CLD                            
	REP              STOSW         	//mov es:[di],ax -> add di,2 (because of cld)
	}
	position=position+16;			//forward position in bytes by 16 = 8 chars ahead. 
}

void inputF(void)								//this function acts accordingly to the input received from the keybord
{

	switch (sc) {
	case 72:										//up arrow was pressed
		if(p_flag==0)updateLetch(1);				//dont make changes if paused 
		break;
	case 80:										//Down arrow was pressed
		if(p_flag==0)updateLetch(0);				//dont make changes if paused
		break;
	case 25:										//Pause/Play (p) was pressed
	(p_flag==0) ? (p_flag=1) : (p_flag=0);
		break;
	}
}

void updateLetch(int op)							//this will update letch acording to op
{
	int flag=0;
	if(op==1 && range<18)							//op=1 will increase range divider and will make letch smaller
	{
		range++;
		flag=1;
	}
	else if (op==0 && range>1)						//op=0 will decrease range divider and will make letch bigger
	{
		range--;
		flag=1;
	}
	if(flag==1)
	{
		freq=startingfreq/range;
		Changefreq(freq);
	}
}
void interrupt MyInt8_Handler(void)				    //creating our own handler for interrupt 8, using th eexisting one
{
	asm{
		pushf										//pushing the flag register because of the IRET function
		call dword ptr Int8Save
	}
	if(p_flag==0)									//when pause flag pressed -> stop counting and stop coloring
	{
	counter++;										//advancing our counter every int 8h
	if(counter==3)
	{
		counter=0;
		coloreight();
	}
	}
}

void interrupt MyInt9_Handler(void)
{													//making sure to call the function only when the button was released
	asm{
		push ax										//making sure we won't change the AX register
		in al, 60h									 //input
		mov sc, al									 //scan code
		pushf
		call dword ptr Int9Save						//calling the original interrupt rutine to take care of ending our new one
		pop ax
	}
	inputF();										//calling the input function
}//using interrupt 9 being called automatically for our advantage.

void startscreen()
{
	asm{
	MOV AH,0          // Select function = 'Set mode'
	MOV AL,1          // 40 by 25 color image
	INT 10h           // Adapter initialized. Page 0 displayed	
	
	MOV              AX,0B800h   
	MOV              ES,AX         
	MOV              DI,0          
	MOV              AL,' '        
	MOV              AH,color        
	MOV              CX,1000       
	CLD                            
	REP              STOSW    
	
	
	MOV BX,3E8h  //just outside of the screen 3E8h = 1000 position will be 1+1000
	MOV DX,3D4H  // Point to 3D4H - 3D5H port pair
	MOV AL,14    // Address of cursor register pos high byte
	MOV AH,BH    // Get desired value of cursor pos high byte
	OUT DX,AX    // Port(3D4h) = 14, Port(3D5h) = Value of BH
	
	MOV AL,15    // Address of cursor register pos low byte
	MOV AH,BL    // Get desired value pf cursor pos low byte
	OUT DX,AX    // Port(3D4h) = 15, Port(3D5h) = Value of BL	
	}
}
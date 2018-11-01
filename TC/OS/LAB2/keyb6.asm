; 
; keyb6.asm
; Example of custom keyboard support software
;
Stack1   SEGMENT PARA STACK 'STACK'
	DB 256 DUP(?)               ; 256d bytes of stack space
Stack1   ENDS
;
Data    SEGMENT  PARA PUBLIC 'Data'
Buffer  DB  ?               ; 1 byte keyboard buffer
Buff_Flag DB ?               ; 0 - Buffer empty, 1 - Char in
                            ; Buffer
Msg3 DB ' PRESS ANY KEY AND YOU WILL SEE IT ON THE'
     DB ' SCREEN (PRESS ESC TO QUIT)',10,13,'$'
Oldbios_Off DW 0
Oldbios_Seg DW 0

; ScanTable converts scan codes received from the keyboard
; into their corresponding ascii character codes:
;
ScanTable DB 0,1,'1234567890-=',8,0
	  DB 'QWERTYUIOP[]',0Dh,0
	  DB 'ASDFGHJKL;',0,0,0,0
	  DB 'ZXCVBNM,./',0,0,0
	  DB ' ',0,0,0,0,0,0,0,0,0,0,0,0,0
	  DB '789-456+1230.'
Data      ENDS
;
Code      SEGMENT PARA PUBLIC 'Code'
Start     PROC FAR
;
;  Standard program prologue
;
    ASSUME CS:Code
      PUSH DS      ; Save PSP  SED addr
      MOV  AX,0
      PUSH AX      ; Save RET addr offset (PSP+0)
      MOV  AX,Data
      MOV  DS,AX   ; Establish Data SEG addressability
    ASSUME DS:Data
;
;  Part1: Setup our own keyboard interrupt service routine
;
      MOV AH,09
      MOV DX,OFFSET Msg3
      INT 21h
;
;=====================================================================
;                                       ###                         ||
;                                        #                          ||
;                                        #                          ||
      CLI          ;                    ###                         ||
      MOV AX,0     ;                                                ||
      MOV ES,AX    ; Point extra segment at the ...                 ||
;                    ... interrupt service routine address table    ||
      MOV DI,36   ; Offset of entry for type code 09h               ||
      MOV AX,ES:[DI]        ;save old bios offset                   ||
      MOV Oldbios_Off,AX    ;in Oldbios_Off                         ||
      MOV WORD PTR ES:[DI],OFFSET Kbint  ;                          || 
                                ; Offset of our service rout        ||
      MOV AX,ES:[DI+2]       ;save old bios segment                 ||
      MOV Oldbios_Seg,AX   ;in Oldbios_Seg                          ||
      MOV  ES:[DI+2],CS    ; SEG of our service routine             ||
      STI                  ; Enable interrupts to the CPU           ||
;                                                                   ||
;=====================================================================
;  Part2: Read from keyboard and display chars on screen
;
Forever:
       CALL Kbget          ; Wait for a character from the keyboard
       CMP AL,01           ; check for "esc"=quit =01
       JZ Quit
       PUSH AX             ; Save the character
       CALL DispChar       ; Display the character received
       POP  AX             ; Restore the character
       CMP  AL,0Dh         ; Was it carrige return?
       JNZ  Forever        ; Repeat loop if not
       MOV  AL,0Ah         ; Yes it was, we must also display ...
       CALL DispChar       ;  ... aline feed.
       JMP  Forever        ; Stay in this loop forever.
;
;======================================================================
;                                   ### ###                          ||
;                                    #   #                           ||
;                                    #   #                           ||
;                                   ### ###                          ||
Quit:                      ;                                         ||
       CLI                 ;disable interrupt                        ||
       MOV AX,0            ;restore the old bios keyboard routine    ||
       MOV ES,AX           ;                                         ||
       MOV DI,36           ;                                         ||
       MOV AX,Oldbios_Off  ;                                         ||
       MOV ES:[DI],AX      ;                                         ||
       MOV AX,Oldbios_Seg  ;                                         ||
       MOV ES:[DI+2],AX    ;                                         ||
       STI                 ;                                         ||
;                                                                    ||
;  Return to DOS                                                     ||
;                                                                    ||
       RET                 ;                                         ||
;                                                                    ||
;======================================================================
;
;
;
; Call Kbget to wait for a character to be received from
;   the keyboard.  The character is returned in reg AL.
;
Kbget  PROC   NEAR
       CLI               ; Disable interrupts
       CMP  Buff_Flag,0  ; Is buffer empty?
       JNZ  Kbget2       ; ->No
       STI               ; Re-enable interrupts
       JMP  Kbget        ; Wait until something in buffer
;  There is something in the buffer, get it:
Kbget2:
       MOV AL,Buffer       ; Get char at buffer start
       MOV Buff_Flag,0     ; Signal Buffer empty
       STI                 ; Re-enable interrupts
       RET                 ; Return from Kbget
Kbget  ENDP
;
;  Kbint is our own interrupt seviice routine
;
Kbint  PROC  FAR
       PUSH  DS  ; save all altered registers
       PUSH  BX
       PUSH  AX
;
;  Establish addresability of our data segment:
;
       MOV AX,Data
       MOV DS,AX
;
;===================================================================
;                            ###  ###  ###                        ||
;                             #    #    #                         ||
;                             #    #    #                         ||
;                            ###  ###  ###                        ||
;                                                                 ||
; Read the keyboard data and send acknowlege signal               ||
;                                                                 ||
       IN   AL,60h   ; Read keyboard input                        ||
       PUSH AX       ; Save keyboard input                        ||
       IN   AL,61h   ; Read 8255 port pb                          ||
       OR   AL,80h   ; Set keyboard acknowlege signal             ||
       OUT  61h,AL   ; Send keyboard acknowlege signal            ||
       AND  AL,7Fh   ; Reset keyboard acknowlege signal           ||
       OUT  61h,AL   ; Restore original 8255 port pb              ||
;                                                                 ||
;===================================================================
;
;=====================================================================
;                              ###  #     #                          ||
;                               #    #   #                           ||
;                               #     # #                            ||
;                              ###     #                             ||
;                                                                    ||
; Decode the scan code received:                                     ||
;                                                                    ||
       POP   AX                   ; Regain the keyboard input (AL)   ||
       TEST  AL,80h               ; Is it a key being released       ||
       JNZ   Kbint2               ; Branch if yes, we ignore these   ||
       MOV   BX,OFFSET ScanTable  ; Scan code - ASCII table          ||
       XLATB                      ; Convert the scan code to         ||
                                  ;  an ASCII char                   ||
       CMP   AL,0                 ; Is it a valid ASCII key          ||
       JZ    Kbint2               ; Branch if not                    ||
;                                                                    ||
; Place the ASCII character into the buffer:                         ||
;                                                                    ||
       MOV   Buffer,AL            ; Place char in buffer             ||
       MOV   Buff_Flag,1          ; Signal char in buffer            ||
;                                                                    ||
;======================================================================
;
;
;======================================================================
;                                #     #                             ||
;                                 #   #                              ||
;                                  # #                               ||
;                                   #                                ||
;                                                                    ||
; Now indicate "END OF INTERRUPT" to the interrupt controller:       ||
;                                                                    ||
Kbint2:                            ;                                 ||
       MOV   AL,20h                ; Send "EOI" command ...          ||
       OUT   20h,AL                ; ... to  8259 command register   ||
;                                                                    ||
;======================================================================
;
       POP   AX                    ; Restore all altered registers
       POP   BX
       POP   DS
       IRET                        ; Return from interrupt
Kbint  ENDP
;
; Subroutine to display a character on the screen.
; Enter with AL = character to be displayed
; Uses video interface in BIOS
;
DispChar  PROC  NEAR
          PUSH  BX     ; Save BX register
          MOV   BX,0   ; Select display page 0
          MOV   AH,14  ; Function code for write
          INT   10h    ; Call video driver in BIOS
          POP   BX     ; Restore BX register
          RET          ; Return to caller of 'DispChar'
DispChar  ENDP
;
Start     ENDP
Code      ENDS
     END  Start

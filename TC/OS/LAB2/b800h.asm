;
; B800h.ASM
; This program demonstrates the use of hardware scrolling.
;
Stack              SEGMENT PARA STACK 'STACK'
                   DB     256 DUP(0)
Stack              ENDS
;
Data               SEGMENT PARA PUBLIC 'Data'
Count              DB      0     ; Number of lines scrollled down
Base               DW      0
;
Direction          DB      0     ; The scroll direction
;                                  0=Down    1=Up
Data               ENDS
Code               SEGMENT PARA PUBLIC 'Code'
Start              PROC    FAR
;
;Standard program prologue
;
  ASSUME           CS:Code
  PUSH             DS          ; Save PSP segment address
  MOV              AX,0
  PUSH             AX          ; Save RET address offset (PSP+0)
  MOV              AX,Data
  MOV              DS,AX       ; Establish Data segment addressability
  ASSUME           DS:Data
;
;Part1 : Initialize the display adapter
;
  MOV              AH,0          ; Select function = 'SET MODE'
  MOV              AL,1          ; 40 BY 25 Color image
  INT              10H           ; Adapter initilizedD. Page 0 displayed
;
  MOV              AX,0B800H     ; Segment address of memory of color adapter
                                 ; in text mode
  MOV              ES,AX         ; Set up extra segment register
  MOV              DI,0          ; Initial offset address into segment
  MOV              AL,'A'        ; Character A to fill adapter memory
  MOV              AH,03H        ; Attribute byte: BLUE + GREEN = LIGHT BLUE
;
  MOV              CX,2000
  CLD
  REP              STOSW
;
  MOV              AL,'B'        ; Character B to fill adapter memoory
  MOV              AH,05H        ; Attribute byte: BLUE + RED = PURPLE
;
  MOV              CX,2000
  CLD
  REP              STOSW
;
  MOV              AL,'C'        ; Character C to fill adapter memory
  MOV              AH,06H        ; Attribute byte: GREEN + RED = BROWN
;
  MOV              CX,2000
  CLD
  REP              STOSW
;
  MOV              AL,'D'        ; Character D to fill adapter memory
  MOV              AH,07H        ; Attribute byte: GREEN + RED + BLUE = WHITE
;
  MOV              CX,2000
  CLD
  REP              STOSW
;
;
; Set the cursor address registers
;
  MOV              BX,40*63      ; Cursor position: 40th row, Col 0
  MOV              DX,3D4h       ; 3D4h - 3D5h: Display adapter ports
  MOV              AL,14         ; 14 - Cursor address high byte register
  MOV              AH,BH         ; Load AH with desired high byte value
  OUT              DX,AX         ; Output AL to port 3D4h, AH to port 3D5h
;
  MOV              DX,3D4h       ; 3D4h - 3D5h: Display adapter ports
  MOV              AL,15         ; 15 - Cursor address low byte register
  MOV              AH,BL         ; Load AH with desired low byte value
  OUT              DX,AX         ; Output AL to port 3D4h, AH to port 3D5h
;
;
;PART 2 : Scroll the display every second until a key is hit
;
Delay:
  ; Delay AL*CX operations
  MOV               AL,10
Tenth:
  MOV               CX,28000
Dloop:
  LOOP             Dloop
  DEC              AL
  CMP              AL,0
  JNE              Tenth
;
  MOV              AH,1         ; User pressed key?
  INT              16H
  JZ               Scroll       ; No, scroll
  MOV              AH,0         ; Yes, read the key
  INT              16H
  CMP              AH,1         ; Is it Esc?
  JE               ToReturn     ; Yes - return to DOS
  MOV              AH,0         ; No: Wait for another key
  INT              16H
  CMP              AH,1         ; Was it Esc?
  JE               ToReturn     ; Yes - return to DOS
;                               ; No - Continue
Scroll:
  MOV              BX,Base      ; Retrieve present location
  CMP              Direction,0  ; Backwards or Forwards?
  JNE              Backwards    ; Direction = 1: Backwards
;                                 Direction = 0: Forwards
  INC              Count        ; Advance 1 line
  CMP              Count,160    ; End of 4 pages?
  JB               Down_Ok      ; No
;
  MOV              Direction,1  ; Yes, reverse direction
Down_Ok:
  ADD              BX,40        ; No: Advance 1 line
  MOV              Base,BX      ; Store location
;
  JMP              SHORT  Update
;
Backwards:
  DEC              Count        ; Backtack 1 line
  CMP              Count,0      ; Reached first line?
  JNE              Up_Ok        ; No
;
  MOV              Direction,0  ; Yes, reverse direction
Up_Ok:
  SUB              BX,40        ; Backtack 1 line
  MOV              Base,BX      ; Store location
;
Update:
  MOV              DX,3D4h      ; 3D4h - 3D5h: Display adapter ports
  MOV              AL,12        ; Display position high byte register
  MOV              AH,BH        ; Move desired high byte value to AH
  OUT              DX,AX        ; Output AL to port 3D4h, AH to port 3D5h
  MOV              DX,3D4h      ; 3D4h - 3D5h: Display adapter ports
  MOV              AL,13        ; Display position low byte register
  MOV              AH,BL        ; Move desired low byte value to AH
  OUT              DX,AX        ; Output AL to port 3D4h, AH to port 3D5h
;
  JMP              Delay           ; Repeat the process
;
ToReturn:
                   MOV  AX,3   ; Restore adapter mode
                   INT  10h
                   RET         ; Return to DOS
Start              ENDP
Code               ENDS
  END              Start


;
; Cursor2.asm
;
;This program demonstrates cursor manipulation.
;
;
Stak              SEGMENT PARA STACK 'STACK'
DB                 256 DUP(0)
Stak              ENDS
;
Data               SEGMENT PARA PUBLIC 'Data'
CursorPos          DW      0     ;Current position of cursor
OldCursorPos       DW      0     ;last possition of the cursor
Two				   DW      2     ;the number 2
FFnum			   DW      255
Attribute          DB      0     ;Attribute byte

;
Data               ENDS
Code               SEGMENT PARA PUBLIC 'Code'
                   .386  ; Enable 386 commands
SetCursorPos       PROC    NEAR
; Set Cursor to position in BX
; Input: BX
; Output: None
;
  MOV              DX,3D4h  ; Point to 3D4H - 3D5H port pair
  MOV              AL,14    ; Address of cursor register pos high byte
  MOV              AH,BH    ; Get desired value of cursor pos high byte
  OUT              DX,AX    ; Port(3D4h) = 14, Port(3D5h) = Value of BH
;
  MOV              AL,15    ; Address of cursor register pos low byte
  MOV              AH,BL    ; Get desired value pf cursor pos low byte
  OUT              DX,AX    ; Port(3D4h) = 15, Port(3D5h) = Value of BL
;
  RET                       ; Return to caller
SetCursorPos          ENDP
;
Start              PROC    FAR
;
;STANDARD PROGRAM PROLOGUE
;
  ASSUME           CS:Code
  PUSH             DS          ; Save PSP segment address
  MOV              AX,0
  PUSH             AX          ; Save INT 20h address offset (PSP+0)
  MOV              AX,Data
  MOV              DS,AX       ; Establish Data segment addressability
  ASSUME           DS:Data
;
;Part1 : Initialize the display adapter
;
  MOV              AH,0          ; Select function = 'Set mode'
  MOV              AL,1          ; 40 by 25 color image
  INT              10h           ; Adapter initialized. Page 0 displayed
;
  MOV              AX,0B800h     ; Segment address of memory on color adapter
;
  MOV              ES,AX         ; Set up extra segment register
  MOV              DI,0          ; Initial offset address into segment
  MOV              AL,' '        ; Character space to fill adapter memory
  MOV              AH,0Ah        ; Attribute byte : Intense yellow
  MOV              CX,1000       ; Initialize count, 1 Screen
  CLD                            ; Write forward
  REP              STOSW         ; Write
  
;
; Set the cursor address registers
;
  MOV              BX,12*40+20
  CALL             SetCursorPos
;
;PART 2 : Wait for key strike
;
NextLoop:
  MOV              AH,0       ; Wait and read key
  INT              16h 
  
  CMP              AH,1       ; Is it Esc?
  JE               ToReturn   ; Yes - Return to DOS
  
  CMP              AH,75       ; Is it left?
  JE               ToLeft  ; Yes - Return to DOS
  
  CMP              AH,77       ; Is it right?
  JE               ToRight   ; Yes - Return to DOS
  
  CMP              AH,80       ; Is it down?
  JE               ToDown   ; Yes - Return to DOS
  
  CMP              AH,72       ; Is it up?
  JE               ToUp   ; Yes - Return to DOS
  
  CMP              AH,59       ; Is it f1?
  JE               ToF1   ; Yes - Return to DOS
  
  CMP              AH,60       ; Is it f2?
  JE               ToF2   ; Yes - Return to DOS
  
  CMP              AH,61       ; Is it f3?
  JE               ToF3   ; Yes - Return to DOS
  
  CMP              AH,62       ; Is it f4?
  JE               ToF4   ; Yes - Return to DOS
  
  CMP              AL,'z'       ; Is it z?
  JE               ToZ   ; Yes - Return to DOS
  
  CMP              AL,'x'       ; Is it x?
  JE               ToX   ; Yes - Return to DOS
  
  CMP              AL, 0       ; does the key have asci code?
  JNE              ToHaveAsci   ; Yes - Return to DOS
  
  JMP              NextLoop   ; Repeat main loop
	
;

ToLeft:
	mov ax, bx
	inc ax
	mov cl, 40
	div cl
	cmp ah, 1
	je NextLoop ;the most left position
	dec bx
	mov CursorPos,BX
	CALL             SetCursorPos
	jmp NextLoop
	
ToRight:
	mov ax, bx
	inc ax
	mov cl, 40
	div cl
	cmp ah, 0
	je NextLoop ;the most right position
	inc bx
	mov CursorPos,BX
	CALL             SetCursorPos
	jmp NextLoop
	
ToUp:
	cmp bx, 40
	jl NextLoop
	sub bx, 40
	mov CursorPos,BX
	CALL SetCursorPos
	jmp NextLoop

ToDown:
	cmp bx, 960
	jge NextLoop
	add              bx, 40
	mov CursorPos,BX
	CALL             SetCursorPos
	jmp NextLoop
	
ToF1:
	
	mov AX , CursorPos
	mul Two
	mov bx , ax
	MOV AX,word PTR ES:[bx]
	mov cx,ax
	mov al,ah
	mov ah,0
	inc al
	mov ch,al
	MOV word PTR ES:[bx], CX
	MOV BX,CursorPos

	jmp NextLoop
	
ToF2:
	
	mov AX , CursorPos
	mul Two
	mov bx , ax
	MOV AX , word PTR ES:[bx]
	mov al , ah
	MOV word PTR ES:[bx] , AX
	MOV BX , CursorPos
	
	jmp NextLoop
	
ToF3:
	MOV             DX,3D4h  ; Point TO 3D4h - 3D5h port pair
    MOV             AX,000Ah ; Cursor start address (0Ah) - Value 0 (00h)
    OUT             DX,AX    ; Port(3D4h) = 0Ah, Port(3D5h) = 01h
    MOV             AX,0F0Bh ; Cursor end address - Value 15 (0Eh)
    OUT             DX,AX    ; Port(3D4h) = 0Bh, Port(3D5h) = 0Eh
	jmp NextLoop
	
ToF4:
	MOV             DX,3D4h   ; Point to 3D4H - 3D5H port pair
    MOV             AX,0E0Ah  ; Cursor start address (0Ah) - Value 14 (0Dh)
    OUT             DX,AX     ; Port(3D4h) = 0Ah, Port(3D5h) = 01h
    MOV             AX,0F0Bh  ; Cursor end address - Value 15 (0Eh)
    OUT             DX,AX     ; Port(3D4h) = 0Bh, Port(3D5h) = 0Eh
	jmp NextLoop

ToZ:
	mov OldCursorPos, bx
	MOV BX,999
	MOV CursorPos , BX
	CALL SetCursorPos
	jmp NextLoop
	
ToX:
	mov bx, OldCursorPos
	CALL SetCursorPos
	jmp NextLoop
	
ToHaveAsci:
	mov cl, al ;save the letter al cl
	mov CursorPos, bx
	mov ax, bx
	mul Two
	mov bx, ax
	MOV BYTE PTR ES:[bx], cl
	mov bx, CursorPos
	jmp ToDown

ToReturn:
                   MOV  AX,2
                   INT  10h
                   RET
Start              ENDP
Code               ENDS
  END              Start

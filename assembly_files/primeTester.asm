ADDI 12, 0, 2011; Holds prim val
ADDI 30, 0, 1	; starts at one
ADDI  2, 0, 1	; Holds value of true
; START	
	ADDI 6, 12, -1	; R6 = R12-1, 
	ADDI 13, 12, 0	; R13 = R12 primtalet som ska testas

	ADDI 30, 30, 1	; R30 är id i for loopen
	ADDI 14, 30, 0	; R14 = R30
	JALR 1,  0, 52  ; att ändra
	BEQ 10, 2, 8	; Är delbart? flyg till Y
	BLTU 30, 6, -8	; att ändra
	ADDI 10, 0, 1	; R10 = true
	JAL  1, 24		; END PROGRAM
	JAL  1, 20		; SET OUTPUT FALSE AND END PROGRAM

; IS_DIV? (R13%R14==0):
	ADDI 10, 0, 0	; R10 = 0 // output reg
	ADDI 28, 0, 0	; R28 = 0 // adds up to val 20
	ADDI 29, 0, 0	; R29 = 0 // is max val
	ADD  28, 28, 14	; R28 += R14 
	BLTU 28, 13, -2 ; Goes back one if R28 < R13
	BNE 28, 13, 4
	ADDI 10, 0, 1	; Go here to add
	ADDI 10, 10, 0	; Go heree not to add
	JALR 31, 1, 0	; Go back to where it was called from

; SET OUTPUT FALSE:
	ADDI 10, 0, 0	; R10 = 0 

; EOF
	ADDI 0, 0, 0	; EOF, jump here to end program
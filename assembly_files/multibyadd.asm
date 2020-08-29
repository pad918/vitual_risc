; R10 is the return value
; R12 and R13 are function args

; START:
	ADDI 12, 0, 769	; Funktionsargument 1
	ADDI 13, 0, 1678; Funktionsargument 2
	ADDI 6,  0, 0	; index in loop

; For loop:
	ADD  10, 10, 12	; R10 += R12	// output += arg1
	ADDI 6, 6, 1	; R6++			//i++
	BLTU 6, 13, -4	; Goto For loop if R6 < R13

; EOF
ADDI 1, 0, 100		; R1 = 100
JALR 1, 1, 2		; Jump to 100 + 0
;FÖLJANDE ÄR BENCHEEN:
;ADDI 6, 0, 2000		; R6 = 2000
;ADDI 7, 7, 1		; R7++
;ADDI 5, 5, 2000		; R5 += 2000
;BLTU 7, 6, -4;		; Go back if R7 < R6
;ADDI 1, 1, 1		; R1 ++
;BLTU 1, 5, -2		; Go back if R1 < R5
; Följande är fibben
;ADDI 5, 0, 40	; R5 = 40
;ADDI 1, 0, 1 	; R1 = 1
;ADDI 2, 0, 0 	; R2 = 0
;ADDI 3, 0, 0 	; R3 = 0
;ADDI 4, 0, 1 	; R4 = 1 
;ADD 3, 1, 2		; R3 = R1 + R2 // FOR
;ADD 2, 0, 1		; R2 = R1 
;ADD 1, 0, 3		; R1 = R3
;ADDI 4, 4, 1		; R4 += 1
;BLT 4, 5, -8		; Goes to FOR if R4 < R5;

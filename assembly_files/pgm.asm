ADDI 1, 0, 1000		; R1 = 1000 // Är största nummret
ADDI 2, 0, 3		; R4 är summan av 3
ADDI 3, 0, 5		; R5 är summan av 5
; LOOP3:
ADD  4, 4, 2			; R4 += R2
ADDI 2, 2, 3		; R2 += 3
BLTU 2, 1, -4		; Go to LOOP3 if R2 < R1
; LOOP5
ADD  5, 5, 3			; R5 += R3
ADDI 3, 3, 5		; R3 += 5
BLTU 3, 1, -4		; Go to LOOP5 if R3 < R1
ADD  31, 4, 5		; R31 = R4 + R5
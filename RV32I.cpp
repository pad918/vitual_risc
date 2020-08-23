#include "RV32I.h"

RISC::RV32I::RV32I()
{
	pc.reg = 0;
	memset(&mem, 0, sizeof(mem));
	//En ADDI
	mem[0] =	0b01001000; //rs1 = 1160
	mem[1] =	0b10000000;
	mem[2] =	0b00001110;
	mem[3] =	0b00010011;

	mem[4] =	0b11111000; //rs1 -= 128
	mem[5] =	0b00001110;
	mem[6] =	0b00001110;
	mem[7] =	0b00010011;

	mem[8] =	0b00000000; //SLTIU
	mem[9] =	0b00011110;
	mem[10] =	0b10111110;
	mem[11] =	0b10010011;
}

void RISC::RV32I::step()
{
	//Läs in hel instuktion från ram:
	RISC::Reg32 inst;
	for (int i = 3; i >= 0; i--)
		inst.reg += mem[pc.reg + (3 - i)] << (i * 8);
	deCoder.deCodeInstruction(inst.reg, mem);
	pc.reg += 4; // Alla intruktioneeer är 8 byte (32bit) långa.
}


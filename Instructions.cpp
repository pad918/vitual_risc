#include "Instructions.h"
#include <iostream>
/*
BRA RESURSERE:

	https://inst.eecs.berkeley.edu/~cs61c/resources/su18_lec/Lecture7.pdf

	officiella RISC-V PDF:n

*/


//Op code definitions:
#define OP_IMM	0b0010011
//Funct3 definitions:
#define SLLI	0b001 
#define SRLI	0b101
#define SRAI	0b101
#define ADDI	0b000
//#define ADDI 0
#define SLTI	0b010
#define SLTIU	0b011
#define XORI	0b100
#define ORI		0b110
#define ANDI	0b111
//Instruction functions (Kommer bli många!)
#define ADDI_	xi[rd].reg =	xi[rs1].reg + imm12;					//Klar
#define SLTI_	xi[rd].reg =	(signed int)xi[rs1].reg < imm12;		//Klar typ
#define SLTIU_  xi[rd].reg =	xi[rs1].reg < (unsigned int) imm12		//Klar typ
#define XORI_	xi[rd].reg =	xi[rs1].reg ^ imm12;					//Inte testad
#define ORI_	xi[rd].reg =	xi[rs1].reg | imm12;
#define ANDI_	xi[rd].reg =	xi[rs1].reg & imm12;

RISC::Reg32::Reg32()
{
	reg = 0;
}

int RISC::Instruction::unSignedToSigned(uint32_t input, size_t size) //Borde förbättras
{
	int output = std::abs((int)input); size *= 8;
	uint32_t tmp = input >> (size - 1);
	if (tmp) {
		output -= 1 << (size-1);
	}
	return output;
}

uint32_t RISC::Instruction::signExtend(uint32_t input, unsigned char size)
{
	uint32_t output = input;
	if (input >> (size - 1)) {
		//is negative
		uint32_t max = 0b11111111111111111111111111111111;
		max -= (2 << size-1)-1;
		output = input | max;
	}
	return output;
}

void RISC::Instruction::preCalculate(uint32_t inst)
{
	opCode = 0b00000000000000000000000001111111 & inst; // Nice kod BTW
	funct3 = (0b00000000000000000111000000000000 & inst) >> 12;
}

void RISC::Instruction::deCodeItype(uint32_t inst)
{
	funct3 = (0b00000000000000000111000000000000 & inst) >> 12;
	rd = (0b111110000000 & inst) >> 7; //KLAR
	rs1 = (0b11111000000000000000 & inst) >> 15;
	imm12 = (0b11111111111100000000000000000000 & inst) >> 20;
	imm12 = signExtend(imm12, 12);
}

RISC::Instruction::Instruction()
{
	for (auto tmp : xi) // Sätter alla x registeer till 0
		tmp.reg = 0;
}

void RISC::Instruction::deCodeInstruction(uint32_t inst, std::array<uint8_t, 1024> &mem)
{
		preCalculate(inst);
	//Instructions
	if (opCode == OP_IMM) {
		if (funct3 != SLLI && funct3 != SRLI && funct3 != SRAI) {
			//Alla dessa är av typ I
			deCodeItype(inst);
			switch (funct3) {
			case ADDI:	ADDI_;	break;
			case SLTI:	SLTI_;	break;
			case SLTIU:	SLTIU_;	break;
			case XORI:	XORI_;	break;
			case ORI:	ORI_;	break;
			case ANDI:	ANDI_;	break;
			default:
				std::cout << "funct är negativt eller för högt...\n";
				break;
			}
		}
	}
}
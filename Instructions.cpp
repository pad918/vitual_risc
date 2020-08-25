#include "Instructions.h"
#include <iostream>
/*
BRA RESURSERE:

	https://inst.eecs.berkeley.edu/~cs61c/resources/su18_lec/Lecture7.pdf

	officiella RISC-V PDF:n

*/

//Op code definitions:
#define OP			0b0110011
#define OP_IMM		0b0010011
#define BRANCH		0b1100011 
#define LUI			0b0110111 
#define AUIPC		0b0010111 
#define JAL			0b1101111 
#define JALR		0b1100111 
#define LOAD		0b0000011 
#define STORE		0b0100011 
#define MISC_MEM	0b0001111 

//Funct3 definitions:		/**************************************TWO REGISTER MATH*********************************************/
#define ADD			0b000	/*																									*/
#define SUB			0b000	/*																									*/
#define SLL			0b001	/*																									*/
#define SLT			0b010	/*																									*/
#define SLTU		0b011	/*																									*/
#define XOR			0b100	/*	LOGIC XOR																						*/
#define SRL			0b101	/*	LOGIC LEFT SHIFT																				*/
#define SRA			0b101	/*	ARITHMETRIC RIGHT SHIFT																			*/
#define	OR			0b110	/*	LOGIC OR																						*/
#define AND			0b111	/*	LOGIC AND																						*/
							/***********************************REGISTER IMMIDIATE MATH******************************************/
#define SLLI		0b001 	/*	LEEEFT SHIFT																					*/
#define SRLI		0b101	/*	RIGHT SHIFT																						*/
#define SRAI		0b101 	/*	ARITHMETRIC RIGHT SHIFT 																		*/
#define ADDI		0b000	/*	ADDS RS1 REGISTER AND IMMIDIATE AND SETS DESTINATION REGISTER TO THE SUM						*/
#define SLTI		0b010	/*																									*/
#define SLTIU		0b011	/*																									*/
#define XORI		0b100	/*																									*/
#define ORI			0b110	/*																									*/
#define ANDI		0b111	/*																									*/
							
//Instruction functions (Kommer bli många!)
#define ADD_	xi[rd].reg =	xi[rs1].reg + xi[rs2].reg;								//Testad - fungerar
#define SUB_F	xi[rd].reg =	xi[rs1].reg - xi[rs2].reg;								//Testad - fungerar
#define SLT_	xi[rd].reg =	(signed long)xi[rs1].reg < (signed long)xi[rs2].reg;	//Testad - fungerar 
#define SLTU_	xi[rd].reg =	xi[rs1].reg < xi[rs2].reg;								//Testad - fungerar
#define SLL_	xi[rd].reg =	xi[rs1].reg << (0b11111 & xi[rs2].reg);  				//Testad - fungerar
#define SRL_	xi[rd].reg =	xi[rs1].reg >> (0b11111 & xi[rs2].reg); 				//Testad - fungerar
#define SRA_	xi[rd].reg =	(signed long)(xi[rs1].reg >> (0b11111 & xi[rs2].reg));	//Testad - fungerar
#define XOR_	xi[rd].reg =	xi[rs1].reg ^ xi[rs2].reg;								//Inte testad
#define OR_		xi[rd].reg =	xi[rs1].reg | xi[rs2].reg;								//Inte testad
#define AND_	xi[rd].reg =	xi[rs1].reg & xi[rs2].reg;								//Inte testad

#define ADDI_	xi[rd].reg =	xi[rs1].reg + imm12;									//Testad - fungerar
#define SLTI_	xi[rd].reg =	(signed int)xi[rs1].reg < imm12;						//Testad - fungerar
#define SLTIU_  xi[rd].reg =	xi[rs1].reg < (unsigned int) imm12;						//Testad - fungerar
#define XORI_	xi[rd].reg =	xi[rs1].reg ^ imm12;									//Inte testad
#define ORI_	xi[rd].reg =	xi[rs1].reg | imm12;									//Inte testad
#define ANDI_	xi[rd].reg =	xi[rs1].reg & imm12;									//Inte testad
#define SLLI_	xi[rd].reg =	xi[rs1].reg << immL;									//Testad - fungerar
#define SRLI_	xi[rd].reg =	(xi[rs1].reg >> immL);									//Testad - fungerar
#define SRAI_	xi[rd].reg =	(signed long) (xi[rs1].reg >> immL);					//Testad - fungerar

RISC::Reg32::Reg32()
{
	reg = 0;
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
	immL = (0b11111 & imm12); // Lower 5 bits of immidiate
	immH = (0b111111100000 & imm12); // higher 7 bit of immidiate (Right shift flag (IDFK varför de har gjort så här))
	imm12 = signExtend(imm12, 12);

}

void RISC::Instruction::deCodeRtype(uint32_t inst)
{
	funct3 =	(0b00000000000000000111000000000000 & inst) >> 12;
	rd =		(0b111110000000 & inst) >> 7; 
	rs1 =		(0b11111000000000000000 & inst) >> 15;
	rs2 =		(0b1111100000000000000000000 & inst) >> 20;
	funct7 =	(0b11111110000000000000000000000000 & inst) >> 25;
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
	if (opCode == OP) {
		deCodeRtype(inst);
		switch (funct3) {
			case ADD: if (funct7==0){ADD_;} else {SUB_F;} break;
			case SLL: SLL_; break;
			case SRL: if (funct7==0){SRL_;} else{SRA_  ;} break;
			case SLT: SLT_; break;
			case SLTU: SLTU_; break;
			case XOR: XOR_; break;
			case OR: OR_; break;
			case AND: AND_; break;
			default: std::cout << "funct är negativt eller för högt...\n"; break;
		}
	}
	else if (opCode == OP_IMM) {
		deCodeItype(inst);
		if (funct3 != SLLI && funct3 != SRLI && funct3 != SRAI) {
			//Alla dessa är av typ I
			switch (funct3) {
				case ADDI:	ADDI_;	break;
				case SLTI:	SLTI_;	break;
				case SLTIU:	SLTIU_;	break;
				case XORI:	XORI_;	break;
				case ORI:	ORI_;	break;
				case ANDI:	ANDI_;	break;
				default: std::cout << "funct är negativt eller för högt...\n"; break;
			}
		}
		else {
			switch (funct3)
			{
			case SLLI: SLLI_; break;
				case SRLI: if (immH == 0b0100000) { SRAI_; } else { SRLI_; } break; 
				default:
					break;
			}
		}
	}
	else if (opCode == 123) {
		//Att göra...
	}
}
﻿#pragma once
#include <stdint.h>
#include <array>
#include <vector>

namespace RISC {

	class Reg32 {
		/*Ett register som är 32 birar långt (används ofta i RV32I)*/
	public:
		Reg32();
		uint32_t reg;
	};

	class Instruction {
	private:
		//Virtuella minne
		std::array<uint32_t, 32> xi;
		uint32_t pc;
		//Variabler
		int opCode=0, rd=0, rs1=0, rs2=0, funct3=0, shift=0;
		int funct7=0, imm12=0, immL=0, immH=0; 
		long imm20;
		unsigned int totalInstructions = 0;
		uint32_t signExtend(uint32_t input, unsigned char size);
		void preCalculate(uint32_t inst);
		void deCodeItype(uint32_t inst);
		void deCodeRtype(uint32_t inst);
		void deCodeStype(uint32_t inst);
		void deCodeBtype(uint32_t inst);
		void deCodeJtype(uint32_t inst);
		//Functions 
		void deCodeInstruction(uint32_t inst);
		void setMem(uint32_t pos, uint32_t val, uint8_t byteSize);
		void loadMem(uint32_t pos, uint8_t byteSize, bool isSiged);
	public:
		Instruction();
		bool loadProgram(std::vector<uint32_t> &input);
		bool step();//Utför en instruktion
		//Public vars:
		std::array<uint8_t, 4096> mem;
	};

}
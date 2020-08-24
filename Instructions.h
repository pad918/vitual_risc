#pragma once
#include <stdint.h>
#include <array>

namespace RISC {

	class Reg32 {
		/*Ett register som är 32 birar långt (används ofta i RV32I)*/
	public:
		Reg32();
		uint32_t reg;
	};

	class Instruction {
	private:
		//Virtuella register
		std::array<RISC::Reg32, 32> xi; // 32 heltalsregister, std::array är bra för debugging
		//Variabler
		int opCode=0, rd=0, rs1=0, rs2=0, funct3=0, shift=0;
		int funct7=0, imm12=0, /*Följande är speciella för flaktiva i-type: */ immL=0, immH=0; 
		uint32_t signExtend(uint32_t input, unsigned char);
		void preCalculate(uint32_t inst);
		void deCodeItype(uint32_t inst);
		void deCodeRtype(uint32_t inst);
	public:
		Instruction();
		void deCodeInstruction(uint32_t inst, std::array<uint8_t, 1024> &mem);
	};

}
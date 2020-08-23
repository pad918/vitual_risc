﻿#pragma once
#include <array>
#include <stdint.h>
#include "Instructions.h"

namespace RISC {

	class RV32I {
	/*
		En simulerad cpu meed RV32I isa:n, fast den använder big-endian 
		istället för den äckliga little-endian
	*/
	private:
		//Virtual memory
		std::array<uint8_t, 1024> mem; //Ram memory
		Reg32 pc;
		Instruction deCoder;
	public:
		RV32I();
		void step();//Utför en instruktion
	};
}

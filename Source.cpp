#include <iostream>
#include "RV32I.h"
#include "Compiler.h"

int main() {

	//TEST
	ASM::Compiller cmp;
	RISC::RV32I risc_cpu;
	risc_cpu.loadProgram(*cmp.compile("C:/Users/mansa/source/repos/RV32I_01/RV32I_01/pgm.asm"));
	for(int i=0; i<3; i++)
		risc_cpu.step();

	//END

	int c; std::cin >> c; // Se till att den stannar kvar
	return 0;
}
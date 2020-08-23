#include <iostream>
#include "RV32I.h"

int main() {
	//TEST
	RISC::RV32I risc_cpu;
	risc_cpu.step();
	risc_cpu.step();
	risc_cpu.step();
	//END

	int c; std::cin >> c; // Se till att den stannar kvar
	return 0;
}
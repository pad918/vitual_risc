#include <iostream>
#include <chrono>
#include "Compiler.h"
#include "Instructions.h"

int main() {

	//TEST
	ASM::Compiller cmp;
	RISC::Instruction risc_cpu;
	risc_cpu.loadProgram(*cmp.compile("C:/Users/mansa/source/repos/RV32I_01/RV32I_01/assembly_files/primeTester.asm"));
	bool isRunning = true;
	auto start = std::chrono::high_resolution_clock::now();
	while (isRunning) {
		isRunning = !risc_cpu.step();
	}
	auto end = std::chrono::high_resolution_clock::now();
	//END
	auto duration = std::chrono::duration_cast<std::chrono::microseconds>(end - start);
	std::cout << "Total time: " << duration.count() << "us\n";
	int c; std::cin >> c; // Se till att den stannar kvar
	return 0;
}
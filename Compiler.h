#pragma once
#include <string>
#include <array>
#include <vector>
#include <stdint.h>

namespace ASM {

	class Compiller {
	/*
	Not a compiler but an assembly to machine code translator (ASSEMBLER?)
	*/
	private:
		std::array<std::string, 41> _INSTRUCTIONS;
		std::array<std::string, 32> _IREGISTERS;
		std::array<std::string, 32> _FREGISTERS;
		std::array<std::string, 14> _ITYPE_INSTRUCTIONS;
		std::array<std::string, 10> _RTYPE_INSTRUCTIONS;
		std::vector<uint32_t> program;
		std::vector<std::string> split(const std::string& str, const std::string& delim);
		uint32_t generateInstruction(std::string inst, std::vector<std::string> args);
		bool is_ITYPE(std::string input);
	public:
		Compiller();
		std::vector<uint32_t> * compile(std::string path);
	};

}

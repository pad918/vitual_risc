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
		//
		std::array<std::string, 41> _INSTRUCTIONS;
		std::array<std::string, 32> _IREGISTERS;
		std::array<std::string, 32> _FREGISTERS;
		//Listor över instructioner av olika typer
		std::array<std::string, 15> _ITYPE_INSTRUCTIONS;
		std::array<std::string, 10> _RTYPE_INSTRUCTIONS;
		std::array<std::string, 5>  _STYPE_INSTRUCTIONS;
		std::array<std::string, 2>  _UTYPE_INSTRUCTIONS;
		std::array<std::string, 6>  _BTYPE_INSTRUCTIONS;
		std::array<std::string, 1>  _JTYPE_INSTRUCTIONS;
		//
		std::vector<uint32_t> program;
		std::vector<std::string> split(const std::string& str, const std::string& delim);
		uint32_t generateInstruction(std::string inst, std::vector<std::string> args, int lineId);
	public:
		Compiller();
		std::vector<uint32_t> * compile(std::string path);
	};

}

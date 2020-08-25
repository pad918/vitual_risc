#include "Compiler.h"
#include <iostream>
#include <fstream>
#include <ios>
#include <algorithm>

std::vector<std::string> ASM::Compiller::split(const std::string & str, const std::string & delim)
{
	std::vector<std::string> tokens;
	size_t prev = 0, pos = 0;
	do
	{
		pos = str.find(delim, prev);
		if (pos == std::string::npos) pos = str.length();
		std::string token = str.substr(prev, pos - prev);
		if (!token.empty()) tokens.push_back(token);
		prev = pos + delim.length();
	} while (pos < str.length() && prev < str.length());
	return tokens;
}

uint32_t ASM::Compiller::generateInstruction(std::string inst, std::vector<std::string> args)
{
	uint32_t output = 0;
	//MASSVIS MED AVANCERAD KOD:
	if (std::find(_ITYPE_INSTRUCTIONS.begin(), _ITYPE_INSTRUCTIONS.end(), inst) != _ITYPE_INSTRUCTIONS.end()) { //Om det är en i_type instruktion
		if (args.size() != 3) { std::cout << "ERROR: args invalid\n"; return 0; }
		//Add imm, rd and rs1
		uint16_t rd  = std::stoi(args[0]);	//SKALL FIXAS TILL REG ISTÄLLET FÖR NUMMER!
		uint16_t rs1 = std::stoi(args[1]);	//SKALL FIXAS TILL REG ISTÄLLET FÖR NUMMER!
		int16_t imm = std::stoi(args[2]);
		output = output | (imm << 20);
		output = output | (rd  <<  7);
		output = output | (rs1 << 15);
		//Add opcode and func3:
		if		(inst == "ADDI")	{ output = output | (0b000 << 12); output = output | 0b0010011; }
		else if (inst == "SLTI")	{ output = output | (0b010 << 12); output = output | 0b0010011; }
		else if (inst == "SLTIU")	{ output = output | (0b011 << 12); output = output | 0b0010011; }
		else if (inst == "XORI")	{ output = output | (0b100 << 12); output = output | 0b0010011; }
		else if (inst == "ORI")		{ output = output | (0b110 << 12); output = output | 0b0010011; }
		else if (inst == "ANDI")	{ output = output | (0b111 << 12); output = output | 0b0010011; }
		else if (inst == "SLLI")	{ output = output | (0b001 << 12); output = output | 0b0010011; }
		else if (inst == "SRAI")	{ output = output | (0b101 << 12); output = output | 0b0010011; }	//Inte av I tpye, gör en egen kategori...
		else if (inst == "SRLI")	{ output = output | (0b101 << 12); output = output | 0b0010011; }	//Inte av I tpye, gör en egen kategori...
	}
	else { std::cout << "ERROR: Incorrect instuction" << std::endl; return 0; }

	return output;
}

bool ASM::Compiller::is_ITYPE(std::string input)
{
	
	return false;
}

ASM::Compiller::Compiller()
{
	for (int i = 0; i < _IREGISTERS.size(); i++)
		_IREGISTERS[i] = "x" + std::to_string(i);
	for (int i = 0; i < _FREGISTERS.size(); i++)
		_FREGISTERS[i] = "f" + std::to_string(i);
	_INSTRUCTIONS = {
		"ADD", "SUB", "SLL", "SLT", "SLTU", "XOR", "SRL", "SRA", "OR", "AND",
		"ADDI", "SLTI", "SLTIU", "XORI", "ORI", "ANDI", "SLLI", "SRLI", "SRAI",
		"BEQ", "BNE", "BLT", "BGE", "BLTU", "BGEU", "LUI", "AUIPC", "JAL", 
		"JALR", "LB", "LH", "LW", "LBU", "LHU", "SB", "SH", "SW", "SBU", "SHU",
		"FENCE", "FENCE.I"
	};
	_ITYPE_INSTRUCTIONS = {"ADDI", "SLTI", "SLTIU", "XORI", "ORI", "ANDI", "SLLI", "SRLI", "SRAI", "LB", "LH", "LW", "LBU", "LHU"};
}

std::vector<uint32_t> *ASM::Compiller::compile(std::string path)
{
	program.clear();
	std::string line;
	std::ifstream file(path);
	if (file.is_open()) {
		while (getline(file, line)) {
			uint32_t m_code = 0;
			std::size_t commentId = line.find_first_of(";");
			if (commentId != std::string::npos) //if there is a comment remove it
				line.erase(line.begin() + commentId, line.end());
			std::size_t letterId = line.find_first_of("ABCDEEFGHIJKLMNOPQRSTUVWXYZ"); // find fist char pos of instruction 
			if(letterId != std::string::npos) // Remove white spaces before first letter
				line.erase(line.begin(), line.begin() + letterId);
			std::size_t codeEndId = line.find_first_of(" \t");
			if (codeEndId == std::string::npos) { std::cout << "ERROR: INCORRECT FORMAT!"; }
			std::string inst = line.substr(0, codeEndId);
			std::string tmp = line.substr(codeEndId, line.size() - codeEndId);
			std::vector<std::string> args = split(tmp, ",");
			for (std::string &a : args) {
				std::size_t fisrtNonSpace = a.find_first_not_of(" \t"); // REMOVE BLANKS BEFORE
				if (fisrtNonSpace != std::string::npos) { a.erase(a.begin(), a.begin() + fisrtNonSpace); }
				std::size_t lastId = a.find_last_not_of(" \t"); // REMOVE BLANKS AFTER
				if (lastId != std::string::npos && lastId != a.size()-1) { a.erase(a.begin() + lastId + 1, a.end()); }
			}
			// Pass instruction and argumnt string vectors for processing.
			program.push_back(generateInstruction(inst, args));
		}
		file.close();
	}
	return &program;
}

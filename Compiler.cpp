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
		//Load instructioner "LB", "LH", "LW", "LBU", "LHU"
		else if (inst == "LB")		{ output = output | (0b000 << 12); output = output | 0b0000011; }
		else if (inst == "LH")		{ output = output | (0b001 << 12); output = output | 0b0000011; }
		else if (inst == "LW")		{ output = output | (0b010 << 12); output = output | 0b0000011; }
		else if (inst == "LBU")		{ output = output | (0b100 << 12); output = output | 0b0000011; }
		else if (inst == "LHU")		{ output = output | (0b101 << 12); output = output | 0b0000011; }
		//Jump instruktionen
		else if (inst == "JALR")	{ output = output | (0b000 << 12); output = output | 0b1100111;}
		//Annorlunda I-type instructioner
		else if (inst == "SLLI")	{ output = output | (0b001 << 12); output = output | 0b0010011; output = output | (0b0000000 << 25); } //Bordeee typ funka...
		else if (inst == "SRAI")	{ output = output | (0b101 << 12); output = output | 0b0010011; output = output | (0b0000000 << 25); } //Bordeee typ funka...	
		else if (inst == "SRLI")	{ output = output | (0b101 << 12); output = output | 0b0010011; output = output | (0b0100000 << 25); } //Bordeee typ funka...
	}
	else if (std::find(_RTYPE_INSTRUCTIONS.begin(), _RTYPE_INSTRUCTIONS.end(), inst) != _RTYPE_INSTRUCTIONS.end()) { //Om det är en r_type instruktion
		if (args.size() != 3) { std::cout << "ERROR: args invalid\n"; return 0; }
		uint16_t rd=std::stoi(args[0]), rs1= std::stoi(args[1]), rs2= std::stoi(args[2]);
		output = output | (rd  << 7);
		output = output | (rs1 << 15);
		output = output | (rs2 << 20);
		output = output | 0b0110011; // Sets op code
		if		(inst == "ADD")		{ output = output | (0b000 << 12); output = output | (0b0000000 << 25); }
		else if (inst == "SUB")		{ output = output | (0b000 << 12); output = output | (0b0100000 << 25); }
		else if (inst == "SLL")		{ output = output | (0b001 << 12); output = output | (0b0000000 << 25); }
		else if (inst == "SLT")		{ output = output | (0b010 << 12); output = output | (0b0000000 << 25); }
		else if (inst == "SLTU")	{ output = output | (0b011 << 12); output = output | (0b0000000 << 25); }
		else if (inst == "XOR")		{ output = output | (0b100 << 12); output = output | (0b0000000 << 25); }
		else if (inst == "SRL")		{ output = output | (0b101 << 12); output = output | (0b0000000 << 25); }
		else if (inst == "SRA")		{ output = output | (0b101 << 12); output = output | (0b0100000 << 25); }
		else if (inst == "OR")		{ output = output | (0b110 << 12); output = output | (0b0000000 << 25); }
		else if (inst == "AND")		{ output = output | (0b111 << 12); output = output | (0b0000000 << 25); }
	}
	else if (std::find(_STYPE_INSTRUCTIONS.begin(), _STYPE_INSTRUCTIONS.end(), inst) != _STYPE_INSTRUCTIONS.end()) { //Om det är en s_type instruktion
		if (args.size() != 3) { std::cout << "ERROR: args invalid\n"; return 0; }
		uint16_t rs1 = std::stoi(args[0]), rs2 = std::stoi(args[1]);
		int16_t imm12 = std::stoi(args[2]);
		uint16_t immH = (imm12 & 0b111111100000) >> 5, immL = imm12 & 0b000000011111;
		output = output | (immH << 25);
		output = output | (immL <<  7);
		output = output | (rs1  << 15);
		output = output | (rs2  << 20);
		//Set op code
		output = output | (0b0100011);
		//Set funct3
		if		(inst == "SB")		{ output = output | (0b000 << 12); }
		else if (inst == "SH")		{ output = output | (0b001 << 12); }
		else if (inst == "SW")		{ output = output | (0b010 << 12); }
		else if (inst == "SBU")		{ output = output | (0b100 << 12); }
		else if (inst == "SHU")		{ output = output | (0b101 << 12); }
	}
	else if (std::find(_BTYPE_INSTRUCTIONS.begin(), _BTYPE_INSTRUCTIONS.end(), inst) != _BTYPE_INSTRUCTIONS.end()) { // Om det är en B_type instruktion
		if (args.size() != 3) { std::cout << "ERROR: args invalid\n"; return 0; }
		uint16_t rs1 = std::stoi(args[0]), rs2 = std::stoi(args[1]);
		int16_t imm12 = std::stoi(args[2]);
		uint16_t i12 = imm12 >> 12;
		uint16_t i11 = imm12 >> 11 & (0b01);
		uint16_t i105 = imm12 >> 5 & (0b111111);
		uint16_t i41 = imm12 >> 1 & (0b1111);
		output = output | (0b1100011); // opcode
		output = output | (rs1 << 15); //rs1
		output = output | (rs2 << 20); //rs2
		output = output | i12	<< 31;
		output = output | i11	<< 7;
		output = output | i105	<< 25;
		output = output | i41	<< 8;
		// set funct3
		if		(inst == "BEQ")		{ output = output | (0b000 << 12); }
		else if (inst == "BNE")		{ output = output | (0b001 << 12); }
		else if (inst == "BLT")		{ output = output | (0b100 << 12); }
		else if (inst == "BGE")		{ output = output | (0b101 << 12); }
		else if (inst == "BLTU")	{ output = output | (0b110 << 12); }
		else if (inst == "BGEU")	{ output = output | (0b111 << 12); }
	}
	else if (std::find(_JTYPE_INSTRUCTIONS.begin(), _JTYPE_INSTRUCTIONS.end(), inst) != _JTYPE_INSTRUCTIONS.end()) { // Om det är en J_type instruktion
		if (args.size() != 2) { std::cout << "ERROR: args invalid\n"; return 0; }
		uint16_t	rd = std::stoi(args[0]); //699050
		int32_t		imm12 = std::stoi(args[1]);
		uint16_t	imm101 = 0, imm20 = 0, imm11 = 0, imm1912 = 0;
		imm101 =	(0b1111111111			& imm12); // 0 till 9
		imm1912 =	(0b1111111100000000000	& imm12) >> 11;
		imm11 =		((imm12 & (1 << 10)) >> 10);
		imm20 =		((imm12 & (1 << 19)) >> 19); // 20-1
		output =	output | 0b1101111;//opcode
		output =	output | (rd << 7);
		output =	output | (imm101)  << 21;
		output =	output | (imm1912) << 12;
		output =	output | (imm11)   << 20;
		output =	output | (imm20)   << 31;
	}
	else { std::cout << "ERROR: Incorrect instuction" << std::endl; return 0; }

	return output;
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
	_ITYPE_INSTRUCTIONS = {"ADDI", "SLTI", "SLTIU", "XORI", "ORI", "ANDI", "SLLI", "SRLI", "SRAI", "LB", "LH", "LW", "LBU", "LHU", "JALR"};
	_RTYPE_INSTRUCTIONS = {"ADD", "SUB", "SLL", "SLT", "SLTU", "XOR", "SRL", "SRA", "OR", "AND"};
	_STYPE_INSTRUCTIONS = {"SB", "SH", "SW", "SBU", "SHU"};
	_UTYPE_INSTRUCTIONS = {"LUI", "AUIPC"};
	_BTYPE_INSTRUCTIONS = {"BEQ", "BNE", "BLT", "BGE", "BLTU", "BGEU"};
	_JTYPE_INSTRUCTIONS = {"JAL"};
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
			if (letterId != std::string::npos) // Remove white spaces before first letter
				line.erase(line.begin(), line.begin() + letterId);
			std::size_t codeEndId = line.find_first_of(" \t");
			if (line != "") {
				if (codeEndId == std::string::npos) {
					std::cout << "ERROR: INCORRECT FORMAT!";
					program.clear(); return &program;
				}
				std::string inst = line.substr(0, codeEndId);
				std::string tmp = line.substr(codeEndId, line.size() - codeEndId);
				std::vector<std::string> args = split(tmp, ",");
				for (std::string &a : args) {
					std::size_t fisrtNonSpace = a.find_first_not_of(" \t"); // REMOVE BLANKS BEFORE
					if (fisrtNonSpace != std::string::npos) { a.erase(a.begin(), a.begin() + fisrtNonSpace); }
					std::size_t lastId = a.find_last_not_of(" \t"); // REMOVE BLANKS AFTER
					if (lastId != std::string::npos && lastId != a.size() - 1) { a.erase(a.begin() + lastId + 1, a.end()); }
				}
				uint32_t tmpCode = generateInstruction(inst, args);
				if (tmpCode != 0)
					program.push_back(tmpCode);
			}
		}
		file.close();
	}
	return &program;
}

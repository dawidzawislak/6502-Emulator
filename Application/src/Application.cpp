#include "Emulator6502.h"
#include <iostream>
#include <bitset>


int main()
{
	Emulator::Memory memory;
	memory[0xFFFC] = (uint8_t)Emulator::Instructions::LDA_ABS;
	memory[0xFFFD] = 0xAA;
	memory[0xFFFE] = 0xAA;
	memory[0xAAAA] = 0xFF;

	Emulator::Emulator6502 em6502(memory);
	em6502.Reset();
	int additionalCycles = -em6502.Execute(4);

	std::cout << "A: 0x" << std::hex << (int)em6502.ReadAccumulatorValue() << "\n";
	std::cout << "X: 0x" << std::hex << (int)em6502.ReadXRegisterValue() << "\n";
	std::cout << "Y: 0x" << std::hex << (int)em6502.ReadYRegisterValue() << "\n";
	std::cout << "PC: 0x" << std::hex << (int)em6502.ReadProgramCounter() << "\n";
	std::cout << "SP: 0x" << std::hex << (int)em6502.ReadStackPointer() << "\n";
	std::cout << "Status: 0b" << std::bitset<8>{em6502.ReadProcessorStatus()} << "\n";
	std::cout << "Additional cycles needed to finish executing instruction: " << additionalCycles;
	return 0;
}
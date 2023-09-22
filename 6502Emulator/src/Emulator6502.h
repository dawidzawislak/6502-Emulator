#pragma once

#include "Memory.h"

namespace Emulator {

enum class Instructions
{
	LDA_IM = 0xA9,
	LDA_ZP = 0xA5,
	LDA_ZPX = 0xB5,
	LDA_ABS = 0xAD,
	LDA_ABSX = 0xBD,
	LDA_ABSY = 0xB9,
	LDA_INDX = 0xA1,
	LDA_INDY = 0xB1
};

class Emulator6502
{
	Memory& m_Memory;

	// Registers
	uint16_t m_ProgramCounter;
	uint8_t m_StackPointer;
	uint8_t m_Accumulator;
	uint8_t m_IndexRegisterX;
	uint8_t m_IndexRegisterY;

	uint8_t m_ProcessorStatus;

public:
	Emulator6502(Memory& memory);

	void Reset();
	int Execute(int cycles);

	uint8_t ReadAccumulatorValue() const;
	uint8_t ReadXRegisterValue() const;
	uint8_t ReadYRegisterValue() const;
	uint8_t ReadStackPointer() const;
	uint16_t ReadProgramCounter() const;
	uint8_t ReadProcessorStatus() const;
	

private:
	const uint8_t& FetchByte(int& cycles);
	const uint8_t& ReadByte(uint16_t address, int& cycles);

	// Load / Store Operations
	void LDAImmediate(int& cycles);
	void LDAZeroPage(int& cycles);
	void LDAZeroPageX(int& cycles);
	void LDAAbsolute(int& cycles);
	void LDAAbsoluteX(int& cycles);
	void LDAAbsoluteY(int& cycles);
	void LDAIndirectX(int& cycles);
	void LDAIndirectY(int& cycles);

	void SetLDAFlags();

	// Flags setters/getters
	void SetCarryFlag(bool state);
	void SetZeroFlag(bool state);
	void SetInterruptDisableFlag(bool state);
	void SetDecimalModeFlag(bool state);
	void SetBreakCommandFlag(bool state);
	void SetOverflowFlag(bool state);
	void SetNegativeFlag(bool state);
};

}
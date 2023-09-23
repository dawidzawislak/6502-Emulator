#pragma once

#include "Memory.h"

namespace Emulator {

enum class Instructions
{
	NOP = 0xEA,

	LDA_IM = 0xA9,
	LDA_ZP = 0xA5,
	LDA_ZPX = 0xB5,
	LDA_ABS = 0xAD,
	LDA_ABSX = 0xBD,
	LDA_ABSY = 0xB9,
	LDA_INDX = 0xA1,
	LDA_INDY = 0xB1,

	LDX_IM = 0xA2,
	LDX_ZP = 0xA6,
	LDX_ZPY = 0xB6,
	LDX_ABS = 0xAE,
	LDX_ABSY = 0xBE,

	LDY_IM = 0xA0,
	LDY_ZP = 0xA4,
	LDY_ZPX = 0xB4,
	LDY_ABS = 0xAC,
	LDY_ABSX = 0xBC
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

	uint32_t m_cyclesElapsed;

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
	const uint8_t& FetchByte();
	const uint8_t& ReadByte(uint16_t address);

	// Load Operations
	void LDAImmediate();
	void LDAZeroPage();
	void LDAZeroPageX();
	void LDAAbsolute();
	void LDAAbsoluteX();
	void LDAAbsoluteY();
	void LDAIndirectX();
	void LDAIndirectY();

	void LDXImmediate();
	void LDXZeroPage();
	void LDXZeroPageY();
	void LDXAbsolute();
	void LDXAbsoluteY();

	void LDYImmediate();
	void LDYZeroPage();
	void LDYZeroPageX();
	void LDYAbsolute();
	void LDYAbsoluteX();

	void SetLDAFlags();
	void SetLDXFlags();
	void SetLDYFlags();

	// Store Operations


	uint16_t GetZeroPageAddress();
	uint16_t GetZeroPageXAddress();
	uint16_t GetZeroPageYAddress();
	uint16_t GetAbsoluteAddress();
	uint16_t GetAbsoluteXAddress();
	uint16_t GetAbsoluteYAddress();
	uint16_t GetIndirectXAddress();
	uint16_t GetIndirectYAddress();

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
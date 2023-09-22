#include "Emulator6502.h"
#include <iostream>

namespace Emulator {

Emulator6502::Emulator6502(Memory& memory)
	: m_Memory(memory)
{}

void Emulator6502::Reset()
{
	m_ProgramCounter = 0xFFFC;
	m_StackPointer = 0x0100;
	m_ProcessorStatus = 0;
	m_Accumulator = m_IndexRegisterX = m_IndexRegisterY = 0;
}

int Emulator6502::Execute(int cycles)
{
	while (cycles > 0) {
		const uint8_t& ins = FetchByte(cycles);

		switch (ins)
		{
		case (int)Instructions::LDA_IM: LDAImmediate(cycles);	break;
		case (int)Instructions::LDA_ZP: LDAZeroPage(cycles);	break;
		case (int)Instructions::LDA_ZPX: LDAZeroPageX(cycles);	break;
		case (int)Instructions::LDA_ABS: LDAAbsolute(cycles);	break;
		default: std::cout << "Invalid instruction!\n";			break;
		}
	}

	return cycles;
}

uint8_t Emulator6502::ReadAccumulatorValue() const
{
	return m_Accumulator;
}

const uint8_t& Emulator6502::FetchByte(int& cycles)
{
	const uint8_t& val = m_Memory[m_ProgramCounter];
	cycles--;
	m_ProgramCounter++;

	return val;
}

const uint8_t& Emulator6502::ReadByte(uint16_t address, int& cycles)
{
	const uint8_t& val = m_Memory[address];
	cycles--;

	return val;
}

void Emulator6502::LDAImmediate(int& cycles)
{
	m_Accumulator = FetchByte(cycles);
	SetLDAFlags();
}

void Emulator6502::LDAZeroPage(int& cycles)
{
	uint16_t addr = FetchByte(cycles);
	m_Accumulator = ReadByte(addr, cycles);
	SetLDAFlags();
}

void Emulator6502::LDAZeroPageX(int& cycles)
{
	uint16_t addr = FetchByte(cycles) + m_IndexRegisterX;
	cycles--;
	addr &= 0xFF;
	m_Accumulator = ReadByte(addr, cycles);
	SetLDAFlags();
}

void Emulator6502::LDAAbsolute(int& cycles)
{
	uint16_t addr = FetchByte(cycles);
	addr = (addr << 8) | FetchByte(cycles);
	m_Accumulator = ReadByte(addr, cycles);
	SetLDAFlags();
}

void Emulator6502::SetLDAFlags()
{
	SetZeroFlag(m_Accumulator == 0);
	SetNegativeFlag((m_Accumulator & 0b1000000) == 1);
}

void Emulator6502::SetCarryFlag(bool state)
{
	if (state) m_ProcessorStatus |= 1 << 0;
	else m_ProcessorStatus &= ~(1 << 0);
}

void Emulator6502::SetZeroFlag(bool state)
{
	if (state) m_ProcessorStatus |= 1 << 1;
	else m_ProcessorStatus &= ~(1 << 1);
}

void Emulator6502::SetInterruptDisableFlag(bool state)
{
	if (state) m_ProcessorStatus |= 1 << 2;
	else m_ProcessorStatus &= ~(1 << 2);
}

void Emulator6502::SetDecimalModeFlag(bool state)
{
	if (state) m_ProcessorStatus |= 1 << 3;
	else m_ProcessorStatus &= ~(1 << 3);
}

void Emulator6502::SetBreakCommandFlag(bool state)
{
	if (state) m_ProcessorStatus |= 1 << 4;
	else m_ProcessorStatus &= ~(1 << 4);
}

void Emulator6502::SetOverflowFlag(bool state)
{
	if (state) m_ProcessorStatus |= 1 << 6;
	else m_ProcessorStatus &= ~(1 << 6);
}

void Emulator6502::SetNegativeFlag(bool state)
{
	if (state) m_ProcessorStatus |= 1 << 7;
	else m_ProcessorStatus &= ~(1 << 7);
}

}
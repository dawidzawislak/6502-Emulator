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
		// LDA
		case (int)Instructions::LDA_IM: LDAImmediate(cycles);	break;
		case (int)Instructions::LDA_ZP: LDAZeroPage(cycles);	break;
		case (int)Instructions::LDA_ZPX: LDAZeroPageX(cycles);	break;
		case (int)Instructions::LDA_ABS: LDAAbsolute(cycles);	break;
		case (int)Instructions::LDA_ABSX: LDAAbsoluteX(cycles);	break;
		case (int)Instructions::LDA_ABSY: LDAAbsoluteY(cycles);	break;
		case (int)Instructions::LDA_INDX: LDAIndirectX(cycles);	break;
		case (int)Instructions::LDA_INDY: LDAIndirectY(cycles);	break;


		default: std::cout << "Invalid instruction!\n";			break;
		}
	}

	return cycles;
}

uint8_t Emulator6502::ReadAccumulatorValue() const
{
	return m_Accumulator;
}

uint8_t Emulator6502::ReadXRegisterValue() const
{
	return m_IndexRegisterX;
}

uint8_t Emulator6502::ReadYRegisterValue() const
{
	return m_IndexRegisterY;
}

uint8_t Emulator6502::ReadStackPointer() const
{
	return m_StackPointer;
}

uint16_t Emulator6502::ReadProgramCounter() const
{
	return m_ProgramCounter;
}

uint8_t Emulator6502::ReadProcessorStatus() const
{
	return m_ProcessorStatus;
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

void Emulator6502::LDAAbsoluteX(int& cycles)
{
	uint16_t addr = FetchByte(cycles);
	addr = (addr << 8) | FetchByte(cycles);
	addr += m_IndexRegisterX;
	if ((addr & 0xFF) < m_IndexRegisterX) // this is only true when the above addition "wraps around", or carries
		cycles--;
	m_Accumulator = ReadByte(addr, cycles);
	SetLDAFlags();
}

void Emulator6502::LDAAbsoluteY(int& cycles)
{
	uint16_t addr = FetchByte(cycles);
	addr = (addr << 8) | FetchByte(cycles);
	addr += m_IndexRegisterY;
	if ((addr & 0xFF) < m_IndexRegisterY) // this is only true when the above addition "wraps around", or carries
		cycles--;
	m_Accumulator = ReadByte(addr, cycles);
	SetLDAFlags();
}

void Emulator6502::LDAIndirectX(int& cycles)
{
	uint16_t zpAddr = FetchByte(cycles) + m_IndexRegisterX;
	cycles--;
	zpAddr &= 0xFF;
	uint16_t addr = (ReadByte(zpAddr+1, cycles) << 2) | ReadByte(zpAddr, cycles);
	m_Accumulator = ReadByte(addr, cycles);
	SetLDAFlags();
}

void Emulator6502::LDAIndirectY(int& cycles)
{
	uint16_t zpAddr = FetchByte(cycles);
	uint16_t addr = (ReadByte(zpAddr+1, cycles) << 2) | ReadByte(zpAddr, cycles);
	addr += m_IndexRegisterY;
	if ((addr & 0xFF) < m_IndexRegisterY) // this is only true when the above addition "wraps around", or carries
		cycles--;
	m_Accumulator = ReadByte(addr, cycles);
	SetLDAFlags();
}

// ----------------------------------------------------

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
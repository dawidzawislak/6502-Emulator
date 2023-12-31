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
	m_cyclesElapsed = 0;

	while (cycles > m_cyclesElapsed) {
		const uint8_t& ins = FetchByte();

		switch (ins)
		{
		case (int)Instructions::NOP:		m_cyclesElapsed++;		break;
		// LDA
		case (int)Instructions::LDA_IM:		LDAImmediate();	break;
		case (int)Instructions::LDA_ZP:		LDAZeroPage();	break;
		case (int)Instructions::LDA_ZPX:	LDAZeroPageX();	break;
		case (int)Instructions::LDA_ABS:	LDAAbsolute();	break;
		case (int)Instructions::LDA_ABSX:	LDAAbsoluteX();	break;
		case (int)Instructions::LDA_ABSY:	LDAAbsoluteY();	break;
		case (int)Instructions::LDA_INDX:	LDAIndirectX();	break;
		case (int)Instructions::LDA_INDY:	LDAIndirectY();	break;

		// LDX
		case (int)Instructions::LDX_IM:		LDXImmediate();	break;
		case (int)Instructions::LDX_ZP:		LDXZeroPage();	break;
		case (int)Instructions::LDX_ZPY:	LDXZeroPageY();	break;
		case (int)Instructions::LDX_ABS:	LDXAbsolute();	break;
		case (int)Instructions::LDX_ABSY:	LDXAbsoluteY();	break;

		// LDY
		case (int)Instructions::LDY_IM:		LDYImmediate();	break;
		case (int)Instructions::LDY_ZP:		LDYZeroPage();	break;
		case (int)Instructions::LDY_ZPX:	LDYZeroPageX();	break;
		case (int)Instructions::LDY_ABS:	LDYAbsolute();	break;
		case (int)Instructions::LDY_ABSX:	LDYAbsoluteX();	break;

		// STA
		case (int)Instructions::STA_ZP:		STAZeroPage();	break;
		case (int)Instructions::STA_ZPX:	STAZeroPageX();	break;
		case (int)Instructions::STA_ABS:	STAAbsolute();	break;
		case (int)Instructions::STA_ABSX:	STAAbsoluteX();	break;
		case (int)Instructions::STA_ABSY:	STAAbsoluteY();	break;
		case (int)Instructions::STA_INDX:	STAIndirectX();	break;
		case (int)Instructions::STA_INDY:	STAIndirectY();	break;

		// STX
		case (int)Instructions::STX_ZP:		STXZeroPage();	break;
		case (int)Instructions::STX_ZPY:	STXZeroPageY();	break;
		case (int)Instructions::STX_ABS:	STXAbsolute();	break;

		// STY
		case (int)Instructions::STY_ZP:		STYZeroPage();	break;
		case (int)Instructions::STY_ZPX:	STYZeroPageX();	break;
		case (int)Instructions::STY_ABS:	STYAbsolute();	break;

		// Register Transfers
		case (int)Instructions::TAX:		TAX();	break;
		case (int)Instructions::TAY:		TAY();	break;
		case (int)Instructions::TXA:		TXA();	break;
		case (int)Instructions::TYA:		TYA();	break;

		// Stack operations
		case (int)Instructions::TSX:		TSX();	break;
		case (int)Instructions::TXS:		TXS();	break;
		case (int)Instructions::PHA:		PHA();	break;
		case (int)Instructions::PHP:		PHP();	break;
		case (int)Instructions::PLA:		PLA();	break;
		case (int)Instructions::PLP:		PLP();	break;

		default: std::cout << "Invalid instruction!\n";			break;
		}
	}

	return m_cyclesElapsed - cycles;
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

const uint8_t& Emulator6502::FetchByte()
{
	const uint8_t& val = m_Memory[m_ProgramCounter];
	m_cyclesElapsed++;
	m_ProgramCounter++;

	return val;
}

const uint8_t& Emulator6502::ReadByte(uint16_t address)
{
	const uint8_t& val = m_Memory[address];
	m_cyclesElapsed++;

	return val;
}

void Emulator6502::LDAImmediate()
{
	m_Accumulator = FetchByte();
	SetNZFlags(m_Accumulator);
}

void Emulator6502::LDAZeroPage()
{
	uint16_t addr = GetZeroPageAddress();
	m_Accumulator = ReadByte(addr);
	SetNZFlags(m_Accumulator);
}

void Emulator6502::LDAZeroPageX()
{
	uint16_t addr = GetZeroPageXAddress();
	m_Accumulator = ReadByte(addr);
	SetNZFlags(m_Accumulator);
}

void Emulator6502::LDAAbsolute()
{
	uint16_t addr = GetAbsoluteAddress();
	m_Accumulator = ReadByte(addr);
	SetNZFlags(m_Accumulator);
}

void Emulator6502::LDAAbsoluteX()
{
	uint16_t addr = GetAbsoluteXAddress();
	m_Accumulator = ReadByte(addr);
	SetNZFlags(m_Accumulator);
}

void Emulator6502::LDAAbsoluteY()
{
	uint16_t addr = GetAbsoluteYAddress();
	m_Accumulator = ReadByte(addr);
	SetNZFlags(m_Accumulator);
}

void Emulator6502::LDAIndirectX()
{
	uint16_t addr = GetIndirectXAddress();
	m_Accumulator = ReadByte(addr);
	SetNZFlags(m_Accumulator);
}

void Emulator6502::LDAIndirectY()
{
	uint16_t addr = GetIndirectYAddress();
	m_Accumulator = ReadByte(addr);
	SetNZFlags(m_Accumulator);
}

void Emulator6502::LDXImmediate()
{
	m_IndexRegisterX = FetchByte();
	SetNZFlags(m_IndexRegisterX);
}

void Emulator6502::LDXZeroPage()
{
	uint16_t addr = GetZeroPageAddress();
	m_IndexRegisterX = ReadByte(addr);
	SetNZFlags(m_IndexRegisterX);
}

void Emulator6502::LDXZeroPageY()
{
	uint16_t addr = GetZeroPageYAddress();
	m_IndexRegisterX = ReadByte(addr);
	SetNZFlags(m_IndexRegisterX);
}

void Emulator6502::LDXAbsolute()
{
	uint16_t addr = GetAbsoluteAddress();
	m_IndexRegisterX = ReadByte(addr);
	SetNZFlags(m_IndexRegisterX);
}

void Emulator6502::LDXAbsoluteY()
{
	uint16_t addr = GetAbsoluteYAddress();
	m_IndexRegisterX = ReadByte(addr);
	SetNZFlags(m_IndexRegisterX);
}

void Emulator6502::LDYImmediate()
{
	m_IndexRegisterY = FetchByte();
	SetNZFlags(m_IndexRegisterY);
}

void Emulator6502::LDYZeroPage()
{
	uint16_t addr = GetZeroPageAddress();
	m_IndexRegisterY = ReadByte(addr);
	SetNZFlags(m_IndexRegisterY);
}

void Emulator6502::LDYZeroPageX()
{
	uint16_t addr = GetZeroPageXAddress();
	m_IndexRegisterY = ReadByte(addr);
	SetNZFlags(m_IndexRegisterY);
}

void Emulator6502::LDYAbsolute()
{
	uint16_t addr = GetAbsoluteAddress();
	m_IndexRegisterY = ReadByte(addr);
	SetNZFlags(m_IndexRegisterY);
}

void Emulator6502::LDYAbsoluteX()
{
	uint16_t addr = GetAbsoluteXAddress();
	m_IndexRegisterY = ReadByte(addr);
	SetNZFlags(m_IndexRegisterY);
}

void Emulator6502::SetNZFlags(uint8_t value)
{
	SetZeroFlag(value == 0);
	SetNegativeFlag((value & 0b1000000) > 0);
}

// -- Storing -----------------------------------------

void Emulator6502::STAZeroPage()
{
	uint32_t addr = GetZeroPageAddress();
	m_Memory[addr] = m_Accumulator;
	m_cyclesElapsed++;
}

void Emulator6502::STAZeroPageX()
{
	uint32_t addr = GetZeroPageXAddress();
	m_Memory[addr] = m_Accumulator;
	m_cyclesElapsed++;
}

void Emulator6502::STAAbsolute()
{
	uint32_t addr = GetAbsoluteAddress();
	m_Memory[addr] = m_Accumulator;
	m_cyclesElapsed++;
}

void Emulator6502::STAAbsoluteX()
{
	uint32_t addr = GetAbsoluteXAddress(false);
	m_Memory[addr] = m_Accumulator;
	m_cyclesElapsed++;
}

void Emulator6502::STAAbsoluteY()
{
	uint32_t addr = GetAbsoluteYAddress(false);
	m_Memory[addr] = m_Accumulator;
	m_cyclesElapsed++;
}

void Emulator6502::STAIndirectX()
{
	uint32_t addr = GetIndirectXAddress();
	m_Memory[addr] = m_Accumulator;
	m_cyclesElapsed++;
}

void Emulator6502::STAIndirectY()
{
	uint32_t addr = GetIndirectYAddress(false);
	m_Memory[addr] = m_Accumulator;
	m_cyclesElapsed++;
}

void Emulator6502::STXZeroPage()
{
	uint32_t addr = GetZeroPageAddress();
	m_Memory[addr] = m_IndexRegisterX;
	m_cyclesElapsed++;
}

void Emulator6502::STXZeroPageY()
{
	uint32_t addr = GetZeroPageYAddress();
	m_Memory[addr] = m_IndexRegisterX;
	m_cyclesElapsed++;
}

void Emulator6502::STXAbsolute()
{
	uint32_t addr = GetAbsoluteAddress();
	m_Memory[addr] = m_IndexRegisterX;
	m_cyclesElapsed++;
}

void Emulator6502::STYZeroPage()
{
	uint32_t addr = GetZeroPageAddress();
	m_Memory[addr] = m_IndexRegisterY;
	m_cyclesElapsed++;
}

void Emulator6502::STYZeroPageX()
{
	uint32_t addr = GetZeroPageXAddress();
	m_Memory[addr] = m_IndexRegisterY;
	m_cyclesElapsed++;
}

void Emulator6502::STYAbsolute()
{
	uint32_t addr = GetAbsoluteAddress();
	m_Memory[addr] = m_IndexRegisterY;
	m_cyclesElapsed++;
}

void Emulator6502::TAX()
{
	m_IndexRegisterX = m_Accumulator;
	m_cyclesElapsed++;
	SetNZFlags(m_IndexRegisterX);
}

void Emulator6502::TAY()
{
	m_IndexRegisterY = m_Accumulator;
	m_cyclesElapsed++;
	SetNZFlags(m_IndexRegisterY);
}

void Emulator6502::TXA()
{
	m_Accumulator = m_IndexRegisterX;
	m_cyclesElapsed++;
	SetNZFlags(m_Accumulator);
}

void Emulator6502::TYA()
{
	m_Accumulator = m_IndexRegisterY;
	m_cyclesElapsed++;
	SetNZFlags(m_Accumulator);
}

void Emulator6502::TSX()
{
	m_IndexRegisterX = m_StackPointer;
	m_cyclesElapsed++;
	SetNZFlags(m_IndexRegisterX);
}

void Emulator6502::TXS()
{
	m_StackPointer = m_IndexRegisterX;
	m_cyclesElapsed++;
}

void Emulator6502::PHA()
{
	m_Memory[m_StackPointer] = m_Accumulator;
	m_cyclesElapsed++;
	m_StackPointer--;
	m_cyclesElapsed++;
}

void Emulator6502::PHP()
{
	m_Memory[m_StackPointer] = m_ProcessorStatus;
	m_cyclesElapsed++;
	m_StackPointer--;
	m_cyclesElapsed++;
}

void Emulator6502::PLA()
{
	m_Accumulator = m_Memory[m_StackPointer];
	m_cyclesElapsed++;
	m_StackPointer++;
	m_cyclesElapsed++;
	SetNZFlags(m_Accumulator);
	m_cyclesElapsed++;
}

void Emulator6502::PLP()
{
	m_ProcessorStatus = m_Memory[m_StackPointer];
	m_StackPointer++;
	m_cyclesElapsed += 3;
}

uint16_t Emulator6502::GetZeroPageAddress()
{
	return FetchByte();
}

uint16_t Emulator6502::GetZeroPageXAddress()
{
	uint16_t addr = FetchByte() + m_IndexRegisterX;
	m_cyclesElapsed++;
	addr &= 0xFF;
	return addr;
}

uint16_t Emulator6502::GetZeroPageYAddress()
{
	uint16_t addr = FetchByte() + m_IndexRegisterY;
	m_cyclesElapsed++;
	addr &= 0xFF;
	return addr;
}

uint16_t Emulator6502::GetAbsoluteAddress()
{
	uint16_t addr = FetchByte();
	addr = (addr << 8) | FetchByte();

	return addr;
}

uint16_t Emulator6502::GetAbsoluteXAddress(bool addCycle)
{
	uint16_t addr = FetchByte();
	addr = (addr << 8) | FetchByte();
	addr += m_IndexRegisterX;
	if ((addr & 0xFF) < m_IndexRegisterX && addCycle) // this is only true when the above addition "wraps around", or carries
		m_cyclesElapsed++;

	return addr;
}

uint16_t Emulator6502::GetAbsoluteYAddress(bool addCycle)
{
	uint16_t addr = FetchByte();
	addr = (addr << 8) | FetchByte();
	addr += m_IndexRegisterY;
	if ((addr & 0xFF) < m_IndexRegisterY && addCycle) // this is only true when the above addition "wraps around", or carries
		m_cyclesElapsed++;

	return addr;
}

uint16_t Emulator6502::GetIndirectXAddress()
{
	uint16_t zpAddr = FetchByte() + m_IndexRegisterX;
	m_cyclesElapsed++;
	zpAddr &= 0xFF;
	uint16_t addr = (ReadByte(zpAddr + 1) << 8) | ReadByte(zpAddr);

	return addr;
}

uint16_t Emulator6502::GetIndirectYAddress(bool addCycle)
{
	uint16_t zpAddr = FetchByte();
	uint16_t addr = (ReadByte(zpAddr + 1) << 8) | ReadByte(zpAddr);
	addr += m_IndexRegisterY;
	if ((addr & 0xFF) < m_IndexRegisterY && addCycle) // this is only true when the above addition "wraps around", or carries
		m_cyclesElapsed++;

	return addr;
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
#pragma once

#include <cassert>
#include <cstdint>
#include <memory>

#define MEMORY_CAPACITY 0xFFFF

namespace Emulator {

class Memory
{
public:
	Memory()
	{
		memset(data, 0, sizeof(uint8_t) * MEMORY_CAPACITY);
	}

	uint8_t& operator[](uint16_t address)
	{
		assert(address < MEMORY_CAPACITY);
		return data[address];
	}

private:
	uint8_t data[MEMORY_CAPACITY];
};

}
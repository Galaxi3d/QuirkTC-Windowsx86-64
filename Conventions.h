#ifndef CONVENTIONS_H
#define CONVENTIONS_H
#include <string>
#include <array>

namespace Conventions /// TODO CREATE A SMARTER TABLE FOR ALL TYPES AND REGISTERS
{	
	constexpr std::array<const char*, 4> IntegerArguments = { "rcx","rdx","r8","r9" };
	constexpr std::array<const char*, 4> DoubleArguments = { "xmm0l","xmm1l","xmm2l","xmm3l" };
	constexpr std::array<const char*, 2> MathRegisters = { "rax","rdx" };
	constexpr std::array<const char*, 1> MemoryAccess = { "QWORD PTR" };

	inline int8_t IntegerArgumentPos = 0;
	inline int8_t DoubleArgumentPos = 0;
	inline uint8_t MathRegisterPos = 0; /// eax,ecx
}

#endif // !CONVENTIONS_H

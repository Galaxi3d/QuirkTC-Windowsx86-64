#ifndef ASSEMBLY_H
#define ASSEMBLY_H
#include <string>
#include "Bytecode.h"
#include <stack>

namespace Assembly
{
	inline std::string* AssemblySource;
	inline Bytecode::BytecodePackage* Package;
	inline std::stack<size_t> StackLocations;
	inline size_t ParameterAmount = 0;
}
#endif
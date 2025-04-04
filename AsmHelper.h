#ifndef ASMHELPER_H
#define ASMHELPER_H
#include <iostream>
#include <string>
#include "Bytecode.h"
#include "Conventions.h"

#define INDEX_TO_STACK_SIZE(index) index * 8 + 8

namespace AsmHelper
{
	std::string CreateStringName(size_t value);

	std::string CreateFunctionName(size_t value);

	std::string CreateLabelName(size_t value);

	std::string GetLabelName(size_t value); /// might be smarter to store the previous results?	

	std::string GetArithmeticOperator(Bytecode::BinaryOperators BinaryOperator);

	std::string GetConditionalJump(Bytecode::BinaryOperators BinaryOperators, bool IsJumpIf);

	std::string CreateStackAddress(size_t Address);
}





#endif
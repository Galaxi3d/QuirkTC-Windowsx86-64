#ifndef ASMHELPER_H
#define ASMHELPER_H
#include <iostream>
#include <string>
#include "Bytecode.h"

namespace AsmHelper
{
	std::string CreateStringName(size_t value);

	std::string CreateFunctionName(size_t value);

	std::string CreateLabelName(size_t value);

	std::string GetLabelName(size_t value); /// might be smarter to store the previous results?	

	std::string GetArithmeticOperator(Bytecode::BinaryOperators BinaryOperator);
}





#endif
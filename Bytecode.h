#ifndef BYTECODE_H
#define BYTECODE_H
#include "DataTypes.h"

namespace Bytecode
{
	typedef enum : int8_t
	{
		LOAD_CONST, /// floats, long, string, char
		LOAD_INT, /// direct value upto 8 bytes
		SET_PARAMETER,
		LOAD_PARAMETER,
		LOAD_PARAMETERS, /// takes the amount of parameters to load. if 0 thrn doesn't exist
		SET_VARIABLE,
		LOAD_VARIABLE,
		LOAD_FROM_STACK,
		MODIFY_FROM_STACK,
		STACK_ALLOCATE, /// how many local variables inside this function		
		REVERSE, /// used for operations that can be reversed like substraction,division,power_of,modulus etc...		
		BINARY_OPERATOR,
		STACK_RETURN,
		RETURN_VALUE,
		GOTO,
		GOTO_IF,
		GOTO_IF_NOT,
		CALL,
		CALL_STD,
		LOAD_LIBRARY,
		FUNCTION,
		FUNCTION_END,
		LABEL,
	} Bytecode;

	typedef enum : int8_t
	{
		ADDITION,
		SUBSTRACTION,
		MULTIPLICATION,
		DIVISION,
		MODULUS,
		POWER_OF,
		EQUALS,
		EQUALS_NOT,
		GREATER_THAN,
		GREATER_OR_EQUAL,
		LOWER_OR_EQUAL,
		LOWER_THAN,
		DEFAULT,
	} BinaryOperators;

	inline bool IsArithmeticOperator(BinaryOperators OP)
	{
		return OP == ADDITION || OP == SUBSTRACTION || OP == MULTIPLICATION || OP == DIVISION;
	}

	inline bool IsComparisonOperator(BinaryOperators OP)
	{
		return OP == EQUALS || OP == EQUALS_NOT || OP == GREATER_THAN || OP == GREATER_OR_EQUAL || OP == LOWER_OR_EQUAL || OP == LOWER_THAN;
	}

	inline bool IsLoad(Bytecode OP)
	{
		return OP == LOAD_CONST || OP == LOAD_INT || OP == LOAD_PARAMETER || OP == LOAD_VARIABLE || OP == LOAD_FROM_STACK;
	}

	typedef struct
	{
		Bytecode* Instructions;
		unsigned int* Indices; /// can be ptrs
		DataTypes::Types* DataSize;
		size_t InstructionsSize; /// same as indices size

		void** Literals;
		DataTypes::DataType* LiteralType;
		size_t LiteralSize;
	} BytecodePackage;
}

#endif

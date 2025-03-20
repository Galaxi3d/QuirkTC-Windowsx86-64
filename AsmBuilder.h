#ifndef ASMBUILDER_H
#define ASMBUILDER_H
#include "Bytecode.h"
#include "AsmHelper.h"

std::string BuildAssembly(Bytecode::BytecodePackage* Package);

namespace
{	
	namespace Conventions
	{
		const std::string IntegerArguments[] = {"rcx","rdx","r8","r9"};
		int8_t IntegerArgumentPos = 0;

		constexpr auto DoubleArguments = { "xmm0l","xmm1l","xmm2l","xmm3l" };
		int8_t DoubleArgumentPos = 0;

		const std::string MathRegisters[] = {"eax","edx"};

		uint8_t MathRegisterPos = 0; /// eax,ecx
	}

	std::string* AssemblySource;
	Bytecode::BytecodePackage* Package;	
	size_t CurrentStackLocation = 8; /// theres automatically stuff before
	size_t ParameterAmount = 0;	

	bool LoadConst(size_t LiteralIndex, size_t InstructioIndex); /// bool is if it does a math op

	bool LoadParameter(size_t Index);

	bool LoadFromStack(size_t Index);

	void SetVariable();

	void SetParameter();

	void BinaryOperation(size_t Index, std::string Value);

    void GenerateStringLiterals();

	void LoadParameters();

	void GenerateFunctionPrologue();

	void GenerateFunctionEpilogue();



	void AddLine(std::string First, std::string Second = "", std::string Third = "");

	void Output()
	{
		printf("%s\n", AssemblySource->c_str());
	}
}

#endif // !ASMBUILDER_H

#ifndef ASMBUILDER_H
#define ASMBUILDER_H
#include "Assembly.h"
#include "AsmHelper.h"

std::string BuildAssembly(std::unique_ptr<Bytecode::BytecodePackage> Package);
void AddLine(std::string First, std::string Second = "", std::string Third = "");

namespace
{			
	bool IsUpcomingCondition(size_t Index);	

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


	void Output()
	{
		printf("%s\n", Assembly::AssemblySource->c_str());
	}
}

#endif // !ASMBUILDER_H

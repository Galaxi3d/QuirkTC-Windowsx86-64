#include "AsmLoads.h"
#include "AsmBuilder.h"

bool AsmLoads::IsUpcomingCondition() const
{
	return Bytecode::IsLoad(Assembly::Package->Instructions[Index + 1])
		&& Bytecode::IsComparisonOperator((Bytecode::BinaryOperators)Assembly::Package->Indices[Index + 2]);
}

/// <summary>
/// Checks if the upcoming bytecode is another load and then a math operator
/// </summary>
/// <returns></returns>
bool AsmLoads::IsUpcomingMath() const
{
	return Bytecode::IsLoad(Assembly::Package->Instructions[Index + 1])
		&& Bytecode::IsArithmeticOperator((Bytecode::BinaryOperators)Assembly::Package->Indices[Index + 2]);
}

inline bool AsmLoads::IsUpcomingSetToStack() const
{
	return Assembly::Package->Instructions[Index + 1] == Bytecode::SET_VARIABLE;	
}

inline bool AsmLoads::IsUpcomingSetToParameter() const
{
	return Assembly::Package->Instructions[Index + 1] == Bytecode::SET_PARAMETER;
}

inline bool AsmLoads::IsUpcomingReplacingVariable() const /// TODO: DISTINCITON BETWEEN MODIFY AND REPLACE FROM STACk
{
	return Assembly::Package->Instructions[Index + 1] == Bytecode::MODIFY_FROM_STACK; /// todo: later we can simplfi x = x + 100 to x+=100 directly into asm
}

inline bool AsmLoads::IsUpcomingModifyVariable() const
{
	return Assembly::Package->Instructions[Index + 1] == Bytecode::BINARY_OPERATOR && Assembly::Package->Instructions[Index + 2] == Bytecode::MODIFY_FROM_STACK;
}

uint8_t AsmLoads::GenericLoad()
{	
	if (IsUpcomingMath())
	{
		BinaryOperation();
		return 2; /// compiles LOAD,LOAD,BINARY_OPERATOR
	}
	else if (IsUpcomingCondition())
	{
		Condtional();
		return 3; /// compiles LOAD,LOAD,COMPARISON,JUMP
	}	
	else if (IsUpcomingSetToStack())
	{
		SetToStack();
		return 1; /// compiles LOAD,SET_VARIABLE
	}
	else if (IsUpcomingSetToParameter())
	{
		SetToParameter(); /// compiles LOAD,SET_PARAMETER
		return 1;
	}
	else if (IsUpcomingReplacingVariable())
	{
		ReplaceVariable(); /// compiles LOAD, MODIFY_FROM_STACK
		return 1;
	}
	else if (IsUpcomingModifyVariable())
	{
		ModifyVariable(); /// compiles LOAD, BINARY OPERATOR ,MODIFY_FROM_STACK
		return 2;
	}

	/// default
	
	AddLine("mov",Conventions::MathRegisters[Conventions::MathRegisterPos++],Address);
	//std::cout << "register at " << std::to_string(Conventions::MathRegisterPos) << '\n';
	return 0;
}

inline void AsmLoads::BinaryOperation() const
{
	AddLine("mov", Conventions::MathRegisters[0], Address); /// first load the value to eax

	Bytecode::BinaryOperators BinaryOperator = (Bytecode::BinaryOperators)Assembly::Package->Indices[Index + 2];
	std::string Operator = AsmHelper::GetArithmeticOperator(BinaryOperator);
	
	std::string SecondAddress;

	if (Assembly::Package->Instructions[Index + 1] == Bytecode::LOAD_CONST)
	{
		SecondAddress = GetConstAddress(Index + 1);
	}
	else
	{
		SecondAddress = GetStackAddress(Index + 1);
	}

	AddLine(Operator, Conventions::MathRegisters[0], SecondAddress);

	Conventions::MathRegisterPos = 0;
}

inline void AsmLoads::Condtional() const
{
	/// in theory, we should never has to compare 2 constants since the compiler should have optimized that already
	/// the only possible combinations are
	/// CONST, STACK -> we can't have this for cmp so we have to set to another register: eax
	/// STACK, CONST
	/// STACK, STACK	

	std::string FirstAddress = Address; /// STACK, ___ is accounted for here
	std::string SecondAddress;

	/// CONST,___ or STACK,___
	AddLine("mov", Conventions::MathRegisters[0], FirstAddress);
	FirstAddress = Conventions::MathRegisters[Conventions::MathRegisterPos++];	
	

	if (Assembly::Package->Instructions[Index + 1] == Bytecode::LOAD_CONST) /// ____, CONST
	{
		SecondAddress = GetConstAddress(Index + 1);
	}
	else /// ____, STACK
	{		
		AddLine("mov", Conventions::MathRegisters[Conventions::MathRegisterPos], GetStackAddress(Index + 1));
		SecondAddress = Conventions::MathRegisters[Conventions::MathRegisterPos++];		
	}

	AddLine("cmp", FirstAddress, SecondAddress);

	/// Index + 2 is the COMPARING instruction
	/// Index + 3 is the JUMP instruction

	bool JumpIfTrue = Assembly::Package->Instructions[Index + 3] == Bytecode::GOTO_IF 
	|| Assembly::Package->Indices[Index + 3] == Bytecode::GOTO_IF_NOT;

	std::string Label = AsmHelper::GetLabelName(Assembly::Package->Indices[Index + 3]);
	
	std::string Jump = AsmHelper::GetConditionalJump((Bytecode::BinaryOperators)Assembly::Package->Indices[Index + 2],JumpIfTrue);

	AddLine(Jump, Label);	 

	Conventions::MathRegisterPos = 0;
}

inline void AsmLoads::SetToStack()
{
	if (Assembly::Package->DataSize[this->Index] == Bytecode::DataTypes::Types::STRING)
	{
		AddLine("lea", "rax", Address);
		Address = "rax";
	}
	else
	{
		AddLine("mov", "rax", Address);
		Address = "rax";
	}

	AddLine("mov", AsmHelper::CreateStackAddress(Assembly::StackLocations.top()), Address);
	
	Assembly::StackLocations.top() += 8; /// 8 bytes
	Conventions::MathRegisterPos = 0;
}

inline void AsmLoads::SetToParameter() const 
{
	std::string Address = Conventions::IntegerArguments[Conventions::IntegerArgumentPos++];
	if (Assembly::Package->DataSize[this->Index] == Bytecode::DataTypes::Types::STRING && IsAdressAStackAddress == false)
	{
		AddLine("lea", Address, this->Address);
		return;
	}

	AddLine("mov", Address, this->Address);
	Conventions::MathRegisterPos = 0;
}

inline void AsmLoads::ReplaceVariable() 
{
	if (Assembly::Package->DataSize[this->Index] == Bytecode::DataTypes::Types::STRING)
	{
		AddLine("lea", "rax", Address);
		Address = "rax";
	}
	else
	{
		AddLine("mov", "rax", Address);
		Address = "rax";
	}

	AddLine("mov", AsmLoads::GetStackAddress(Index),Address);
	Conventions::MathRegisterPos = 0;
}

inline void AsmLoads::ModifyVariable() /// this should be IMPOSSIBLE FOR A STRING
{
	std::string Operator = AsmHelper::GetArithmeticOperator((Bytecode::BinaryOperators)Assembly::Package->Indices[Index + 1]);

	AddLine(Operator, GetStackAddress(this->Index + 2), Address);
}

inline std::string AsmLoads::GetStackAddress(size_t Index)
{
	if (Assembly::ParameterAmount > Assembly::Package->Indices[Index]) /// is rsp and not rbp
	{
		return std::string(Conventions::DEFAULT_ACCESSER) + " [rsp+" + std::to_string(Assembly::Package->Indices[Index] * 8) + "]";
	}

	return AsmHelper::CreateStackAddress(INDEX_TO_STACK_SIZE(Assembly::Package->Indices[Index]));

	//return "[rbp-" + std::to_string(Assembly::Package->Indices[Index] * 8 + 8) + "]";
}

inline std::string AsmLoads::GetConstAddress(size_t Index)
{
	size_t LiteralIndex = Assembly::Package->Indices[Index];
	std::string Literal = "";

	switch (Assembly::Package->LiteralType[LiteralIndex].Type)
	{
	case Bytecode::DataTypes::Types::INT_32:
	{
		int32_t value = *(int32_t*)Assembly::Package->Literals[LiteralIndex];
		Literal = std::to_string(value);		
		break;
	}
	case Bytecode::DataTypes::Types::STRING:
	{
		/*std::string StringValue = (char*)Assembly::Package->Literals[LiteralIndex];*/
		Literal = AsmHelper::CreateStringName(LiteralIndex); /// since we have to reference the string
		break;
	}
	case Bytecode::DataTypes::Types::BOOL:
	{
		bool value = *(bool*)Assembly::Package->Literals[LiteralIndex];		
		Literal = value ? "1" : "0";
		break;
	}
	default:
		throw std::runtime_error("Unsupported literal type");
		break;
	}

	return Literal;
}

uint8_t AsmLoads::LoadFromStack()
{
	Address = GetStackAddress(Index);	
	IsAdressAStackAddress = true; 
	return GenericLoad();    
}

uint8_t AsmLoads::LoadConst()
{
	Address = GetConstAddress(Index);
	IsAdressAStackAddress = false;
	return GenericLoad();    
}

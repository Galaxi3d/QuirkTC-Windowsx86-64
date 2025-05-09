#include "AsmBuilder.h"
#include "AsmLoads.h"
#include "StandardLibraryCompiler.h"
#include <vector>

namespace
{
    inline size_t CurrentInstructionIndex = 1;

    /*bool IsUpcomingCondition(size_t Index)
    {
        return Bytecode::IsLoad(Assembly::Package->Instructions[Index + 1]) && Bytecode::IsComparisonOperator((Bytecode::BinaryOperators)Assembly::Package->Indices[Index + 2]);        
    }*/


    //bool LoadConst(size_t LiteralIndex, size_t InstructioIndex)
    //{
    //    bool IsNextBytecodeMath = Assembly::Package->Instructions[InstructioIndex + 1] == Bytecode::BINARY_OPERATOR;

    //    switch (Assembly::Package->LiteralType[LiteralIndex].Type)
    //    {
    //        case Bytecode::DataTypes::Types::INT:
    //        {
    //            int32_t value = *(int32_t*)Assembly::Package->Literals[LiteralIndex];
    //            std::string StringValue = std::to_string(value);

    //            if (!IsNextBytecodeMath)
    //            {
    //                AddLine("mov",Conventions::MathRegisters[Conventions::MathRegisterPos++],StringValue);
    //                //std::cout << "Goes up \n";
    //            }
    //            else
    //            {
    //                BinaryOperation(InstructioIndex+1,StringValue);
    //                //std::cout << "does binary operation \n";
    //                return true;
    //            }
    //            break;        
    //        }
    //    default:
    //        break;
    //    }

    //    return false;
    //}

    //void BinaryOperation(size_t Index, std::string Value)
    //{
    //    Bytecode::BinaryOperators BinaryOperator = (Bytecode::BinaryOperators)Assembly::Package->Indices[Index];        
    //    std::string Operator = AsmHelper::GetArithmeticOperator(BinaryOperator);

    //    //std::cout << std::to_string(Conventions::MathRegisterPos) << '\n';

    //    AddLine(Operator, Conventions::MathRegisters[Conventions::MathRegisterPos-1], Value);
    //    std::cout << "Goes down \n";
    //}
    
    void GenerateStringLiterals()
    {        
        for (size_t i = 0; i < Assembly::Package->LiteralSize; i++)
        {
            if (Assembly::Package->LiteralType[i].Type == Bytecode::DataTypes::Types::STRING)
            {
                char* Cstr = (char*)Assembly::Package->Literals[i];

                std::string String = AsmHelper::CreateStringName(i) + " BYTE \"" + Cstr + "\", 0"; /// TODO: MAKE MORE VERSATILE

                Assembly::AddToDataSection(String);
            }
        }
    }

    void LoadParameters()
    {       
        size_t paramCount = Assembly::ParameterAmount > 3 ? 3 : Assembly::ParameterAmount;
        for (size_t i = 0; i < paramCount; i++)
        {
            std::string Address = std::string(Conventions::DEFAULT_ACCESSER) + " [rsp+" + std::to_string(i * 8) + "]";

            AddLine("mov",Address,Conventions::IntegerArguments[i]);
        }

        if (Assembly::ParameterAmount > 4) /// flm
        {
            printf("nah too lazy to write for stack crap ");
        }

		//Assembly::ParameterAmount = 0;
    }

    void GenerateFunctionPrologue()
    {
        AddLine("push", "rbp");
        AddLine("mov", "rbp", "rsp");
        AddLine("sub","rsp","32"); /// required for 64 bits
		AddLine("sub", "rsp", "8"); /// FOR STACK ALIGNMENT WINDOWS
        Assembly::StackLocations.push(8); /// initial stack location 
    }

    void GenerateFunctionEpilogue()
    {
        AddLine("mov","rsp","rbp");
        AddLine("pop","rbp");
        AddLine("ret");
        Assembly::StackLocations.pop();
		Assembly::ParameterAmount = 0;
    }

    /*bool LoadParameter(size_t Index)
    {
        bool IsNextBytecodeMath = Assembly::Package->Instructions[Index + 1] == Bytecode::BINARY_OPERATOR;

        std::string Address = "[rsp+" + std::to_string(Assembly::Package->Indices[Index] * 8 + 8) + "]";

        if (IsNextBytecodeMath)
        {
            BinaryOperation(Index+1,Address);
            return true;
        }
        else
        {
            AddLine("mov",Conventions::MathRegisters[Conventions::MathRegisterPos++], Address);
            return false;
        }
    }*/

   /* bool LoadFromStack(size_t Index)
    {
        bool IsNextBytecodeMath = Assembly::Package->Instructions[Index + 1] == Bytecode::BINARY_OPERATOR;
		
        std::string Address = "[rbp-" + std::to_string(Assembly::Package->Indices[Index] * 8 + 8) + "]";

        if (IsNextBytecodeMath)
        {
            BinaryOperation(Index + 1, Address);
            return true;
        }
        else
        {
            AddLine("mov", Conventions::MathRegisters[Conventions::MathRegisterPos++], Address);
            return false;
        }
    }*/

    void SetVariable()            
    {        
        /*if (Assembly::Package->DataSize[CurrentInstructionIndex] == Bytecode::DataTypes::Types::STRING)
        {
            AddLine("lea","rax",AsmHelper::)
        }*/


        AddLine("mov", AsmHelper::CreateStackAddress(Assembly::StackLocations.top()), Conventions::MathRegisters[0]);
        Assembly::StackLocations.top() += 8; /// 8 bytes

        //std::string Address = "[rbp-" + std::to_string(Assembly::StackLocations.top()) + "]";

        //Assembly::StackLocations.top() += 8; /// 8 bytes

        //AddLine("mov", Address, Conventions::MathRegisters[0]);

        //Conventions::MathRegisterPos = 0;
    }

    void SetParameter()
    {
		std::string Address = Conventions::IntegerArguments[Conventions::IntegerArgumentPos++];
		AddLine("mov", Address, Conventions::MathRegisters[0]); /// inefficient cuz it could be direct no
		Conventions::MathRegisterPos = 0;
    }

}

void AddLine(std::string First, std::string Second, std::string Third)
{
    Assembly::AssemblySource->append(First);
    
    if (!Second.empty())
    {
        Assembly::AssemblySource->append(" " + Second);

        if (!Third.empty())
        {
            Assembly::AssemblySource->append(", " + Third);

        }

    }

    Assembly::AssemblySource->append("\n");
}

void GenerateCode()
{
    AddLine(".code");
    AddLine("main PROC");
    GenerateFunctionPrologue();

    std::string CurrentFunctionName = "main";
    std::string CurrentRegister = "rax";
	
    //size_t FunctionCount = 0;
    
    Bytecode::BinaryOperators FirstOperator = Bytecode::DEFAULT;
    
    unsigned int SkipIterations = 0;    

    for (size_t i = 1; i < Assembly::Package->InstructionsSize; i++) /// since first instruction MUST be [FUNCTION]
    {
        if (SkipIterations > 0)
        {
            SkipIterations--;
            continue;
        }
        
        switch (Assembly::Package->Instructions[i])
        {
        case Bytecode::LOAD_CONST:
			SkipIterations = AsmLoads::AsmLoads(i).LoadConst();            
            break;
        case Bytecode::LOAD_FROM_STACK:
			SkipIterations = AsmLoads::AsmLoads(i).LoadFromStack();
			break;
        case Bytecode::FUNCTION:
            CurrentFunctionName = AsmHelper::CreateFunctionName(Assembly::Package->Indices[i]);
            AddLine(CurrentFunctionName, "PROC");
            GenerateFunctionPrologue();
            break;
        case Bytecode::FUNCTION_END:
            GenerateFunctionEpilogue();
            AddLine(CurrentFunctionName,"ENDP");
            break;
        case Bytecode::GOTO:
            AddLine("jmp", AsmHelper::GetLabelName(Assembly::Package->Indices[i]));
            break;
        case Bytecode::LABEL:
            AddLine(AsmHelper::CreateLabelName(Assembly::Package->Indices[i]) + ":");
            break;
        case Bytecode::CALL:
            AddLine("call", AsmHelper::CreateFunctionName(Assembly::Package->Indices[i]));
			Conventions::IntegerArgumentPos = 0; /// reset the integer argument position
            break; 
        case Bytecode::MODIFY_FROM_STACK:
            AddLine("mov", AsmHelper::CreateStackAddress(INDEX_TO_STACK_SIZE(Assembly::Package->Indices[i])),Conventions::MathRegisters[0]);
            break;
        case Bytecode::SET_VARIABLE:

			SetVariable();

            
            break;
        /*case Bytecode::SET_PARAMETER:
			SetParameter();
            break;  */      
        case Bytecode::STACK_ALLOCATE:
            AddLine("sub", "rsp", std::to_string(Assembly::Package->Indices[i] * 8)); /// 8 cuz 8 byte variables
            break;
        case Bytecode::LOAD_PARAMETERS:
            Assembly::ParameterAmount = Assembly::Package->Indices[i];
            LoadParameters();
            break;  
        case Bytecode::RETURN_VALUE:
			Conventions::MathRegisterPos = 0;
			break;
        case Bytecode::CALL_STD:
			CompileStandardLibraryFunction(Assembly::Package->Indices[i]);
            break;        
        /*case Bytecode::BINARY_OPERATOR: /// all the loading bytecode should do the binary_opeartions for this so the only case should be register,register
        {

            Bytecode::BinaryOperators BinaryOperator = (Bytecode::BinaryOperators)Assembly::Package->Indices[i];

            if (BinaryOperator == Bytecode::BinaryOperators::MODULUS)
            {
                break;
            }
            
            if (Bytecode::IsArithmeticOperator(BinaryOperator))
            {
                AddLine(AsmHelper::GetArithmeticOperator(BinaryOperator), Conventions::MathRegisters[0], Conventions::MathRegisters[1]);
                Conventions::MathRegisterPos = 0; /// should be back to 0
                std::cout << "Goes down \n";
            }
            break;
        }*/
        default:
            break;
        }

		CurrentInstructionIndex++; /// TODO, very stupid make this better
    }

    AddLine("END");
}
 

std::string BuildAssembly(std::unique_ptr<Bytecode::BytecodePackage> Package)
{
    Assembly::AssemblySource = std::make_unique<std::string>();
    Assembly::AssemblyPrologue = std::make_unique<std::string>();
    Assembly::AssemblyDataSection = std::make_unique<std::string>(".data\n");
    Assembly::Package = std::move(Package);
    GenerateStringLiterals();
    GenerateCode();
	//delete Package;
    return *Assembly::AssemblyPrologue + *Assembly::AssemblyDataSection + *Assembly::AssemblySource;
}

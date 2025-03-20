#include "AsmBuilder.h"
#include <vector>

namespace
{

    bool LoadConst(size_t LiteralIndex, size_t InstructioIndex)
    {
        bool IsNextBytecodeMath = Package->Instructions[InstructioIndex + 1] == Bytecode::BINARY_OPERATOR;

        switch (Package->LiteralType[LiteralIndex].Type)
        {
            case Bytecode::DataTypes::Types::INT:
            {
                int32_t value = *(int32_t*)Package->Literals[LiteralIndex];
                std::string StringValue = std::to_string(value);

                if (!IsNextBytecodeMath)
                {
                    AddLine("mov",Conventions::MathRegisters[Conventions::MathRegisterPos++],StringValue);
                    //std::cout << "Goes up \n";
                }
                else
                {
                    BinaryOperation(InstructioIndex+1,StringValue);
                    //std::cout << "does binary operation \n";
                    return true;
                }
                break;        
            }
        default:
            break;
        }

        return false;
    }

    void BinaryOperation(size_t Index, std::string Value)
    {
        Bytecode::BinaryOperators BinaryOperator = (Bytecode::BinaryOperators)Package->Indices[Index];        
        std::string Operator = AsmHelper::GetArithmeticOperator(BinaryOperator);

        //std::cout << std::to_string(Conventions::MathRegisterPos) << '\n';

        AddLine(Operator, Conventions::MathRegisters[Conventions::MathRegisterPos-1], Value);
        std::cout << "Goes down \n";
    }

    
    void GenerateStringLiterals()
    {
        bool IsFirst = true;

        for (size_t i = 0; i < Package->LiteralSize; i++)
        {
            if (Package->LiteralType[i].Type == Bytecode::DataTypes::Types::STRING)
            {
                if (IsFirst)
                {
                    AddLine(".data");
                    IsFirst = false;
                }

                char* Cstr = (char*)Package->Literals[i];

                std::string String = AsmHelper::CreateStringName(i) + " BYTE \"" + Cstr + "\", 0"; /// TODO: MAKE MORE VERSATILE

                AddLine(String);
            }
        }
    }

    void LoadParameters()
    {       
        for (size_t i = 0; i < 3; i++)
        {
            std::string Address = "[rsp+ " + std::to_string(i * 8) + "]";

            AddLine("mov",Address,Conventions::IntegerArguments[i]);
        }

        if (ParameterAmount > 4) /// flm
        {
            printf("nah too lazy to write for stack crap ");
        }

    }

    void GenerateFunctionPrologue()
    {
        AddLine("push", "rbp");
        AddLine("mov", "rbp", "rsp");
        AddLine("sub","rsp","32"); /// required for 64 bits
    }

    void GenerateFunctionEpilogue()
    {
        AddLine("mov","rsp","rbp");
        AddLine("pop","rbp");
        AddLine("ret");
    }

    bool LoadParameter(size_t Index)
    {
        bool IsNextBytecodeMath = Package->Instructions[Index + 1] == Bytecode::BINARY_OPERATOR;

        std::string Address = "[rsp+" + std::to_string(Package->Indices[Index] * 8) + "]";

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
    }

    bool LoadFromStack(size_t Index)
    {
        bool IsNextBytecodeMath = Package->Instructions[Index + 1] == Bytecode::BINARY_OPERATOR;

        std::string Address = "[rbp-" + std::to_string(Package->Indices[Index] * 8 + 8) + "]";

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
    }

    void SetVariable()
    {
        std::string Address = "[rbp-" + std::to_string(CurrentStackLocation) + "]";

        CurrentStackLocation += 8; /// 8 bytes

        AddLine("mov", Address, "eax");

        Conventions::MathRegisterPos = 0;
    }

    void SetParameter()
    {


    }

    void AddLine(std::string First, std::string Second, std::string Third)
    {
        AssemblySource->append(First);

        if (!Second.empty())
        {
            AssemblySource->append(" " + Second);

            if (!Third.empty())
            {
                AssemblySource->append(", " + Third);
            }

        }

        AssemblySource->append("\n");
    }
}

/// THE PROTOCOLE
/// 
/// 
/// 
/// 

void GenerateCode()
{
    AddLine(".code");
    AddLine("main PROC");
    GenerateFunctionPrologue();

    std::string CurrentFunctionName = "main";

    std::string CurrentRegister = "rax";

    size_t FunctionCount = 0;

    Bytecode::BinaryOperators FirstOperator = Bytecode::DEFAULT;
    
    bool SkipAnIteration = false;    

    for (size_t i = 1; i < Package->InstructionsSize; i++) /// since first instruction MUST be [FUNCTION]
    {
        if (SkipAnIteration)
        {
            SkipAnIteration = false;
            continue;
        }   

        switch (Package->Instructions[i])
        {
        case Bytecode::FUNCTION:
            CurrentFunctionName = AsmHelper::CreateFunctionName(++FunctionCount);
            AddLine(CurrentFunctionName, "PROC");
            GenerateFunctionPrologue();
            break;
        case Bytecode::FUNCTION_END:
            GenerateFunctionEpilogue();
            AddLine(CurrentFunctionName,"ENDP");
            break;
        case Bytecode::GOTO:
            AddLine("jmp", AsmHelper::GetLabelName(Package->Indices[i]));
            break;
        case Bytecode::LABEL:
            AddLine(AsmHelper::CreateLabelName(Package->Indices[i]) + ":");
            break;
        case Bytecode::CALL:
            AddLine("call", AsmHelper::CreateFunctionName(Package->Indices[i]));
            break; 
        case Bytecode::SET_VARIABLE:
            SetVariable();
            //std::cout << "Goes down for set var \n";
            break;
        case Bytecode::SET_PARAMETER:
            Conventions::MathRegisterPos--;
            //std::cout << "Goes down for set param \n";
            break;
        case Bytecode::LOAD_FROM_STACK:

            if (ParameterAmount == 0) /// default stack [rbp-]
            {
                if (LoadFromStack(i))
                {
                    SkipAnIteration = true;
                }
            }
            else if (Package->Indices[i] <= ParameterAmount) /// [rsp+]
            {
                if (LoadParameter(i))
                {
                    SkipAnIteration = true;
                }
            }            
            continue;
        case Bytecode::STACK_ALLOCATE:
            AddLine("sub", "rsp", std::to_string(Package->Indices[i] * 8)); /// 8 cuz 8 byte variables
            break;
        case Bytecode::LOAD_PARAMETERS:
            ParameterAmount = Package->Indices[i];
            LoadParameters();
            break;
        case Bytecode::LOAD_CONST:
        {
            bool DoesMathOp = LoadConst(Package->Indices[i],i); /// kinda stupid ngl

            if (DoesMathOp)
            {
                SkipAnIteration = true;
                continue;
            }

            break;
        }
        case Bytecode::BINARY_OPERATOR: /// all the loading bytecode should do the binary_opeartions for this so the only case should be register,register
        {

            Bytecode::BinaryOperators BinaryOperator = (Bytecode::BinaryOperators)Package->Indices[i];

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
        }
        default:



            break;
        }
    }

    AddLine("END");
}
 

std::string BuildAssembly(Bytecode::BytecodePackage* Package)
{
    AssemblySource = new std::string();
    ::Package = Package;

    GenerateStringLiterals();
    GenerateCode();
    Output();

    return *AssemblySource;
}

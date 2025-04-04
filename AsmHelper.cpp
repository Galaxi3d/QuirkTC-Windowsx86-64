#include "AsmHelper.h"
#include <sstream>

namespace AsmHelper
{  
    std::string CreateName(const std::string& prefix, size_t value)
    {
        return prefix + std::to_string(value);
    }

    std::string CreateStringName(size_t value)
    {
        return CreateName("s", value);
    }

    std::string CreateFunctionName(size_t value)
    {
        return CreateName("f", value);
    }

    std::string CreateLabelName(size_t value)
    {
        return CreateName("l", value);
    }

    std::string GetLabelName(size_t value)
    {
        return CreateLabelName(value);
    }

    std::string GetArithmeticOperator(Bytecode::BinaryOperators BinaryOperator)
    {   
        switch (BinaryOperator)
        {
            case Bytecode::BinaryOperators::ADDITION: return "add";
            case Bytecode::BinaryOperators::SUBSTRACTION: return "sub";
            case Bytecode::BinaryOperators::DIVISION: return "div";
            case Bytecode::BinaryOperators::MULTIPLICATION: return "imul";
            default: return "";
        }
    }    

    std::string GetConditionalJump(Bytecode::BinaryOperators BinaryOperators, bool IsJumpIf)
    {
        switch (BinaryOperators)
        {
        case Bytecode::BinaryOperators::EQUALS: return IsJumpIf ? "je" : "jne";
        case Bytecode::BinaryOperators::EQUALS_NOT: return IsJumpIf ? "jne" : "je";
        case Bytecode::BinaryOperators::GREATER_THAN: return IsJumpIf ? "jg" : "jle";
        case Bytecode::BinaryOperators::GREATER_OR_EQUAL: return IsJumpIf ? "jge" : "jl";
        case Bytecode::BinaryOperators::LOWER_OR_EQUAL: return IsJumpIf ? "jle" : "jg";
        case Bytecode::BinaryOperators::LOWER_THAN: return IsJumpIf ? "jl" : "jge";
        default: return "";
        }
    }

    std::string CreateStackAddress(size_t Address)
    {
        std::ostringstream oss;
        oss << Conventions::MemoryAccess[0] << " [rbp-" << Address << "]";
        return oss.str();
    }
}

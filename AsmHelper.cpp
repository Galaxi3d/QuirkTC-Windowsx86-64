#include "AsmHelper.h"
#include <unordered_map>

namespace AsmHelper
{  


    std::string CreateStringName(size_t value)
    {
        std::string String = "s";
        String.append(std::to_string(value));
        return String;
    }

    std::string CreateFunctionName(size_t value)
    {
        std::string String = "f";
        String.append(std::to_string(value));
        return String;
    }

    std::string CreateLabelName(size_t value)
    {
        std::string String = "l";
        String.append(std::to_string(value));
        return String;
    }

    std::string GetLabelName(size_t value)
    {
        return CreateLabelName(value);
    }

    
    std::string GetArithmeticOperator(Bytecode::BinaryOperators BinaryOperator)
    {
        static std::unordered_map<Bytecode::BinaryOperators, std::string> BinaryMap =
        {
            {Bytecode::BinaryOperators::ADDITION, "add"},
            {Bytecode::BinaryOperators::SUBSTRACTION, "sub"},
            {Bytecode::BinaryOperators::DIVISION, "div"},
            {Bytecode::BinaryOperators::MULTIPLICATION, "imul"},
        };

        auto it = BinaryMap.find(BinaryOperator);

        return it->second;
    }
}

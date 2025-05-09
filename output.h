#ifndef OUTPUT_H
#define OUTPUT_H
#include "Bytecode.h"
#include <string>
#include <unordered_map>
#include <iostream>

const std::unordered_map<Bytecode::Bytecode, std::string> BytecodeToString = {
    //{Bytecode::LOAD_CONST, "LOAD_CONST"},
    //{Bytecode::LOAD_INT, "LOAD_INT"},
    //{Bytecode::SET_PARAMETER, "SET_PARAMETER"},
    //{Bytecode::LOAD_PARAMETER, "LOAD_PARAMETER"},
    //{Bytecode::SET_VARIABLE, "SET_VARIABLE"},
    //{Bytecode::LOAD_VARIABLE, "LOAD_VARIABLE"},
    //{Bytecode::MODIFY_FROM_STACK,"MODIFY_FROM_STACK"},
    //{Bytecode::REVERSE,"REVERSE"},
    //{Bytecode::LOAD_PARAMETERS,"LOAD_PARAMETERS"},
    //{Bytecode::BINARY_OPERATOR,"BINARY_OPERATOR"},
    //{Bytecode::STACK_ALLOCATE,"STACK_ALLOCATE"},
    //
    //{Bytecode::STACK_RETURN, "STACK_RETURN"},
    //{Bytecode::RETURN_VALUE, "RETURN_VALUE"},

    //{Bytecode::GOTO, "GOTO"},
    //{Bytecode::GOTO_IF, "GOTO_IF"},
    //{Bytecode::GOTO_IF_NOT, "GOTO_IF_NOT"},

    //{Bytecode::CALL,"CALL"},
    ////{Bytecode::CALL_EXTERN, "CALL_EXTERN"},
    //{Bytecode::LOAD_LIBRARY, "LOAD_LIBRARY"},
    //{Bytecode::FUNCTION, "FUNCTION"},
    //{Bytecode::FUNCTION_END, "FUNCTION_END"},
    //{Bytecode::LABEL, "LABEL"},
    //{Bytecode::LOAD_FROM_STACK, "LOAD_FROM_STACK"},
};

const std::unordered_map<Bytecode::DataTypes::Types, std::string> TypeToStringMap = {
    //{Bytecode::DataTypes::Types::INT, "INT"},
    //{Bytecode::DataTypes::Types::FLOAT, "FLOAT"},
    //{Bytecode::DataTypes::Types::STRING, "STRING"},
    //{Bytecode::DataTypes::Types::CHAR, "CHAR"},
    //{Bytecode::DataTypes::Types::VOID, "VOID"},
    //{Bytecode::DataTypes::Types::BOOL, "BOOL"},
    //{Bytecode::DataTypes::Types::LIST, "LIST"},
    //{Bytecode::DataTypes::Types::STACK, "STACK"}
};

void Out(Bytecode::BytecodePackage* Package)
{
    for (int i = 0; i < Package->LiteralSize; i++)
    {
        std::string String = TypeToStringMap.find(Package->LiteralType[i].Type)->second;

        std::cout << i << '\t' << String << '\t';

        void* Literal = Package->Literals[i];

        switch (Package->LiteralType[i].Type)
        {
        case Bytecode::DataTypes::Types::INT_32:
            std::cout << *(uint32_t*)Literal << '\n';
            continue;
        case Bytecode::DataTypes::Types::STRING:
            std::cout << (char*)Literal << '\n';
            continue;
        default:
            std::cout << "none rn \n";
            continue;
        }

    }

    std::cout << '\n';


    for (int i = 0; i < Package->InstructionsSize;i++)
    {
        auto it = BytecodeToString.find(Package->Instructions[i]);

        if (it == BytecodeToString.end())
        {
            std::cout << "CORRUPTED: numerical value is " + std::to_string((uint8_t)Package->Instructions[i]) + " \n";
        }
        else
        {
            std::string Bytecode = it->second;
            std::string Index = std::to_string(Package->Indices[i]);

            std::cout << Bytecode + '\t' + Index << '\n';
        }
    }
}


#endif
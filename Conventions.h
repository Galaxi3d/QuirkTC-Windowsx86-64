#ifndef CONVENTIONS_H  
#define CONVENTIONS_H  
#include <string>  
#include <array>  
#include <unordered_map>  

namespace Conventions /// TODO CREATE A SMARTER TABLE FOR ALL TYPES AND REGISTERS  
{      
   constexpr const char* DEFAULT_ACCESSER = "QWORD PTR";  

   inline const std::unordered_map<Bytecode::DataTypes::Types, std::vector<const char*>> IntegerTypeToRegisterMap = {  
       { Bytecode::DataTypes::Types::UINT_64, { "rax", "rbx" } },  
       { Bytecode::DataTypes::Types::UINT_32, { "eax", "ebx" } },  
       { Bytecode::DataTypes::Types::UINT_16, { "ax", "bx" } },  
       { Bytecode::DataTypes::Types::UINT_8, { "al", "bl" } },  
       { Bytecode::DataTypes::Types::INT_64, { "rax", "rbx" } },  
       { Bytecode::DataTypes::Types::INT_32, { "eax", "ebx" } },  
       { Bytecode::DataTypes::Types::INT_16, { "ax", "bx" } },  
       { Bytecode::DataTypes::Types::INT_8, { "al", "bl" } }  
   };  
   inline const std::unordered_map<Bytecode::DataTypes::Types, std::vector<const char*>> MemoryAccessMap = {    
       { Bytecode::DataTypes::Types::UINT_64, { "QWORD PTR" } },    
       { Bytecode::DataTypes::Types::UINT_32, { "DWORD PTR" } },    
       { Bytecode::DataTypes::Types::UINT_16, { "WORD PTR" } },    
       { Bytecode::DataTypes::Types::UINT_8, { "BYTE PTR" } },    
       { Bytecode::DataTypes::Types::INT_64, { "QWORD PTR" } },    
       { Bytecode::DataTypes::Types::INT_32, { "DWORD PTR" } },    
       { Bytecode::DataTypes::Types::INT_16, { "WORD PTR" } },    
       { Bytecode::DataTypes::Types::INT_8, { "BYTE PTR" } }    
   };  

   constexpr std::array<const char*, 4> IntegerArguments = { "rcx","rdx","r8","r9" };  
   constexpr std::array<const char*, 4> DoubleArguments = { "xmm0l","xmm1l","xmm2l","xmm3l" };  
   constexpr std::array<const char*, 2> MathRegisters = { "rax","rdx" };  

   inline int8_t IntegerArgumentPos = 0;  
   inline int8_t DoubleArgumentPos = 0;  
   inline uint8_t MathRegisterPos = 0; /// eax,ecx  
}  

#endif // !CONVENTIONS_H

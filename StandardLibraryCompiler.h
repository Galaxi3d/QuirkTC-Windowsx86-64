#ifndef STANDARDLIBRARYCOMPILER_H
#define STANDARDLIBRARYCOMPILER_H
#include "QuirkSTD.h"
#include "Assembly.h"
#include <array>
#include <functional>

void Print_Int();
void Print_String();
void Print_Float();
void System();

using CompileFunction = std::function<void()>;

inline void CompileStandardLibraryFunction(size_t FunctionIndex)
{
    static const std::array<CompileFunction, 100> compileFunctions = {
        Print_Int,
		Print_String,
        Print_Float,   
        System,
    };

    if (FunctionIndex < compileFunctions.size())
    {
        compileFunctions[FunctionIndex]();
	}
    else
    {
        std::cerr << "Function index out of range: " << FunctionIndex << std::endl;
    }


}
#endif // STANDARDLIBRARYCOMPILER_H
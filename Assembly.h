#ifndef ASSEMBLY_H
#define ASSEMBLY_H
#include "QuirkSTD.h"
#include <string>
#include <stack>
#include <vector>

namespace Assembly
{
	inline std::unique_ptr<std::string> AssemblySource;
	inline std::unique_ptr<std::string> AssemblyPrologue;	
    inline std::unique_ptr<std::string> AssemblyDataSection;
	inline std::vector<std::string> AdditionalSymbols; /// not regulated well, currently just newline 
	inline std::vector<std::string> IncludedLibraries;
	inline std::vector<std::string> IncludedExternalFunctions;
	inline std::unique_ptr<Bytecode::BytecodePackage> Package;
	inline std::stack<size_t> StackLocations;
	inline size_t ParameterAmount = 0;
	inline bool IsStandardLibrary = false;

	/// <summary>
	/// Returns True When the symbol was added, False when it already exists
	/// </summary>
	/// <param name="String"></param>
	/// <returns></returns>
	inline bool TryAddSymbol(std::string String)
	{
		if (std::find(AdditionalSymbols.begin(), AdditionalSymbols.end(), String) == AdditionalSymbols.end())
		{
			AdditionalSymbols.push_back(String);			

			return true;
		}

		return false;
	}


	/// <summary>
	/// Returns True when the library was added, False when it already exists
	/// </summary>
	/// <param name="Library"></param>
	/// <returns></returns>
	inline bool TryAddLibrary(std::string Library)
	{
		if (std::find(IncludedLibraries.begin(), IncludedLibraries.end(), Library) == IncludedLibraries.end())
		{
			IncludedLibraries.push_back(Library);
			AssemblyPrologue->append("includelib " + Library + "\n");

			return true;
		}

		return false;
	}

	inline bool HasFunctionBeenUsed(std::string FunctionName)
	{
		return std::find(IncludedExternalFunctions.begin(), IncludedExternalFunctions.end(), FunctionName) != IncludedExternalFunctions.end();
	}

	inline void AddExternalFunctionReference(std::string FunctionName) /// ASSUMES THAT THE FUNCTION HAS ALREADY BEEN CHECKED AND DOES NOT EXIST
	{		
		IncludedExternalFunctions.push_back(FunctionName);
		AssemblyPrologue->append("extern " + FunctionName + ": PROC\n");		
	}

	inline void AddToPrologue(std::string Line)
	{
		AssemblyPrologue->append(Line + '\n');
	}

	inline void AddToDataSection(std::string Line)
	{
		AssemblyDataSection->append(Line + '\n');
	}
}
#endif
#include "StandardLibraryCompiler.h"
#include "Conventions.h"

void Print_Int()
{
	if (!Assembly::HasFunctionBeenUsed("print_int"))
	{
		Assembly::TryAddLibrary("Quirk-Standard-Library-Windows.lib");
		Assembly::AddExternalFunctionReference("print_int");

		/*if (Assembly::TryAddSymbol("newline"));
		{
			Assembly::AddToDataSection("newline BYTE 10, 0"); /// represents \n
		}*/
	}

	Assembly::AssemblySource->append("call print_int\n");

	Conventions::IntegerArgumentPos = 0; /// reset the integer argument position
}

void Print_String() /// TOOD: add  ADDLINE here
{
	if (!Assembly::HasFunctionBeenUsed("print_string"))
	{
		Assembly::TryAddLibrary("Quirk-Standard-Library-Windows.lib");
		Assembly::AddExternalFunctionReference("print_string");

		/*if (Assembly::TryAddSymbol("newline"));
		{
			Assembly::AddToDataSection("newline BYTE 10, 0"); /// represents \n
		}*/
	}
	
	Assembly::AssemblySource->append("call print_string\n");				

	Conventions::IntegerArgumentPos = 0; /// reset the integer argument position
}

void Print_Float()
{
}

void System()
{
}

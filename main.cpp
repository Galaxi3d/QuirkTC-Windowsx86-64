//#define _CRTDBG_MAP_ALLOC
#include <stdlib.h>
#include <crtdbg.h>
#include "Extractor.h"
#include "output.h"
#include "AsmBuilder.h"
#include "ToExecutable.h"

/// FIRST INPUT FILE
/// SECOND OUTPUT FILE
/// I

void debugMode()
{
	const char* FilePath = "C:\\Users\\anisr\\source\\repos\\QuirkRepo\\Compiler\\Source.qrkc";
	const char* OutputPath = "C:\\Users\\anisr\\OneDrive\\Desktop\\Output";

	std::unique_ptr<Bytecode::BytecodePackage> Bytecode = ExtractFileContents(FilePath);

	std::string AssemblySource = BuildAssembly(std::move(Bytecode));

	std::cout << AssemblySource << '\n';	

	ToExecutable(AssemblySource, OutputPath);
}

int main(int argc, char* argv[])
{
	//_CrtSetDbgFlag(_CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF);

	//printf("FOR THE LOVE OF ");

	if (argc == 1)
	{
		std::cerr << "No arguments provided.Defaults to DEBUG\n";
		debugMode();
		return 0;
	}
	else if (argc > 3)
	{
		std::cerr << "Too many arguments provided.\n";
		return 1;
	}

	std::string InputFile = argv[1];
	std::string OutputFile;

	std::cout << InputFile << '\n';

	if (argc == 3)
	{
		OutputFile = argv[2];
	}
	else
	{		
		OutputFile = InputFile.substr(0, InputFile.find_last_of('.'));
	}

	std::cout << OutputFile << '\n';

	std::unique_ptr<Bytecode::BytecodePackage> Bytecode = ExtractFileContents(InputFile.c_str());

	std::string AssemblySource = BuildAssembly(std::move(Bytecode));

	ToExecutable(AssemblySource, OutputFile.c_str());
	return 0;


	//ToExecutable(AssemblySource, OutputPath);
}


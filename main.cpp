#include "Extractor.h"
#include "output.h"
#include "AsmBuilder.h"
#include "ToExecutable.h"

void main()
{
	const char* FilePath = "C:\\Users\\anisr\\source\\repos\\QuirkRepo\\Compiler\\Output.qrk";
	const char* OutputPath = "C:\\Users\\anisr\\OneDrive\\Desktop\\Output";

	Bytecode::BytecodePackage* Bytecode = ExtractFileContents(FilePath);

	//Out(Bytecode);
	
	std::string AssemblySource = BuildAssembly(Bytecode);

	//Out(Bytecode);

	ToExecutable(AssemblySource, OutputPath);
}
#include "Extractor.h"
#include "output.h"
#include "AsmBuilder.h"




void main()
{
	const char* FilePath = "C:\\Users\\anisr\\source\\repos\\QuirkRepo\\Compiler\\Output.qrk";

	Bytecode::BytecodePackage* Bytecode = ExtractFileContents(FilePath);

	Out(Bytecode);
	
	BuildAssembly(Bytecode);

	//Out(Bytecode);
}
#ifndef EXTRACTOR_H
#define EXTRACTOR_H
#include "Bytecode.h"
#include <stdlib.h>
#include <stdio.h>

Bytecode::BytecodePackage* ExtractFileContents(const char* FilePath);

Bytecode::DataTypes::DataType* ReadGenericType(FILE* fPtr);
#endif // !EXTRACTOR_H





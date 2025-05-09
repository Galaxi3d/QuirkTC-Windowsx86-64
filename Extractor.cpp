#include "Extractor.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <cstdint>

#define READ_LITTLE_ENDIAN(value,file)\
 do {                                \
        int b0 = fgetc(file);           \
        int b1 = fgetc(file);           \
        int b2 = fgetc(file);           \
        int b3 = fgetc(file);           \
        if (b0 == EOF || b1 == EOF || b2 == EOF || b3 == EOF) { \
            /* Handle error (e.g., return or break) */ \
            fprintf(stderr, "Error: Unexpected EOF while reading.\n"); \
            exit(1);                    \
        }                               \
        value = (uint32_t)b0 |          \
                ((uint32_t)b1 << 8) |   \
                ((uint32_t)b2 << 16) |  \
                ((uint32_t)b3 << 24);   \
    } while (0)

std::unique_ptr<Bytecode::BytecodePackage> ExtractFileContents(const char* FilePath)  
{  
	FILE* file = fopen(FilePath, "rb");  

	if (!file)  
	{  
		printf("Cannot Open File %s \n", FilePath);  
		exit(1);  
	}  

	char header[4] = { 0 };  
	fread(header, 1, 3, file);  
	if (strcmp(header, "QRK") != 0)  
	{  
		fclose(file);  
		printf("Incompatible file");  
		exit(1);  
	}  

	uint32_t instructionsSize = 0;  
	uint32_t literalSize = 0;  

	READ_LITTLE_ENDIAN(instructionsSize, file);  
	READ_LITTLE_ENDIAN(literalSize, file);  

	printf("Instructions Size: %i \n", instructionsSize);  

	auto Package = std::make_unique<Bytecode::BytecodePackage>();  

	if (!Package)  
	{  
		fclose(file);  
		printf("bad malloc");  
		exit(1);  
	}  

	Package->InstructionsSize = instructionsSize;  
	Package->LiteralSize = literalSize;  

	//Package->Instructions = nullptr;  
	//Package->DataSize = nullptr;  

	if (instructionsSize > 0)  
	{  
		Package->Instructions = new Bytecode::Bytecode[instructionsSize];  
		Package->DataSize = new Bytecode::DataTypes::Types[instructionsSize];  
		if (Package->Instructions && Package->DataSize)  
		{  
			fread(Package->Instructions, 1, instructionsSize, file);  
			fread(Package->DataSize, 1, instructionsSize, file);  
		}  
		else  
		{  
			perror("bad alloc");  
			exit(1);  
		}  
	}  

	Package->Indices = nullptr;  

	if (instructionsSize > 0)  
	{  
		Package->Indices = new uint32_t[instructionsSize];  
		if (Package->Indices) {  
			for (uint32_t i = 0; i < instructionsSize; i++) {  
				uint32_t value = 0;  
				READ_LITTLE_ENDIAN(value, file);  
				Package->Indices[i] = value;  
			}  
		}  
	}  

	Package->LiteralType = nullptr;  

	if (literalSize > 0)  
	{  
		Package->LiteralType = new Bytecode::DataTypes::DataType[literalSize];  
		Package->Literals = new void*[literalSize];  

		if (Package->LiteralType) {  
			for (uint32_t i = 0; i < literalSize; i++) {  

				uint8_t type;  
				READ_LITTLE_ENDIAN(type, file);  
				Package->LiteralType[i].Type = static_cast<Bytecode::DataTypes::Types>(type);  

				if (Bytecode::DataTypes::IsGeneric(Package->LiteralType[i].Type))  
				{  
					Package->LiteralType[i].GenericType = ReadGenericType(file);  
				}  
				else  
				{  
					Package->LiteralType[i].GenericType = nullptr;  
				}  

				switch (Package->LiteralType[i].Type)  
				{  
				case Bytecode::DataTypes::Types::INT_32:  
				{  
					int32_t value;  
					fread(&value, sizeof(int32_t), 1, file);  
					Package->Literals[i] = new int(value);  
					break;  
				}  
				case Bytecode::DataTypes::Types::STRING:  
				{  
					uint32_t dataSize;  
					READ_LITTLE_ENDIAN(dataSize, file);  
					if (dataSize > 0)  
					{  
						char* value = new char[dataSize];  
						fread(value, sizeof(char), dataSize, file);  
						Package->Literals[i] = value;  
					}  
					else  
					{  
						exit(82);  
					}  
					break;  
				}  
				case Bytecode::DataTypes::Types::BOOL:  
				{  
					bool value;  
					fread(&value, sizeof(bool), 1, file);  
					Package->Literals[i] = new bool(value);  
					break;  
				}  
				default:  
					perror("Unidentified Data Type Detected");  
					exit(99);  
					break;  
				}  
			}  
		}  
	}  

	fclose(file);  
	return Package;  
}

Bytecode::DataTypes::DataType* ReadGenericType(FILE* fPtr) /// UNTESTED
{
	Bytecode::DataTypes::DataType* dataType = new Bytecode::DataTypes::DataType();
	uint8_t genericType;
	READ_LITTLE_ENDIAN(genericType, fPtr);			
	if (genericType == GENERIC_TYPE_ENUMARTION_SENTINEL)
	{
		dataType->GenericType = nullptr;
		return dataType;
	}

	dataType->Type = static_cast<Bytecode::DataTypes::Types>(genericType);

	dataType->GenericType = ReadGenericType(fPtr);

	return dataType;
}

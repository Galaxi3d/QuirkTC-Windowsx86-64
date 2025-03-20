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

Bytecode::BytecodePackage* ExtractFileContents(const char* FilePath)
{
	FILE* file = fopen(FilePath, "rb");

	if (!file)
	{
		printf("error");
		exit(1);
	}

	char header[4] = { 0 };
	fread(header, 1, 3, file);
	if (strcmp(header, "BCP") != 0)
	{
		fclose(file);
		printf("unknown");
		exit(1);
	}


	uint32_t instructionsSize = 0;
	uint32_t literalSize = 0;

	READ_LITTLE_ENDIAN(instructionsSize, file);
	READ_LITTLE_ENDIAN(literalSize, file);

	Bytecode::BytecodePackage* Package = (Bytecode::BytecodePackage*)malloc(sizeof(Bytecode::BytecodePackage));	

	if (!Package)
	{
		fclose(file);
		printf("bad malloc");
		exit(1);
	}

	Package->InstructionsSize = instructionsSize;
	Package->LiteralSize = literalSize;

	Package->Instructions = NULL;

	if (instructionsSize > 0)
	{
		Package->Instructions = (Bytecode::Bytecode*)malloc(instructionsSize * sizeof(uint8_t));		
		if (Package->Instructions)
		{
			fread(Package->Instructions, 1, instructionsSize, file);			
		}
		else
		{
			perror("bad alloc");
			exit(1);
		}
	}

	Package->Indices = NULL;

	if (instructionsSize > 0)
	{
		Package->Indices = (uint32_t*)malloc(instructionsSize * sizeof(uint32_t));
		if (Package->Indices) {
			for (uint32_t i = 0; i < instructionsSize; i++) {
				uint32_t value = 0;				
				READ_LITTLE_ENDIAN(value, file);
				Package->Indices[i] = value;
			}
		}
	}

	Package->LiteralType = NULL;

	if (literalSize > 0)
	{
		Package->LiteralType = (Bytecode::DataTypes::DataType*)malloc(literalSize * sizeof(Bytecode::DataTypes::DataType));
		Package->Literals = (void**)malloc(literalSize * sizeof(void*));

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
				case Bytecode::DataTypes::Types::INT:
				{
					int32_t value;
					fread(&value, sizeof(int32_t), 1, file);
					int* intPtr = new int(value);            
					Package->Literals[i] = intPtr;  

					//printf("int value is %i \n", value);

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
					bool* boolPtr = new bool(value);
					Package->Literals[i] = boolPtr;
					break;
				}
				default:
					perror("Unidentified Data Type Detected");
					exit(99);
					break;
				}

				/*Package->Literals[i] = malloc(dataSize);

				fread(Package->Literals[i], 1, dataSize, file);				*/
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

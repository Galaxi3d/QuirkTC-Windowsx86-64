#include <cstdint>
#ifndef DATATYPES_H 
#define DATATYPES_H
#define GENERIC_TYPE_ENUMARTION_SENTINEL 0xFF


namespace Bytecode
{
	namespace DataTypes
	{
		enum class Types : int8_t
		{
			INT,
			FLOAT,
			STRING,
			CHAR,
			VOID,
			BOOL,
			LIST,
			STACK,
			DEFAULT,
		};


		struct DataType
		{
			/// TODO: create constructor for Type
			Types Type;
			struct DataType* GenericType = nullptr;
		};

		inline bool IsGeneric(Types type)
		{
			return (type == Types::STACK || type == Types::LIST);
		}

	}
}

#endif
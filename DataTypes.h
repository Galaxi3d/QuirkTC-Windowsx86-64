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
			BOOL,
			UINT_64,
			UINT_32,
			UINT_16,
			UINT_8,
			INT_64,
			INT_32,
			INT_16,
			INT_8,
			FLOAT_128,
			FLOAT_64,
			FLOAT_32,
			FLOAT_16,
			STRING,
			LIST,
			VOID,
			DEFAULT,
		};


		struct DataType
		{

			/// TODO: create constructor for Type
			Types Type;
			DataType* GenericType = nullptr;
			DataType() {}
			DataType(Types t)
			{
				this->Type = t;
			}
		};

		inline bool IsGeneric(Types type)
		{
			return (/*type == Types::STACK ||*/ type == Types::LIST);
		}

	}
}

#endif
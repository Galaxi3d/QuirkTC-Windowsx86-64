#ifndef QUIRKSTD_H
#define QUIRKSTD_H
#include "Bytecode.h"
#include <array>
#include <string_view>
#include <variant>
#include <iostream>

namespace QuirkSTD
{
	struct Parameter
	{
		Bytecode::DataTypes::Types type;
		std::string_view name;
	};

	template<size_t N>
	struct FunctionInfo
	{
		std::string_view name;
		Bytecode::DataTypes::Types returnType;
		std::array<Parameter, N> parameters;

		constexpr FunctionInfo(std::string_view name, Bytecode::DataTypes::Types returnType, std::array<Parameter, N> parameters) :
			name(name), returnType(returnType), parameters(parameters) {
		}
	};


	struct Overload
	{

	};

	using FunctionVariant = std::variant<
		FunctionInfo<0>,
		FunctionInfo<1>,
		FunctionInfo<2>,
		FunctionInfo<3>>;


	template<size_t N>
	struct GenericFunction
	{
		std::string_view name;
		std::array<const FunctionVariant*, N> overloads;

		constexpr GenericFunction(std::string_view name, std::array<const FunctionVariant*, N> overloads) :
			name(name), overloads(overloads) {
		}
	};

	using GenericVariants = std::variant<
		GenericFunction<0>,
		GenericFunction<1>,
		GenericFunction<2>,
		GenericFunction<3>>;


	constexpr std::array<FunctionVariant, 7> Functions =
	{
		FunctionInfo<1>{"print_int", Bytecode::DataTypes::Types::VOID, { {Bytecode::DataTypes::Types::INT_32, "value"} }},
		FunctionInfo<1>{"print_string", Bytecode::DataTypes::Types::VOID, { {Bytecode::DataTypes::Types::STRING, "value"} }},
		FunctionInfo<1>{"print_float", Bytecode::DataTypes::Types::VOID, { {Bytecode::DataTypes::Types::FLOAT_32, "value"} }},
		FunctionInfo<1>{"sin", Bytecode::DataTypes::Types::FLOAT_32, { {Bytecode::DataTypes::Types::FLOAT_32, "value"} }},
		FunctionInfo<1>{"cos", Bytecode::DataTypes::Types::FLOAT_32, { {Bytecode::DataTypes::Types::FLOAT_32, "value"} }},
		FunctionInfo<1>{"tan", Bytecode::DataTypes::Types::FLOAT_32, { {Bytecode::DataTypes::Types::FLOAT_32, "value"} }},
		FunctionInfo<1>{"system", Bytecode::DataTypes::Types::VOID, { {Bytecode::DataTypes::Types::STRING, "command"} }},
	};

	constexpr inline size_t GetFunctionIndex(std::string_view function)
	{
		for (size_t i = 0; i < Functions.size(); i++)
		{
			if (std::visit([&](auto&& func) { return func.name == function; }, Functions[i]))
				return i;
		}

		return std::string_view::npos;
	}

	constexpr std::array<GenericVariants, 1> GenericFunctions =
	{
		GenericFunction<1>{"print", {&Functions[GetFunctionIndex("print_int")]}},
	};

	constexpr inline size_t GetGenericFunctionIndex(std::string_view function)
	{
		for (size_t i = 0; i < GenericFunctions.size(); i++)
		{
			if (std::visit([&](auto&& func) { return func.name == function; }, GenericFunctions[i]))
				return i;
		}
		return std::string_view::npos;
	}

	constexpr Bytecode::DataTypes::DataType* GetFunctionReturnType(size_t index)
	{
		if (index != std::string_view::npos)
		{
			return new Bytecode::DataTypes::DataType(std::visit([&](auto&& func) { return func.returnType; }, Functions[index]));
		}
		return nullptr;
	}
}

#endif // !QUIRKSTD_H

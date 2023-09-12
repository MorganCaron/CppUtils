#pragma once

#include <CppUtils.hpp>

namespace CppUtils::UnitTests::Type::Concept
{
	using namespace CppUtils::Type::Concept;
	
	static_assert(!Printable<void>);
	static_assert(Printable<bool>);
	static_assert(Printable<char>);
	static_assert(Printable<int>);
	static_assert(Printable<double>);
	static_assert(Printable<std::string>);
	static_assert(Printable<std::string_view>);

	static_assert(!Present<int>);
	static_assert(!Present<int, bool, char, double>);
	static_assert(Present<int, bool, char, int, double>);
}

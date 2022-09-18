#pragma once

#include <string_view>

#include <CppUtils/Type/Concept.hpp>

namespace CppUtils::String::Concept
{
	template<class T>
	concept Char = Type::Concept::Present<T, char, signed char, unsigned char, wchar_t, char8_t, char16_t, char32_t>;

	template<class T>
	concept String = Type::Concept::Specializes<T, std::basic_string>;

	template<class T>
	concept StringView = Type::Concept::Specializes<T, std::basic_string_view>;

	template<class T>
	concept Text = String<T> || StringView<T>;
}

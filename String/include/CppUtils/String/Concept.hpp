#pragma once

#include <string_view>

#include <CppUtils/Type/Concept.hpp>

namespace CppUtils::String::Concept
{
	template<class T>
	concept Char = std::char_traits<T>::value;

	template<class T>
	concept String = Type::Concept::Specializes<T, std::basic_string>;

	template<class T>
	concept StringView = Type::Concept::Specializes<T, std::basic_string_view>;

	template<class T>
	concept Text = String<T> || StringView<T>;
}

#pragma once

#include <variant>
#include <iostream>

#include <CppUtils/Type/Concepts.hpp>

template<typename Type, typename... Types> requires CppUtils::Type::Concept::isPrintable<Type> && (CppUtils::Type::Concept::isPrintable<Types> && ...)
inline std::ostream& operator<<(std::ostream& os, const std::variant<Type, Types...>& variant)
{
	return std::visit([&os](auto&& value) -> std::ostream& {
		return os << value;
	}, variant);
}

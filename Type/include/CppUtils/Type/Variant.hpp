#pragma once

#include <variant>
#include <iostream>

#include <CppUtils/Type/Concept.hpp>

namespace CppUtils::Type::Variant::Operators
{
	template<Concept::Printable... Types>
	requires (sizeof...(Types) > 0)
	auto operator<<(std::ostream& os, const std::variant<Types...>& variant) -> std::ostream&
	{
		return std::visit([&os](auto&& value) -> std::ostream& {
			return os << value;
		}, variant);
	}

	template<class LhsType, class... RhsTypes>
	auto operator==(const LhsType& lhs, const std::variant<RhsTypes...>& rhs) -> bool
	{
		if constexpr (!Concept::Present<LhsType, RhsTypes...>)
			return false;
		else if (!std::holds_alternative<LhsType>(rhs))
			return false;
		else
			return lhs == std::get<LhsType>(rhs);
	}

	template<class... LhsTypes, class... RhsTypes>
	requires (!Concept::Present<LhsTypes, RhsTypes...> || ...)
	auto operator==(const std::variant<LhsTypes...>& lhs, const std::variant<RhsTypes...>& rhs) -> bool
	{
		return std::visit([&rhs](auto&& lhsValue) -> bool {
			if constexpr (!Concept::Present<decltype(lhsValue), RhsTypes...>)
				return lhsValue == rhs;
			else
				return false;
		}, lhs);
	}
}

using namespace CppUtils::Type::Variant::Operators;

#pragma once

#include <variant>
#include <iostream>

#include <CppUtils/Type/Concepts.hpp>

namespace CppUtils::Type::Variant::Operators
{
	template<typename Type, typename... Types> requires Concept::isPrintable<Type> && (Concept::isPrintable<Types> && ...)
	inline std::ostream& operator<<(std::ostream& os, const std::variant<Type, Types...>& variant)
	{
		return std::visit([&os](auto&& value) -> std::ostream& {
			return os << value;
		}, variant);
	}

	template<typename LhsType, typename... RhsTypes>
	inline bool operator==(const LhsType& lhs, const std::variant<RhsTypes...>& rhs)
	{
		if constexpr(!Concept::isPresent<LhsType, RhsTypes...>)
			return false;
		else if (!std::holds_alternative<LhsType>(rhs))
			return false;
		else
			return lhs == std::get<LhsType>(rhs);
	}

	template<typename... LhsTypes, typename... RhsTypes> requires (!Concept::isPresent<LhsTypes, RhsTypes...> || ...)
	inline bool operator==(const std::variant<LhsTypes...>& lhs, const std::variant<RhsTypes...>& rhs)
	{
		return std::visit([&rhs](auto&& lhsValue) -> bool {
			if constexpr(!Concept::isPresent<decltype(lhsValue), RhsTypes...>)
				return lhsValue == rhs;
			else
				return false;
		}, lhs);
	}

	template<typename... LhsTypes, typename... RhsTypes> requires (!Concept::isPresent<LhsTypes, RhsTypes...> || ...)
	inline bool operator!=(const std::variant<LhsTypes...>& lhs, const std::variant<RhsTypes...>& rhs)
	{
		return !(lhs == rhs);
	}
}

using namespace CppUtils::Type::Variant::Operators;

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

template<typename LhsType, typename... RhsTypes>
inline bool operator==(const LhsType& lhs, const std::variant<RhsTypes...>& rhs)
{
	if constexpr(!CppUtils::Type::Concept::isPresent<LhsType, RhsTypes...>)
		return false;
	else if (!std::holds_alternative<LhsType>(rhs))
		return false;
	else
		return lhs == std::get<LhsType>(rhs);
}

template<typename... LhsTypes, typename RhsType>
inline bool operator==(const std::variant<LhsTypes...>& lhs, const RhsType& rhs)
{
	return rhs == lhs;
}

template<typename... LhsTypes, typename... RhsTypes>
inline bool operator==(const std::variant<LhsTypes...>& lhs, const std::variant<RhsTypes...>& rhs)
{
	return std::visit([&rhs](auto&& lhsValue) -> bool {
		if constexpr(!CppUtils::Type::Concept::isPresent<decltype(lhsValue), RhsTypes...>)
			return lhsValue == rhs;
		else
			return false;
	}, lhs);
}

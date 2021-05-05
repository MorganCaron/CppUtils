#pragma once

#include <CppUtils/Type/Traits.hpp>
#include <CppUtils/String/String.hpp>
#include <CppUtils/Language/Parser/Context.hpp>

namespace CppUtils::Language::Parser::Modifier
{
	template<typename... Types>
	requires Type::Traits::isPresent<std::string, Types...> || Type::Traits::isPresent<Type::Token, Types...>
	inline bool stringToTokenModifier(Context<Types...>& context, bool saveTypename)
	{
		auto& [cursor, parentNode] = context;
		if (parentNode.get().childs.empty())
			return false;
		auto& lastChild = parentNode.get().childs.back();
		if (!std::holds_alternative<std::string>(lastChild.value))
			return false;
		auto& string = std::get<std::string>(lastChild.value);
		auto token = Type::Token{string};
		if (saveTypename)
			token.saveTypename();
		lastChild.value = std::move(token);
		return true;
	}

	template<typename... Types>
	requires Type::Traits::isPresent<std::string, Types...>
	inline bool trimModifier(Context<Types...>& context)
	{
		auto& [cursor, parentNode] = context;
		if (parentNode.get().childs.empty())
			return false;
		auto& lastChild = parentNode.get().childs.back();
		
		if (!std::holds_alternative<std::string>(lastChild.value))
			return false;
		auto& string = std::get<std::string>(lastChild.value);
		string = String::trimString(string);
		return true;
	}
}

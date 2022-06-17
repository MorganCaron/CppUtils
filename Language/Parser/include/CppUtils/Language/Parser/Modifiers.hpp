#pragma once

#include <CppUtils/Type/Concept.hpp>
#include <CppUtils/String/String.hpp>
#include <CppUtils/Language/Parser/Context.hpp>

namespace CppUtils::Language::Parser::Modifier
{
	template<class... Types>
	requires Type::Concept::Present<std::string, Types...> || Type::Concept::Present<Type::Token, Types...>
	bool stringToTokenModifier(Context<Types...>& context, bool saveTypename)
	{
		auto& [cursor, parentNode, firstChildPosition, parsingErrors] = context;
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

	template<class... Types>
	requires Type::Concept::Present<std::string, Types...>
	bool trimModifier(Context<Types...>& context)
	{
		auto& [cursor, parentNode, firstChildPosition, parsingErrors] = context;
		if (parentNode.get().childs.empty())
			return false;
		auto& lastChild = parentNode.get().childs.back();
		if (!std::holds_alternative<std::string>(lastChild.value))
			return false;
		auto& string = std::get<std::string>(lastChild.value);
		string = String::trimString(string);
		return true;
	}

	template<class... Types>
	requires Type::Concept::Present<std::string, Types...>
	bool insertEmptyString(Context<Types...>& context)
	{
		auto& [cursor, parentNode, firstChildPosition, parsingErrors] = context;
		using namespace std::literals;
		parentNode.get().childs.emplace_back(Parser::ASTNode<Types...>{""s});
		return true;
	}

	template<class... Types>
	requires Type::Concept::Present<std::string, Types...>
	bool insertString(Context<Types...>& context, std::string_view string)
	{
		auto& [cursor, parentNode, firstChildPosition, parsingErrors] = context;
		if (!parentNode.get().childs.empty())
			if (auto& lastValue = parentNode.get().childs.back().value; std::holds_alternative<std::string>(lastValue)) {
				std::get<std::string>(lastValue) += ' ';
				return true;
			}
		parentNode.get().childs.emplace_back(Parser::ASTNode<Types...>{std::string{string}});
		return true;
	}
}

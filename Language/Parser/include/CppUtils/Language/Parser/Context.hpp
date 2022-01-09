#pragma once

#include <stack>
#include <functional>

#include <CppUtils/Type/Typed.hpp>
#include <CppUtils/Graph/VariantTreeNode.hpp>
#include <CppUtils/Language/Parser/Cursor.hpp>

namespace CppUtils::Language::Parser
{
	template<typename... Types>
	using ASTNode = Graph::VariantTreeNode<Types...>;

	using ILexeme = Type::ITyped;

	struct ParsingError final
	{
		std::size_t cursorPosition;
		Parser::ILexeme* lexeme;
	};

	template<typename... Types>
	struct Context final
	{
		Parser::Cursor<std::string> cursor;
		std::reference_wrapper<ASTNode<Types...>> parentNode;
		std::size_t firstChildPosition;
		std::vector<ParsingError> parsingErrors = {};
	};

	template<typename... Types>
	using ParsingFunction = std::function<bool (Context<Types...>&)>;
}

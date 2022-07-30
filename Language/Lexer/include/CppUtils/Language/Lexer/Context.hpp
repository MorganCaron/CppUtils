#pragma once

#include <CppUtils/Language/Parser/Context.hpp>

namespace CppUtils::Language::Lexer
{
	template<class Element>
	struct Context final
	{
		Parser::Context<Element> parserContext;
		std::reference_wrapper<const Parser::AstNode> lexeme;
		std::reference_wrapper<const Parser::Ast> grammar;
	};
}

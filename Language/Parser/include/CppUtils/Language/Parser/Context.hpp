#pragma once

#include <string>
#include <utility>

#include <CppUtils/Language/Parser/Cursor.hpp>

namespace CppUtils::Language::Parser
{
	template<class Element>
	struct Context
	{
		Cursor<Element> cursor;
		std::reference_wrapper<AstNode> parentNode;
		ReadableAstNode ast;
	};
}

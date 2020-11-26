#include <CppUtils/Language/Lexer/Lexeme.hpp>

namespace CppUtils::Language::Lexer::Lexeme
{
	Contingence& Contingence::operator||(const Definition& rhs)
	{
		tokens.emplace_back(rhs.token);
		return *this;
	}
}

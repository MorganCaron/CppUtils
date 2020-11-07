#include <Language/Lexeme.hpp>

namespace CppUtils::Language::Lexeme
{
	Contingence& Contingence::operator||(const Definition& rhs)
	{
		tokens.emplace_back(rhs.token);
		return *this;
	}
}

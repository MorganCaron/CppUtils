#include <CppUtils/Language/Parser/Expression.hpp>

namespace CppUtils::Language::Parser
{
	Contingence& Contingence::operator||(const Expression& rhs)
	{
		tokens.emplace_back(rhs.token);
		return *this;
	}
}

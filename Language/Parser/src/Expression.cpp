#include <CppUtils/Language/Parser/Expression.hpp>

namespace CppUtils::Language::Parser
{
	Alternative& Alternative::operator||(const Expression& rhs)
	{
		tokens.emplace_back(rhs.token);
		return *this;
	}
}

#include <CppUtils/Terminal/TextModifier.hpp>

namespace CppUtils::Terminal
{
#if defined(OS_WINDOWS)
	WORD TextModifier::m_attributes{};
#endif
}

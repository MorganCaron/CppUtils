#include <CppUtils/Switch/Switch.hpp>

namespace CppUtils
{
	std::unordered_map<Switch::Id, bool> Switch::m_idStates{};
	std::unordered_map<Switch::Tag, std::vector<Switch::Id>, Switch::Tag::hash_fn> Switch::m_tags{};
}

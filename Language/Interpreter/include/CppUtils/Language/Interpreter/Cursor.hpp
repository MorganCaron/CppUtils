#pragma once

#include <vector>

namespace CppUtils::Language::Interpreter
{
	template<typename Element>
	struct Cursor final
	{
		explicit Cursor(const std::vector<Element>& c_elements, const std::size_t c_pos = 0):
			elements{c_elements},
			pos{c_pos}
		{}

		[[nodiscard]] inline const Element& getElement() const
		{
			return elements.at(pos);
		}

		inline void exit() noexcept
		{
			pos = elements.size();
		}

		const std::vector<Element>& elements;
		std::size_t pos;
	};
}

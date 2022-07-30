#pragma once

#include <CppUtils/Container/Sequence.hpp>

namespace CppUtils::Log
{
	class TreeNodeLogger final
	{
	public:
		[[nodiscard]] static constexpr char getDifferenceChar(Container::Sequence::Difference difference) noexcept
		{
			switch (difference)
			{
				case Container::Sequence::Difference::Added:
					return '+';
				case Container::Sequence::Difference::Removed:
					return '-';
				case Container::Sequence::Difference::Edited:
					return '*';
				default:
					return ' ';
			}
		}

		[[nodiscard]] static constexpr Terminal::TextColor::TextColorEnum getDifferenceColor(Container::Sequence::Difference difference) noexcept
		{
			switch (difference)
			{
				case Container::Sequence::Difference::Added:
					return Terminal::TextColor::TextColorEnum::Green;
				case Container::Sequence::Difference::Removed:
					return Terminal::TextColor::TextColorEnum::Red;
				case Container::Sequence::Difference::Edited:
					return Terminal::TextColor::TextColorEnum::Cyan;
				default:
					return Terminal::TextColor::TextColorEnum::Default;
			}
		}
	};
}

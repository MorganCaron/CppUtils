#pragma once

#include <array>
#include <ranges>

namespace CppUtils::Language
{
	[[nodiscard]] constexpr auto multiTapeTuringMachine(std::ranges::range auto& instructions, std::ranges::range auto&... ioRanges) -> bool
	{
		using Value = std::decay_t<decltype(instructions[0])>;
		auto ranges = std::array{std::ref(instructions), std::ref(ioRanges)...};
		auto iterators = std::array{std::begin(instructions), std::begin(ioRanges)...};
		auto i = 0, buffer = 0;
		const auto getRange = [&ranges, &i]() -> auto& { return ranges.at(static_cast<std::size_t>(i)).get(); };
		const auto getIterator = [&iterators, &i]() -> auto& { return iterators.at(static_cast<std::size_t>(i)); };
		for (auto& instructionsIterator = iterators[0]; instructionsIterator != std::cend(instructions); ++instructionsIterator)
		{
			switch (*instructionsIterator)
			{
				case '_': buffer = 0; break; // Reset
				case '<': i = buffer; break; // Move left
				case '>': buffer = i; break; // Move right
				case '!': if (i) instructionsIterator += buffer; break; // Conditional Jump
				case '?': if (i <= 0) instructionsIterator += buffer; break; // Conditional Jump with threshold
				case 'A': getRange().push_back(static_cast<Value>(buffer)); break; // Add
				case 'B': getIterator() = std::begin(getRange()) + buffer; break; // Begin
				case 'C': getRange().clear(); break; // Clear
				case 'D': buffer -= i; break; // Decrement
				case 'E': getIterator() = std::end(getRange()) - 1 - buffer; break; // End
				case 'I': buffer += i; break; // Increment
				case 'J': getIterator() += buffer; break; // Jump
				case 'L': buffer += static_cast<int>(std::size(getRange())); break; // Length
				case 'N': buffer = !buffer; break; // Not
				case 'P': buffer += static_cast<int>(getIterator() - std::cbegin(getRange())); break; // Position
				case 'R': buffer += *getIterator(); break; // Read
				case 'S': std::swap(i, buffer); break; // Swap
				case 'U': getRange().pop_back(); break; // Undo
				case 'W': *getIterator() = static_cast<Value>(buffer); break; // Write
				case 'X': return false; // Exit
				default: // Numbers
					if (*instructionsIterator >= '0' && *instructionsIterator <= '9')
						buffer = buffer * 10 + *instructionsIterator - '0';
			}
		}
		return true;
	}
}

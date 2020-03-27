#pragma once

#include <ostream>
#include <sstream>
#include <iterator>
#include <string_view>

using namespace std::literals;

namespace CppUtils::Terminal::TextModifier
{
	[[maybe_unused]] static constexpr const auto Reset = "\x1B[0m"sv;
	[[maybe_unused]] static constexpr const auto EraseCurrentLine = "\x1B[2K0E"sv;

	namespace TextStyle
	{
		[[maybe_unused]] static constexpr const auto Bolder = "\x1B[1m"sv;
		[[maybe_unused]] static constexpr const auto Lighter = "\x1B[2m"sv;
		[[maybe_unused]] static constexpr const auto Italic = "\x1B[3m"sv;
		[[maybe_unused]] static constexpr const auto Underline = "\x1B[4m"sv;
	}
	
	namespace TextColor
	{
		[[maybe_unused]] static constexpr const auto Black = "\x1B[30m"sv;
		[[maybe_unused]] static constexpr const auto Red = "\x1B[31m"sv;
		[[maybe_unused]] static constexpr const auto Green = "\x1B[32m"sv;
		[[maybe_unused]] static constexpr const auto Yellow = "\x1B[33m"sv;
		[[maybe_unused]] static constexpr const auto Blue = "\x1B[34m"sv;
		[[maybe_unused]] static constexpr const auto Magenta = "\x1B[35m"sv;
		[[maybe_unused]] static constexpr const auto Cyan = "\x1B[36m"sv;
		[[maybe_unused]] static constexpr const auto White = "\x1B[37m"sv;
		[[maybe_unused]] static constexpr const auto Default = "\x1B[39m"sv;
	}

	namespace BackgroundColor
	{
		[[maybe_unused]] static constexpr const auto Black = "\x1B[40m"sv;
		[[maybe_unused]] static constexpr const auto Red = "\x1B[41m"sv;
		[[maybe_unused]] static constexpr const auto Green = "\x1B[42m"sv;
		[[maybe_unused]] static constexpr const auto Yellow = "\x1B[43m"sv;
		[[maybe_unused]] static constexpr const auto Blue = "\x1B[44m"sv;
		[[maybe_unused]] static constexpr const auto Magenta = "\x1B[45m"sv;
		[[maybe_unused]] static constexpr const auto Cyan = "\x1B[46m"sv;
		[[maybe_unused]] static constexpr const auto White = "\x1B[47m"sv;
		[[maybe_unused]] static constexpr const auto Default = "\x1B[49m"sv;
	}
}

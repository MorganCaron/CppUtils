#pragma once

#include <iostream>
#include <string_view>

#include <Terminal/Terminal.hpp>
#include <Terminal/TextModifier/TextStyle.hpp>
#include <Terminal/TextModifier/TextColor.hpp>
#include <Terminal/TextModifier/BackgroundColor.hpp>

using namespace std::literals;

namespace CppUtils::Terminal::TextModifier
{
#if defined(OS_LINUX) || defined(OS_MACOS)
	namespace ANSIEscapeCode
	{
		[[maybe_unused]] static constexpr const auto Reset = "\x1B[0m"sv;
		[[maybe_unused]] static constexpr const auto EraseCurrentLine = "\x1B[2K0E"sv;
	}
#endif

	void reset(std::ostream& stream)
	{
#if defined(OS_WINDOWS)
		SetConsoleTextAttribute(getTerminalHandle(stream), 0);
#elif defined(OS_LINUX) || defined(OS_MACOS)
		stream << ANSIEscapeCode::Reset;
#endif
	}

	void eraseCurrentLine([[maybe_unused]]std::ostream& stream)
	{
#if defined(OS_LINUX) || defined(OS_MACOS)
		stream << ANSIEscapeCode::EraseCurrentLine;
#endif
	}

	void colorize(
		std::ostream& stream,
		TextColor::TextColorEnum textColor = TextColor::TextColorEnum::Default,
		BackgroundColor::BackgroundColorEnum backgroundColor = BackgroundColor::BackgroundColorEnum::Default)
	{
		const auto text = TextColor::getTextColorCode(textColor);
		const auto background = BackgroundColor::getBackgroundColorCode(backgroundColor);
#if defined(OS_WINDOWS)
		SetConsoleTextAttribute(getTerminalHandle(stream), (background << 4) + text);
#elif defined(OS_LINUX) || defined(OS_MACOS)
		stream << text << background;
#endif
	}

	void applyStyle(
		[[maybe_unused]] std::ostream& stream,
		[[maybe_unused]] TextStyle::TextStyleEnum textStyle)
	{
#if defined(OS_LINUX) || defined(OS_MACOS)
		const auto style = TextStyle::getTextStyleCode(textStyle);
		stream << style;
#endif
	}

}

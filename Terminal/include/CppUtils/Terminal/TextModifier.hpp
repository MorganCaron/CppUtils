#pragma once

#include <iostream>
#include <string_view>

#include <CppUtils/External/DllExport.hpp>
#include <CppUtils/Platform/OS.hpp>
#include <CppUtils/Terminal/Terminal.hpp>
#include <CppUtils/Terminal/TextStyle.hpp>
#include <CppUtils/Terminal/TextColor.hpp>
#include <CppUtils/Terminal/BackgroundColor.hpp>

namespace CppUtils::Terminal
{
	using namespace std::literals;
	
#if defined(OS_LINUX) || defined(OS_MACOS)
	namespace ANSIEscapeCode
	{
		[[maybe_unused]] static constexpr auto Reset = "\x1B[0m"sv;
		[[maybe_unused]] static constexpr auto EraseCurrentLine = "\x1B[2K0E"sv;
	}
#endif

	class DLL_PUBLIC TextModifier final
	{
	public:
#if defined(OS_WINDOWS)
		[[nodiscard]] static auto getTextColor(std::ostream& stream) -> WORD
		{
			CONSOLE_SCREEN_BUFFER_INFO info;
			if (!GetConsoleScreenBufferInfo(getTerminalHandle(stream), &info))
				return 0;
			return info.wAttributes;
		}
#endif

		static auto colorize(
			std::ostream& stream,
			TextColor::TextColorEnum textColor = TextColor::TextColorEnum::Default,
			BackgroundColor::BackgroundColorEnum backgroundColor = BackgroundColor::BackgroundColorEnum::Default) -> void
		{
			[[maybe_unused]] const auto text = TextColor::getTextColorCode(textColor);
			[[maybe_unused]] const auto background = BackgroundColor::getBackgroundColorCode(backgroundColor);
#if defined(OS_WINDOWS)
			m_attributes = getTextColor(stream);
			SetConsoleTextAttribute(getTerminalHandle(stream), (background << 4) + text);
#elif defined(OS_LINUX) || defined(OS_MACOS)
			stream << text << background;
#endif
		}

		static auto stylize(
			[[maybe_unused]] std::ostream& stream,
			[[maybe_unused]] TextStyle::TextStyleEnum textStyle) -> void
		{
#if defined(OS_LINUX) || defined(OS_MACOS)
			stream << TextStyle::getTextStyleCode(textStyle);
#endif
		}

		static auto reset(std::ostream& stream) -> void
		{
#if defined(OS_WINDOWS)
			SetConsoleTextAttribute(Terminal::getTerminalHandle(stream), m_attributes);
#elif defined(OS_LINUX) || defined(OS_MACOS)
			stream << ANSIEscapeCode::Reset;
#endif
		}

		static auto eraseCurrentLine([[maybe_unused]]std::ostream& stream) -> void
		{
#if defined(OS_LINUX) || defined(OS_MACOS)
			stream << ANSIEscapeCode::EraseCurrentLine;
#endif
		}

#if defined(OS_WINDOWS)
	private:
		static WORD m_attributes;
#endif
	};
}

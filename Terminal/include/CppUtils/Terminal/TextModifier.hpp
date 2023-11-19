#pragma once

#include <print>
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
		TextModifier() = default;

		explicit TextModifier(
			std::FILE* file,
			TextColor::TextColorEnum textColor = TextColor::TextColorEnum::Default,
			BackgroundColor::BackgroundColorEnum backgroundColor = BackgroundColor::BackgroundColorEnum::Default,
			[[maybe_unused]] std::optional<TextStyle::TextStyleEnum> textStyle = std::nullopt):
			m_modify{true},
			m_file{file}
		{
			[[maybe_unused]] const auto text = TextColor::getTextColorCode(textColor);
			[[maybe_unused]] const auto background = BackgroundColor::getBackgroundColorCode(backgroundColor);
#if defined(OS_WINDOWS)
			m_attributes = getTextColor(stream);
			SetConsoleTextAttribute(getTerminalHandle(m_file), (background << 4) + text);
#elif defined(OS_LINUX) || defined(OS_MACOS)
			std::print(m_file, "{}{}", text, background);
			if (textStyle)
				std::print(m_file, "{}", TextStyle::getTextStyleCode(*textStyle));
#endif
		}

		TextModifier(TextModifier&& other) noexcept = default;
		TextModifier& operator=(TextModifier&& other) noexcept = default;

		~TextModifier()
		{
			if (!m_modify)
				return;
#if defined(OS_WINDOWS)
			SetConsoleTextAttribute(Terminal::getTerminalHandle(m_file), m_attributes);
#elif defined(OS_LINUX) || defined(OS_MACOS)
			std::print(m_file, "{}", ANSIEscapeCode::Reset);
#endif
		}

#if defined(OS_WINDOWS)
		[[nodiscard]] static auto getTextColor(std::FILE* file) -> WORD
		{
			CONSOLE_SCREEN_BUFFER_INFO info;
			if (!GetConsoleScreenBufferInfo(getTerminalHandle(file), &info))
				return 0;
			return info.wAttributes;
		}

		[[nodiscard]] static auto getTextColor(std::ostream& stream) -> WORD
		{
			CONSOLE_SCREEN_BUFFER_INFO info;
			if (!GetConsoleScreenBufferInfo(getTerminalHandle(stream), &info))
				return 0;
			return info.wAttributes;
		}
#endif

		static auto eraseCurrentLine([[maybe_unused]]std::ostream& stream) -> void
		{
#if defined(OS_LINUX) || defined(OS_MACOS)
			stream << ANSIEscapeCode::EraseCurrentLine;
#endif
		}

	private:
		bool m_modify = false;
		std::FILE* m_file;
#if defined(OS_WINDOWS)
		WORD m_attributes;
#endif
	};
}

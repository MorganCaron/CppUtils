#include <CppUtils/Terminal/TextModifier.hpp>
#include <CppUtils/Platform/Windows.hpp>

namespace CppUtils::Terminal
{
		TextModifier::TextModifier(
			std::FILE* file,
			TextColor::TextColorEnum textColor,
			BackgroundColor::BackgroundColorEnum backgroundColor,
			[[maybe_unused]] std::optional<TextStyle::TextStyleEnum> textStyle):
			m_modify{true},
			m_file{file}
		{
			[[maybe_unused]] const auto text = TextColor::getTextColorCode(textColor);
			[[maybe_unused]] const auto background = BackgroundColor::getBackgroundColorCode(backgroundColor);
#if defined(OS_WINDOWS)
			m_attributes = getTextColor(m_file);
			SetConsoleTextAttribute(getTerminalHandle(m_file), static_cast<std::uint16_t>((background << 4) + text));
#elif defined(OS_LINUX) || defined(OS_MACOS)
			std::print(m_file, "{}{}", text, background);
			if (textStyle)
				std::print(m_file, "{}", TextStyle::getTextStyleCode(*textStyle));
#endif
		}

		TextModifier::~TextModifier()
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
		auto TextModifier::getTextColor(std::FILE* file) -> std::uint16_t
		{
			CONSOLE_SCREEN_BUFFER_INFO info;
			if (!GetConsoleScreenBufferInfo(getTerminalHandle(file), &info))
				return 0;
			return info.wAttributes;
		}

		auto TextModifier::getTextColor(std::ostream& stream) -> std::uint16_t
		{
			CONSOLE_SCREEN_BUFFER_INFO info;
			if (!GetConsoleScreenBufferInfo(getTerminalHandle(stream), &info))
				return 0;
			return info.wAttributes;
		}
#endif
}

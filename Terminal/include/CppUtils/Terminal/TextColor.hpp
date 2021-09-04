#pragma once

#include <cstdint>
#include <string_view>

#include <CppUtils/Platform/OS.hpp>

namespace CppUtils::Terminal::TextColor
{
	using namespace std::literals;
	
	enum class TextColorEnum: char
	{
		Default,
		Black,
		Red,
		Green,
		Yellow,
		Blue,
		Magenta,
		Cyan,
		White
	};
#if defined(OS_WINDOWS)
	namespace Attribute
	{
		[[maybe_unused]] static constexpr uint8_t Black = 0;
		[[maybe_unused]] static constexpr uint8_t Red = FOREGROUND_RED;
		[[maybe_unused]] static constexpr uint8_t Green = FOREGROUND_GREEN;
		[[maybe_unused]] static constexpr uint8_t Yellow = FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_INTENSITY;
		[[maybe_unused]] static constexpr uint8_t Blue = FOREGROUND_BLUE;
		[[maybe_unused]] static constexpr uint8_t Magenta = 13;
		[[maybe_unused]] static constexpr uint8_t Cyan = 11;
		[[maybe_unused]] static constexpr uint8_t White = 15;
		[[maybe_unused]] static constexpr uint8_t Default = FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_BLUE;
	}

	[[nodiscard]] inline uint8_t getTextColorCode(TextColorEnum textColor)
	{
		switch (textColor)
		{
			case TextColorEnum::Default:
				return Attribute::Default;
			case TextColorEnum::Black:
				return Attribute::Black;
			case TextColorEnum::Red:
				return Attribute::Red;
			case TextColorEnum::Green:
				return Attribute::Green;
			case TextColorEnum::Yellow:
				return Attribute::Yellow;
			case TextColorEnum::Blue:
				return Attribute::Blue;
			case TextColorEnum::Magenta:
				return Attribute::Magenta;
			case TextColorEnum::Cyan:
				return Attribute::Cyan;
			case TextColorEnum::White:
				return Attribute::White;
			default:
				return Attribute::Default;
		}
	}
#elif defined(OS_LINUX) || defined(OS_MACOS)
	namespace ANSIEscapeCode
	{
		[[maybe_unused]] static constexpr auto Black = "\x1B[30m"sv;
		[[maybe_unused]] static constexpr auto Red = "\x1B[31m"sv;
		[[maybe_unused]] static constexpr auto Green = "\x1B[32m"sv;
		[[maybe_unused]] static constexpr auto Yellow = "\x1B[33m"sv;
		[[maybe_unused]] static constexpr auto Blue = "\x1B[34m"sv;
		[[maybe_unused]] static constexpr auto Magenta = "\x1B[35m"sv;
		[[maybe_unused]] static constexpr auto Cyan = "\x1B[36m"sv;
		[[maybe_unused]] static constexpr auto White = "\x1B[37m"sv;
		[[maybe_unused]] static constexpr auto Default = "\x1B[39m"sv;
	}

	[[nodiscard]] inline std::string_view getTextColorCode(TextColorEnum textColor)
	{
		switch (textColor)
		{
			case TextColorEnum::Default:
				return ANSIEscapeCode::Default;
			case TextColorEnum::Black:
				return ANSIEscapeCode::Black;
			case TextColorEnum::Red:
				return ANSIEscapeCode::Red;
			case TextColorEnum::Green:
				return ANSIEscapeCode::Green;
			case TextColorEnum::Yellow:
				return ANSIEscapeCode::Yellow;
			case TextColorEnum::Blue:
				return ANSIEscapeCode::Blue;
			case TextColorEnum::Magenta:
				return ANSIEscapeCode::Magenta;
			case TextColorEnum::Cyan:
				return ANSIEscapeCode::Cyan;
			case TextColorEnum::White:
				return ANSIEscapeCode::White;
			default:
				return ANSIEscapeCode::Default;
		}
	}
#endif
}

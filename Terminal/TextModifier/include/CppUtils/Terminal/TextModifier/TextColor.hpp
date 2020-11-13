#pragma once

#include <cstdint>
#include <string_view>

#include <CppUtils/Platform/OS.hpp>

namespace CppUtils::Terminal::TextModifier::TextColor
{
	using namespace std::literals;
	
	enum class TextColorEnum
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
				break;
			case TextColorEnum::Black:
				return Attribute::Black;
				break;
			case TextColorEnum::Red:
				return Attribute::Red;
				break;
			case TextColorEnum::Green:
				return Attribute::Green;
				break;
			case TextColorEnum::Yellow:
				return Attribute::Yellow;
				break;
			case TextColorEnum::Blue:
				return Attribute::Blue;
				break;
			case TextColorEnum::Magenta:
				return Attribute::Magenta;
				break;
			case TextColorEnum::Cyan:
				return Attribute::Cyan;
				break;
			case TextColorEnum::White:
				return Attribute::White;
				break;
			default:
				return Attribute::Default;
				break;
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
				break;
			case TextColorEnum::Black:
				return ANSIEscapeCode::Black;
				break;
			case TextColorEnum::Red:
				return ANSIEscapeCode::Red;
				break;
			case TextColorEnum::Green:
				return ANSIEscapeCode::Green;
				break;
			case TextColorEnum::Yellow:
				return ANSIEscapeCode::Yellow;
				break;
			case TextColorEnum::Blue:
				return ANSIEscapeCode::Blue;
				break;
			case TextColorEnum::Magenta:
				return ANSIEscapeCode::Magenta;
				break;
			case TextColorEnum::Cyan:
				return ANSIEscapeCode::Cyan;
				break;
			case TextColorEnum::White:
				return ANSIEscapeCode::White;
				break;
			default:
				return ANSIEscapeCode::Default;
				break;
		}
	}
#endif
}

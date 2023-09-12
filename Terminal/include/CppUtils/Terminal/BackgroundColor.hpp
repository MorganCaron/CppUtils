#pragma once

#include <cstdint>
#include <string_view>

#include <CppUtils/Platform/OS.hpp>

namespace CppUtils::Terminal::BackgroundColor
{
	using namespace std::literals;
	
	enum class BackgroundColorEnum: char
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
		namespace
		{
			[[maybe_unused]] constexpr uint8_t Black = 0;
			[[maybe_unused]] constexpr uint8_t Red = BACKGROUND_RED;
			[[maybe_unused]] constexpr uint8_t Green = BACKGROUND_GREEN;
			[[maybe_unused]] constexpr uint8_t Yellow = BACKGROUND_RED | BACKGROUND_GREEN | BACKGROUND_INTENSITY;
			[[maybe_unused]] constexpr uint8_t Blue = BACKGROUND_BLUE;
			[[maybe_unused]] constexpr uint8_t Magenta = 13;
			[[maybe_unused]] constexpr uint8_t Cyan = 11;
			[[maybe_unused]] constexpr uint8_t White = 15;
			[[maybe_unused]] constexpr uint8_t Default = 0;
		}
	}

	[[nodiscard]] constexpr uint8_t getBackgroundColorCode(BackgroundColorEnum backgroundColor)
	{
		switch (backgroundColor)
		{
			case BackgroundColorEnum::Default:
				return Attribute::Default;
			case BackgroundColorEnum::Black:
				return Attribute::Black;
			case BackgroundColorEnum::Red:
				return Attribute::Red;
			case BackgroundColorEnum::Green:
				return Attribute::Green;
			case BackgroundColorEnum::Yellow:
				return Attribute::Yellow;
			case BackgroundColorEnum::Blue:
				return Attribute::Blue;
			case BackgroundColorEnum::Magenta:
				return Attribute::Magenta;
			case BackgroundColorEnum::Cyan:
				return Attribute::Cyan;
			case BackgroundColorEnum::White:
				return Attribute::White;
			default:
				return Attribute::Default;
		}
	}
#elif defined(OS_LINUX) || defined(OS_MACOS)
	namespace ANSIEscapeCode
	{
		namespace
		{
			[[maybe_unused]] constexpr auto Black = "\x1B[40m"sv;
			[[maybe_unused]] constexpr auto Red = "\x1B[41m"sv;
			[[maybe_unused]] constexpr auto Green = "\x1B[42m"sv;
			[[maybe_unused]] constexpr auto Yellow = "\x1B[43m"sv;
			[[maybe_unused]] constexpr auto Blue = "\x1B[44m"sv;
			[[maybe_unused]] constexpr auto Magenta = "\x1B[45m"sv;
			[[maybe_unused]] constexpr auto Cyan = "\x1B[46m"sv;
			[[maybe_unused]] constexpr auto White = "\x1B[47m"sv;
			[[maybe_unused]] constexpr auto Default = "\x1B[49m"sv;
		}
	}

	[[nodiscard]] constexpr std::string_view getBackgroundColorCode(BackgroundColorEnum backgroundColor)
	{
		switch (backgroundColor)
		{
			case BackgroundColorEnum::Default:
				return ANSIEscapeCode::Default;
			case BackgroundColorEnum::Black:
				return ANSIEscapeCode::Black;
			case BackgroundColorEnum::Red:
				return ANSIEscapeCode::Red;
			case BackgroundColorEnum::Green:
				return ANSIEscapeCode::Green;
			case BackgroundColorEnum::Yellow:
				return ANSIEscapeCode::Yellow;
			case BackgroundColorEnum::Blue:
				return ANSIEscapeCode::Blue;
			case BackgroundColorEnum::Magenta:
				return ANSIEscapeCode::Magenta;
			case BackgroundColorEnum::Cyan:
				return ANSIEscapeCode::Cyan;
			case BackgroundColorEnum::White:
				return ANSIEscapeCode::White;
			default:
				return ANSIEscapeCode::Default;
		}
	}
#endif
}

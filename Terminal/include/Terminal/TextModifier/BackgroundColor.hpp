#pragma once

#include <cstdint>
#include <string_view>

#include <Platform/OS.hpp>

using namespace std::literals;

namespace CppUtils::Terminal::TextModifier::BackgroundColor
{
	enum class BackgroundColorEnum
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
		[[maybe_unused]] static constexpr const uint8_t Black = 0;
		[[maybe_unused]] static constexpr const uint8_t Red = BACKGROUND_RED;
		[[maybe_unused]] static constexpr const uint8_t Green = BACKGROUND_GREEN;
		[[maybe_unused]] static constexpr const uint8_t Yellow = BACKGROUND_RED | BACKGROUND_GREEN | BACKGROUND_INTENSITY;
		[[maybe_unused]] static constexpr const uint8_t Blue = BACKGROUND_BLUE;
		[[maybe_unused]] static constexpr const uint8_t Magenta = 13;
		[[maybe_unused]] static constexpr const uint8_t Cyan = 11;
		[[maybe_unused]] static constexpr const uint8_t White = 15;
		[[maybe_unused]] static constexpr const uint8_t Default = 0;
	}

	inline uint8_t getBackgroundColorCode(BackgroundColorEnum backgroundColor)
	{
		switch (backgroundColor)
		{
			case BackgroundColorEnum::Default:
				return Attribute::Default;
				break;
			case BackgroundColorEnum::Black:
				return Attribute::Black;
				break;
			case BackgroundColorEnum::Red:
				return Attribute::Red;
				break;
			case BackgroundColorEnum::Green:
				return Attribute::Green;
				break;
			case BackgroundColorEnum::Yellow:
				return Attribute::Yellow;
				break;
			case BackgroundColorEnum::Blue:
				return Attribute::Blue;
				break;
			case BackgroundColorEnum::Magenta:
				return Attribute::Magenta;
				break;
			case BackgroundColorEnum::Cyan:
				return Attribute::Cyan;
				break;
			case BackgroundColorEnum::White:
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

	inline std::string_view getBackgroundColorCode(BackgroundColorEnum backgroundColor)
	{
		switch (backgroundColor)
		{
			case BackgroundColorEnum::Default:
				return ANSIEscapeCode::Default;
				break;
			case BackgroundColorEnum::Black:
				return ANSIEscapeCode::Black;
				break;
			case BackgroundColorEnum::Red:
				return ANSIEscapeCode::Red;
				break;
			case BackgroundColorEnum::Green:
				return ANSIEscapeCode::Green;
				break;
			case BackgroundColorEnum::Yellow:
				return ANSIEscapeCode::Yellow;
				break;
			case BackgroundColorEnum::Blue:
				return ANSIEscapeCode::Blue;
				break;
			case BackgroundColorEnum::Magenta:
				return ANSIEscapeCode::Magenta;
				break;
			case BackgroundColorEnum::Cyan:
				return ANSIEscapeCode::Cyan;
				break;
			case BackgroundColorEnum::White:
				return ANSIEscapeCode::White;
				break;
			default:
				return ANSIEscapeCode::Default;
				break;
		}
	}
#endif
}

#pragma once

#include <cstdint>
#include <string_view>

#include <Platform/OS.hpp>

using namespace std::literals;

namespace CppUtils::Terminal::TextModifier::TextColor
{
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
		[[maybe_unused]] static constexpr const uint8_t Black = 0;
		[[maybe_unused]] static constexpr const uint8_t Red = 12;
		[[maybe_unused]] static constexpr const uint8_t Green = 2;
		[[maybe_unused]] static constexpr const uint8_t Yellow = 14;
		[[maybe_unused]] static constexpr const uint8_t Blue = 9;
		[[maybe_unused]] static constexpr const uint8_t Magenta = 13;
		[[maybe_unused]] static constexpr const uint8_t Cyan = 11;
		[[maybe_unused]] static constexpr const uint8_t White = 15;
	}

	uint8_t getTextColorCode(TextColorEnum textColor)
	{
		switch (textColor)
		{
			case TextColorEnum::Default:
				return Attribute::White;
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
				return Attribute::White;
				break;
		}
	}
#elif defined(OS_LINUX) || defined(OS_MACOS)
	namespace ANSIEscapeCode
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

	std::string_view getTextColorCode(TextColorEnum textColor)
	{
		switch (textColor)
		{
			case TextColorEnum::Default:
				return ANSIEscapeCode::White;
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
				return "";
				break;
		}
	}
#endif
}

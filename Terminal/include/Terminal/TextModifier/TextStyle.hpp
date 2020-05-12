#pragma once

#include <string_view>

#include <Platform/OS.hpp>

using namespace std::literals;

namespace CppUtils::Terminal::TextModifier::TextStyle
{
	enum class TextStyleEnum
	{
		Bolder,
		Lighter,
		Italic,
		Underline
	};
#if defined(OS_LINUX) || defined(OS_MACOS)
	namespace ANSIEscapeCode
	{
		[[maybe_unused]] static constexpr const auto Bolder = "\x1B[1m"sv;
		[[maybe_unused]] static constexpr const auto Lighter = "\x1B[2m"sv;
		[[maybe_unused]] static constexpr const auto Italic = "\x1B[3m"sv;
		[[maybe_unused]] static constexpr const auto Underline = "\x1B[4m"sv;
	}

	std::string_view getTextStyleCode(TextStyleEnum textStyle)
	{
		switch (textStyle)
		{
			case TextStyleEnum::Bolder:
				return ANSIEscapeCode::Bolder;
				break;
			case TextStyleEnum::Lighter:
				return ANSIEscapeCode::Lighter;
				break;
			case TextStyleEnum::Italic:
				return ANSIEscapeCode::Italic;
				break;
			case TextStyleEnum::Underline:
				return ANSIEscapeCode::Underline;
				break;
			default:
				return "";
				break;
		}
	}
#endif
}

#pragma once

#include <string_view>

#include <CppUtils/Platform/OS.hpp>

namespace CppUtils::Terminal::TextModifier::TextStyle
{
	using namespace std::literals;
	
	enum class TextStyleEnum: char
	{
		Bolder,
		Lighter,
		Italic,
		Underline
	};
#if defined(OS_LINUX) || defined(OS_MACOS)
	namespace ANSIEscapeCode
	{
		[[maybe_unused]] static constexpr auto Bolder = "\x1B[1m"sv;
		[[maybe_unused]] static constexpr auto Lighter = "\x1B[2m"sv;
		[[maybe_unused]] static constexpr auto Italic = "\x1B[3m"sv;
		[[maybe_unused]] static constexpr auto Underline = "\x1B[4m"sv;
	}

	[[nodiscard]] inline std::string_view getTextStyleCode(TextStyleEnum textStyle)
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

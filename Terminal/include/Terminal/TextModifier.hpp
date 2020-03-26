#pragma once

#include <ostream>
#include <sstream>
#include <iterator>
#include <string_view>

using namespace std::literals;

namespace CppUtils::Terminal
{
	class TextModifier
	{
	public:
		enum class SelectGraphicRenditionCode
		{
			TextStyle_Bolder = 1,
			TextStyle_Lighter = 2,
			TextStyle_Italic = 3,
			TextStyle_Underline = 4,
			TextColor_Black = 30,
			TextColor_Red = 31,
			TextColor_Green = 32,
			TextColor_Yellow = 33,
			TextColor_Blue = 34,
			TextColor_Magenta = 35,
			TextColor_Cyan = 36,
			TextColor_White = 37,
			TextColor_Default = 39,
			BackgroundColor_Black = 40,
			BackgroundColor_Red = 41,
			BackgroundColor_Green = 42,
			BackgroundColor_Yellow = 43,
			BackgroundColor_Blue = 44,
			BackgroundColor_Magenta = 45,
			BackgroundColor_Cyan = 46,
			BackgroundColor_White = 47,
			BackgroundColor_Default = 49
		};
		using SGRCode = SelectGraphicRenditionCode;
		
		static constexpr const auto Esc = "\033"sv;
		static constexpr const auto Reset = "\033c"sv;

		explicit TextModifier(const std::vector<SGRCode>& codes)
		{
			std::ostringstream oss;
			oss << Esc << '[';
			for (std::size_t i = 0; i < codes.size(); ++i)
			{
				oss << static_cast<int>(codes[i]);
				if (i > 0)
					oss << ';';
			}
			oss << 'm';
			m_AnsiEscapeCode = oss.str();
		}

		inline std::string getAnsiEscapeCode() const
		{
			return m_AnsiEscapeCode;
		}

	private:
		std::string m_AnsiEscapeCode;
	};

	inline std::ostream& operator<<(std::ostream& os, const TextModifier& modifier)
	{
		return os << modifier.getAnsiEscapeCode();
	}
}

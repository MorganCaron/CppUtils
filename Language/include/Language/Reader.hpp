#pragma once

#include <cctype>
#include <algorithm>
#include <string_view>

namespace CppUtils::Language::Reader
{
	struct Cursor
	{
		Cursor(std::string_view c_src, std::size_t& c_pos):
			src{c_src},
			pos{c_pos}
		{}

		[[nodiscard]] inline bool isEndOfString() const noexcept
		{
			return (pos >= src.length());
		}
		
		[[nodiscard]] inline char getChar() const
		{
			return src.at(pos);
		}

		[[nodiscard]] inline char getCharAndSkipIt()
		{
			return src.at(pos++);
		}

		[[nodiscard]] inline std::string_view getStringIf(const std::function<bool(char)>& validator) const
		{
			const auto length = src.length();
			auto nbChars = 0u;
			while (nbChars < length && validator(src.at(pos + nbChars)))
				++nbChars;
			return src.substr(pos, nbChars);
		}

		[[nodiscard]] inline std::string_view getStringAndSkipItIf(const std::function<bool(char)>& validator)
		{
			const auto string = getStringIf(validator);
			pos += string.length();
			return string;
		}

		inline void skipStringIf(const std::function<bool(char)>& validator)
		{
			while (!isEndOfString() && validator(getChar()))
				++pos;
		}

		inline void skipSpaces()
		{
			skipStringIf([](const char c) -> bool {
				return std::isspace(static_cast<unsigned char>(c));
			});
		}

		[[nodiscard]] inline std::string_view getNextNChar(const std::size_t size) const
		{
			return src.substr(pos, std::min(size, src.length() - pos));
		}

		[[nodiscard]] std::string_view getWord() const
		{
			return getStringIf([](const char c) -> bool {
				return std::isalpha(static_cast<unsigned char>(c));
			});
		}

		[[nodiscard]] std::string_view getWordAndSkipIt()
		{
			auto word = getWord();
			pos += word.length();
			return word;
		}

		[[nodiscard]] std::string_view getKeyword() const
		{
			const auto srcLength = src.length();
			auto subPosition = pos;

			if (subPosition < srcLength && (std::isalpha(src.at(subPosition)) || src.at(subPosition) == '_'))
			{
				do
					++subPosition;
				while (subPosition < srcLength && (std::isalnum(src.at(subPosition)) || src.at(subPosition) == '_'));
			}
			return src.substr(pos, subPosition - pos);
		}

		[[nodiscard]] std::string_view getKeywordAndSkipIt()
		{
			auto keyword = getKeyword();
			pos += keyword.length();
			return keyword;
		}

		[[nodiscard]] std::string_view getKeywordRequired(std::string_view errorMessage)
		{
			auto keyword = getKeywordAndSkipIt();
			if (keyword.empty())
				throw std::runtime_error{errorMessage.data()};
			return keyword;
		}

		bool isEqualSkipIt(std::string_view str)
		{
			const auto length = str.length();
			if (getNextNChar(length) != str)
				return false;
			pos += length;
			return true;
		}

		std::string_view src;
		std::size_t& pos;
	};
}

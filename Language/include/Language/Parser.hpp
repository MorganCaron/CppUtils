#pragma once

#include <cctype>
#include <algorithm>
#include <string_view>

namespace CppUtils::Language::Parser
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

		inline void skipSpaces()
		{
			while (!isEndOfString() && std::isspace(getChar()))
				++pos;
		}

		[[nodiscard]] inline std::string getNextNChar(const std::size_t size) const
		{
			return std::string{src.substr(pos, std::min(size, src.length() - pos))};
		}

		[[nodiscard]] std::string getWord() const
		{
			auto wordLength = std::size_t{0};

			while (std::isalpha(src.at(pos + wordLength)))
				++wordLength;
			return std::string{src.substr(pos, wordLength)};
		}

		[[nodiscard]] std::string getWordAndSkipIt()
		{
			auto word = getWord();
			pos += word.length();
			return word;
		}

		[[nodiscard]] std::string getKeyword() const
		{
			const auto srcLength = src.length();
			auto subPosition = pos;

			if (subPosition < srcLength && (std::isalpha(src.at(subPosition)) || src.at(subPosition) == '_'))
			{
				do
					++subPosition;
				while (subPosition < srcLength && (std::isalnum(src.at(subPosition)) || src.at(subPosition) == '_'));
			}
			return std::string{src.substr(pos, subPosition - pos)};
		}

		[[nodiscard]] std::string getKeywordAndSkipIt()
		{
			auto keyword = getKeyword();
			pos += keyword.length();
			return keyword;
		}

		[[nodiscard]] std::string getKeywordRequired(std::string_view errorMessage)
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

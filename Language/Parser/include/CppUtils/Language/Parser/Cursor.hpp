#pragma once

#include <span>
#include <cctype>
#include <algorithm>
#include <string_view>

namespace CppUtils::Language::Parser
{
	template<typename Element>
	struct Cursor final
	{
		[[nodiscard]] inline bool isEnd() const noexcept
		{
			return (position >= elements.size());
		}

		[[nodiscard]] inline const Element& getElement() const
		{
			return elements[position];
		}

		[[nodiscard]] inline const Element& getElementAndSkipIt()
		{
			return elements[position++];
		}

		[[nodiscard]] inline std::size_t end() const noexcept
		{
			return elements.size();
		}

		std::span<const Element> elements;
		std::size_t position;
	};

	template<>
	struct Cursor<std::string> final
	{
		Cursor(std::string_view c_src, std::size_t& c_position):
			src{c_src},
			position{c_position}
		{}

		[[nodiscard]] inline bool isEndOfString() const noexcept
		{
			return (position >= src.length());
		}
		
		[[nodiscard]] inline char getChar() const
		{
			return src.at(position);
		}

		[[nodiscard]] inline char getCharAndSkipIt()
		{
			return src.at(position++);
		}

		[[nodiscard]] inline std::string_view getStringIf(const std::function<bool(char)>& validator) const
		{
			const auto length = src.length();
			auto nbChars = 0u;
			while (nbChars < length && validator(src.at(position + nbChars)))
				++nbChars;
			return src.substr(position, nbChars);
		}

		[[nodiscard]] inline std::string_view getStringAndSkipItIf(const std::function<bool(char)>& validator)
		{
			const auto string = getStringIf(validator);
			position += string.length();
			return string;
		}

		inline void skipStringIf(const std::function<bool(char)>& validator)
		{
			while (!isEndOfString() && validator(getChar()))
				++position;
		}

		inline void skipSpaces()
		{
			skipStringIf([](const char c) -> bool {
				return std::isspace(static_cast<unsigned char>(c));
			});
		}

		[[nodiscard]] inline std::string_view getNextNChar(const std::size_t size) const
		{
			return src.substr(position, std::min(size, src.length() - position));
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
			position += word.length();
			return word;
		}

		[[nodiscard]] std::string_view getKeyword() const
		{
			const auto srcLength = src.length();
			auto subPosition = position;

			if (subPosition < srcLength && (std::isalpha(src.at(subPosition)) || src.at(subPosition) == '_'))
			{
				do
					++subPosition;
				while (subPosition < srcLength && (std::isalnum(src.at(subPosition)) || src.at(subPosition) == '_'));
			}
			return src.substr(position, subPosition - position);
		}

		[[nodiscard]] std::string_view getKeywordAndSkipIt()
		{
			auto keyword = getKeyword();
			position += keyword.length();
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
			position += length;
			return true;
		}

		std::string_view src;
		std::size_t& position;
	};
}

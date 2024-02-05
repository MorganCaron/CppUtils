#pragma once

#include <concepts>
#include <cstddef>
#include <span>
#include <string_view>

#include <CppUtils/String/Concept.hpp>
#include <CppUtils/Terminal/Terminal.hpp>

namespace CppUtils::Language::Parser
{
	template<typename Element = char>
	struct Cursor final
	{
		std::span<const Element> data;
		std::size_t position = 0;

		[[nodiscard]] constexpr bool isEnd() const
		{
			return position >= data.size();
		}
		[[nodiscard]] constexpr auto operator[](std::size_t position) const -> const Element&
		{
			return data[position];
		}
		[[nodiscard]] constexpr auto current() const -> const Element&
		{
			return data[position];
		}
		[[nodiscard]] constexpr auto read() -> const Element&
		{
			return data[position++];
		}
		[[nodiscard]] constexpr auto is(const Element& value) const -> bool
		{
			return !isEnd() and current() == value;
		}
	};

	template<class CharT>
	Cursor(std::basic_string_view<CharT>, std::size_t position = 0) -> Cursor<CharT>;

	template<String::Concept::Char CharT>
	[[nodiscard]] constexpr auto getLineNumber(const Cursor<CharT>& cursor) -> std::size_t
	{
		const auto subSpan = cursor.data.subspan(0, cursor.position);
		return std::count(std::cbegin(subSpan), std::cend(subSpan), '\n') + 1;
	}

	template<String::Concept::Char CharT>
	[[nodiscard]] constexpr auto getPositionInTheLine(const Cursor<CharT>& cursor) noexcept -> std::size_t
	{
		auto lineLength = 0u;
		while (cursor.position - lineLength >= 0 and cursor.data[cursor.position - lineLength] != '\n')
			++lineLength;
		return lineLength;
	}

	template<String::Concept::Char CharT>
	[[nodiscard]] constexpr auto getSample(const Cursor<CharT>& cursor, std::size_t length) -> std::basic_string<CharT>
	{
		if constexpr (!String::Concept::Char<CharT>)
			return std::basic_string<CharT>{};

		auto startPosition = cursor.position - std::min(getPositionInTheLine(cursor), length);

		auto endPosition = cursor.position;
		while (endPosition < std::size(cursor.data) and endPosition < startPosition + length and cursor.data[endPosition] != '\n')
			++endPosition;

		auto sample = std::basic_string<CharT>{std::cbegin(cursor.data) + startPosition, std::cbegin(cursor.data) + endPosition};
		std::replace(std::begin(sample), std::end(sample), '\t', ' ');

		auto arrowOffset = cursor.position - startPosition;
		auto charOffset = arrowOffset == 0 ? arrowOffset : (arrowOffset == length * 2 + 1 ? arrowOffset - 2 : arrowOffset - 1);

		// Todo: utiliser std::format ici:
		if (!empty(sample))
			sample += '\n' + std::basic_string<CharT>(arrowOffset, ' ') + "↑\n";
		sample += std::basic_string<CharT>(charOffset, ' ') + '\'' + String::reverseEscapedChar(cursor.current()) + "'\n";
		return sample;
	}

	// Todo: constexpr
	template<class Element>
	[[nodiscard]] auto getPositionInformation(const CppUtils::Language::Parser::Cursor<Element>& cursor) -> std::string
	{
		// Todo: utiliser std::format ici:
		// std::cout << std::basic_string<Element>{std::cbegin(cursor.data), std::cend(cursor.data)} << std::endl;
		if constexpr (String::Concept::Char<Element>)
			return "At line " + std::to_string(getLineNumber(cursor)) +
				" position " + std::to_string(getPositionInTheLine(cursor)) + ":\n" +
				getSample(cursor, Terminal::getTerminalSize().width);
		else
			return "\nAt position " + std::to_string(cursor.position);
	}
}

#pragma once

#include <span>
#include <cstddef>
#include <concepts>
#include <string_view>

#include <CppUtils/String/Concept.hpp>

namespace CppUtils::Language::Parser
{
	template<typename Element = char>
	struct Cursor final
	{
		std::span<const Element> data;
		std::size_t pos = 0;
		
		[[nodiscard]] constexpr bool isEnd() const
		{
			return pos >= data.size();
		}
		[[nodiscard]] constexpr auto operator[](std::size_t position) const -> const Element&
		{
			return data[position];
		}
		[[nodiscard]] constexpr auto current() const -> const Element&
		{
			return data[pos];
		}
		[[nodiscard]] constexpr auto read() -> const Element&
		{
			return data[pos++];
		}
		[[nodiscard]] constexpr auto is(const Element& value) const -> bool
		{
			return !isEnd() && current() == value;
		}
	};

	template<class CharT>
	Cursor(std::basic_string_view<CharT>, std::size_t pos = 0) -> Cursor<CharT>;

	template<String::Concept::Char CharT>
	[[nodiscard]] auto getLineNumber(const Cursor<CharT>& cursor) -> std::size_t
	{
		const auto subSpan = cursor.data.subspan(0, cursor.pos);
		return std::count(std::cbegin(subSpan), std::cend(subSpan), '\n') + 1;
	}

	template<String::Concept::Char CharT>
	[[nodiscard]] auto getPositionInTheLine(const Cursor<CharT>& cursor) noexcept -> std::size_t
	{
		auto pos = std::size(cursor.data);
		while (pos > 0 && cursor.data[pos] != '\n')
			--pos;
		return std::size(cursor.data) - pos;
	}

	template<class Element>
	[[nodiscard]] auto getPositionInformation(const CppUtils::Language::Parser::Cursor<Element>& cursor) -> std::string
	{
		if constexpr (String::Concept::Char<Element>)
			return "At line " + std::to_string(getLineNumber(cursor)) +
				", position " + std::to_string(getPositionInTheLine(cursor));
		else
			return "\nAt position " + std::to_string(cursor.pos);
	}
}

#pragma once

#include <span>
#include <cstddef>
#include <concepts>
#include <string_view>

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

	Cursor(std::string_view data, std::size_t pos = 0) -> Cursor<char>;
	Cursor(std::wstring_view data, std::size_t pos = 0) -> Cursor<wchar_t>;
	Cursor(std::u8string_view data, std::size_t pos = 0) -> Cursor<char8_t>;
	Cursor(std::u16string_view data, std::size_t pos = 0) -> Cursor<char16_t>;
	Cursor(std::u32string_view data, std::size_t pos = 0) -> Cursor<char32_t>;

	template<std::integral Char>
	[[nodiscard]] std::size_t getLineNumber(const Cursor<Char>& cursor)
	{
		const auto subSpan = cursor.data.subspan(0, cursor.pos);
		return std::count(std::cbegin(subSpan), std::cend(subSpan), '\n') + 1;
	}

	template<std::integral Char>
	[[nodiscard]] std::size_t getPositionInTheLine(const Cursor<Char>& cursor) noexcept
	{
		auto pos = std::size(cursor.data);
		while (pos > 0 && cursor.data[pos] != '\n')
			--pos;
		return std::size(cursor.data) - pos;
	}

	template<class Element>
	[[nodiscard]] auto getPositionInformation(const CppUtils::Language::Parser::Cursor<Element>& cursor) -> std::string
	{
		if constexpr (std::integral<Element>)
			return "\nAt line " + std::to_string(getLineNumber(cursor)) +
				", position " + std::to_string(getPositionInTheLine(cursor));
		else
			return "\nAt position " + std::to_string(cursor.pos);
	}
}

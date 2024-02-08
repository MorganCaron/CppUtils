#pragma once

#include <string>

namespace Stl
{
	// https://en.cppreference.com/w/cpp/string/basic_string_view
	template<class CharT, class Traits = std::char_traits<CharT>>
	class basic_string_view
	{
		using traits_type = Traits;
		using value_type = CharT;
		using pointer = CharT*;
		using const_pointer = const CharT*;
		using reference = CharT&;
		using const_reference = const CharT&;
		// using const_iterator =
		// using iterator = const_iterator;
		using size_type = std::size_t;
		using difference_type = std::ptrdiff_t;
	};

	using string_view = basic_string_view<char>;
	using wstring_view = basic_string_view<wchar_t>;
	using u8string_view = basic_string_view<char8_t>;
	using u16string_view = basic_string_view<char16_t>;
	using u32string_view = basic_string_view<char32_t>;
}

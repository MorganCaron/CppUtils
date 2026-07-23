#pragma once

#include <cstdio>
#include <utility>

namespace Stl
{
	// https://en.cppreference.com/cpp/header/print

	// https://en.cppreference.com/cpp/io/print
	template<class CharT, class... Args>
	auto print(std::basic_string_view<CharT> format, Args&&... args) -> void
	{
		std::puts(format(format, std::forward<Args>(args)...).c_str());
	}

	// https://en.cppreference.com/cpp/io/println
	template<class CharT, class... Args>
	auto println(std::basic_string_view<CharT> format, Args&&... args) -> void
	{
		print(format + '\n', std::forward<Args>(args)...);
	}
}

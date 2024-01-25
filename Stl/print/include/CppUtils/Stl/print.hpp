#pragma once

#include <cstdio>

#include <Stl/print.hpp>

// https://en.cppreference.com/w/cpp/header/print
namespace CppUtils::Stl
{
	// https://en.cppreference.com/w/cpp/io/print
	template<class CharT, class... Args>
	auto print(std::basic_string_view<CharT> format, Args&&... args) -> void
	{
		std::puts(format(format, std::forward<Args>(args)...).c_str());
	}

	// https://en.cppreference.com/w/cpp/io/println
	template<class CharT, class... Args>
	auto println(std::basic_string_view<CharT> format, Args&&... args) -> void
	{
		print(format + '\n', std::forward<Args>(args)...);
	}
}

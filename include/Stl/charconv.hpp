#pragma once

#include <charconv>
#include <clocale>
#include <concepts>
#include <cstdlib>
#include <memory>
#include <string>
#include <system_error>

#if defined(__APPLE__)
#	include <xlocale.h>
#endif

namespace Stl
{
	using std::chars_format;
	using std::from_chars_result;

#if defined(__APPLE__)
	template<class T>
	requires std::integral<T>
	[[nodiscard]] inline auto from_chars(const char* first, const char* last, T& value, int base = 10) -> from_chars_result
	{
		const auto chunk = std::string(first, last);
		char* end = nullptr;

		if constexpr (std::signed_integral<T>)
		{
			const auto result = std::strtoll(chunk.c_str(), std::addressof(end), base);
			value = static_cast<T>(result);
		}
		else
		{
			const auto result = std::strtoull(chunk.c_str(), std::addressof(end), base);
			value = static_cast<T>(result);
		}

		if (end == chunk.c_str())
			return {first, std::errc::invalid_argument};

		return {first + (end - chunk.c_str()), std::errc{}};
	}

	template<class T>
	requires std::floating_point<T>
	[[nodiscard]] inline auto from_chars(const char* first, const char* last, T& value, [[maybe_unused]] chars_format fmt = chars_format::general) -> from_chars_result
	{
		const auto chunk = std::string(first, last);
		char* end = nullptr;
		static locale_t c_locale = newlocale(LC_ALL_MASK, "C", nullptr);

		if constexpr (std::same_as<T, float>)
			value = strtof_l(chunk.c_str(), std::addressof(end), c_locale);
		else if constexpr (std::same_as<T, double>)
			value = strtod_l(chunk.c_str(), std::addressof(end), c_locale);
		else if constexpr (std::same_as<T, long double>)
			value = strtold_l(chunk.c_str(), std::addressof(end), c_locale);

		if (end == chunk.c_str())
			return {first, std::errc::invalid_argument};

		return {first + (end - chunk.c_str()), std::errc{}};
	}
#else
	using std::from_chars;
#endif
}

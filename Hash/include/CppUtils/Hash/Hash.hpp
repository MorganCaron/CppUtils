#pragma once

#include <span>
#include <string_view>

namespace CppUtils::Hash
{
	template<class T = char>
	[[nodiscard]] constexpr auto hash(std::span<const T> span) noexcept -> std::size_t
	{
		auto result = 0xcbf29ce484222325u;
		for (const auto& value : span)
			result = (static_cast<std::size_t>(value) ^ result) * 0x100000001b3u;
		return result;
	}

	[[nodiscard]] constexpr auto hash(std::string_view string) noexcept -> std::size_t
	{
		return hash<char>(string);
	}

	namespace Literals
	{
		[[nodiscard]] constexpr auto operator"" _hash(const char* cstring, std::size_t) noexcept -> std::size_t
		{
			return hash(cstring);
		}
	}
}

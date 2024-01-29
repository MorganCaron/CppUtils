#pragma once

#include <string_view>

namespace CppUtils::String
{
	using Hash = std::uint64_t;
	using Token = Hash;

	[[nodiscard]] inline constexpr auto hash(std::string_view string) noexcept -> Hash
	{
		auto result = 0xcb'f2'9c'e4'84'22'23'25u;
		for (const auto& c : string)
			result = (static_cast<Hash>(c) ^ result) * 0x1'00'00'00'01'b3u;
		return result;
	}

	template<class CharT = char, std::size_t N = 1>
	struct Hasher final
	{
		constexpr Hasher(Hash hash):
			hash{hash},
			name{'\0'}
		{}

		constexpr Hasher(const CharT (&cString)[N]):
			hash{CppUtils::String::hash(cString)}
		{
			std::copy(cString, cString + N, name.begin());
		}

		[[nodiscard]] constexpr operator Hash() const noexcept
		{
			return hash;
		}

		[[nodiscard]] constexpr operator std::basic_string_view<CharT>() const noexcept
		{
			return std::basic_string_view<CharT>{std::data(name), N - 1};
		}

		[[nodiscard]] constexpr operator std::basic_string<CharT>() const noexcept
		{
			return std::basic_string<CharT>{std::data(name), N - 1};
		}

		[[nodiscard]] constexpr auto operator==(Hash otherHash) const noexcept -> bool
		{
			return hash == otherHash;
		}

		Hash hash;
		std::array<CharT, N> name;
	};

	namespace Literals
	{
		[[nodiscard]] constexpr auto operator"" _hash(const char* cString, std::size_t size) noexcept -> Hash
		{
			return hash(std::string_view{cString, size});
		}

		[[nodiscard]] constexpr auto operator"" _token(const char* cString, std::size_t size) noexcept -> Token
		{
			return hash(std::string_view{cString, size});
		}

		static_assert("Number"_hash == "Number"_hash);
		static_assert("Number"_hash != "String"_hash);

		static_assert("Number"_token == "Number"_token);
		static_assert("Number"_token != "String"_token);

		static_assert("Number"_hash == "Number"_token);
		static_assert("Number"_hash == "Number"_token);
		static_assert("Number"_hash != "String"_token);
	}
}

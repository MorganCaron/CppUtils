#pragma once

#include <string_view>

namespace CppUtils
{
	using Hash = std::uint64_t;
	using Token = Hash;
	
	[[nodiscard]] constexpr auto hash(std::string_view string) noexcept -> Hash
	{
		auto result = 0xcbf29ce484222325u;
		for (const auto& c : string)
			result = (static_cast<Hash>(c) ^ result) * 0x100000001b3u;
		return result;
	}

	struct Hasher final
	{
		constexpr Hasher(Hash hash):
			hash{hash}
		{}

		constexpr Hasher(const char* cString):
			hash{CppUtils::hash(cString)}
		{}

		[[nodiscard]] constexpr auto operator==(Hash otherHash) const noexcept -> bool
		{
			return hash == otherHash;
		}

		const Hash hash;
	};

	namespace Hashing::Literals
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

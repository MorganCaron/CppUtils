#pragma once

#include <span>
#include <string>
#include <stdexcept>
#include <string_view>

#include <CppUtils/String/String.hpp>

namespace CppUtils::Hash
{
	using Hash = std::uint64_t;
	using Token = Hash;
	using TokenNames = std::unordered_map<Token, std::string>;
	
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
			hash{CppUtils::Hash::hash(cString)}
		{}

		[[nodiscard]] constexpr auto operator==(Hash otherHash) const noexcept -> bool
		{
			return hash == otherHash;
		}

		const Hash hash;
	};
	
	auto addTokenName(TokenNames& tokenNames, std::string newName) -> Token
	{
		auto token = hash(newName);
		tokenNames[token] = std::move(newName);
		return token;
	}

	[[nodiscard]] auto getTokenNameOrValue(Token token, const TokenNames& tokenNames) -> std::string
	{
		using namespace std::literals;

		if (tokenNames.contains(token))
		{
			if (const auto& name = tokenNames.at(token); !std::empty(name))
				return name;
			else
				return "\"\"";
		}
		if (token <= WCHAR_MAX)
		{
			auto c = static_cast<char>(token);
			if (std::isprint(static_cast<char>(token)))
				return std::string{std::to_string(token) + " '"s + c + '\''};
			for (const auto& [readableChar, escapedChar] : String::escapedChars)
				if (escapedChar == c)
					return std::to_string(token) + " '\\"s + readableChar + '\'';
		}
		return std::to_string(token);
	}

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
	}
}

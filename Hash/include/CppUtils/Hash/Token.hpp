#pragma once

#include <span>
#include <string>
#include <stdexcept>
#include <string_view>

#include <CppUtils/String/String.hpp>

namespace CppUtils::Hash
{
	using Token = std::uintptr_t;
	using TokenNames = std::unordered_map<Token, std::string>;
	
	[[nodiscard]] constexpr auto hash(String::Concept::StringView auto stringView) noexcept -> Token
	{
		auto result = 0xcbf29ce484222325u;
		for (const auto& c : stringView)
			result = (static_cast<Token>(c) ^ result) * 0x100000001b3u;
		return result;
	}

	template<class CharT>
	[[nodiscard]] constexpr auto hash(const std::basic_string<CharT>& string) noexcept -> Token
	{
		return hash(std::basic_string_view<CharT>{string});
	}

	auto addTokenName(TokenNames& tokenNames, std::string newName) -> Token
	{
		auto token = Hash::hash(newName);
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
		[[nodiscard]] constexpr auto operator"" _token(const char* cString, std::size_t size) noexcept -> Token
		{
			return hash(std::string_view{cString, size});
		}
	}
}

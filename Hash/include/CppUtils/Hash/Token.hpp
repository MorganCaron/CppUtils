#pragma once

#include <span>
#include <string>
#include <sstream>
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

	auto addTokenName(TokenNames& tokenNames, std::string newName) -> void
	{
		tokenNames[Hash::hash(newName)] = std::move(newName);
	}

	[[nodiscard]] auto getTokenNameOrValue(Token token, const TokenNames& tokenNames) -> std::string
	{
		using namespace std::literals;
		if (tokenNames.contains(token))
			return tokenNames.at(token);
		else if (token <= WCHAR_MAX)
		{
			auto c = static_cast<char>(token);
			if (std::isprint(static_cast<char>(token)))
				return std::string{"'"s + c + '\''};
			for (const auto& [readableChar, escapedChar] : String::escapedChars)
				if (escapedChar == c)
					return "'\\"s + readableChar + '\'';
		}
		auto os = std::ostringstream{};
		os << "0x" << std::hex << token;
		return os.str();
	}

	namespace Literals
	{
		[[nodiscard]] constexpr auto operator"" _token(const char* cString, std::size_t size) noexcept -> Token
		{
			return hash(std::string_view{cString, size});
		}
	}
}

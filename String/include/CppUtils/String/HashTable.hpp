#pragma once

#include <unordered_map>

#include <CppUtils/String/Hash.hpp>
#include <CppUtils/String/String.hpp>

namespace CppUtils::String
{
	using HashTable = std::unordered_map<String::Token, std::string>;

	inline auto addTokenName(HashTable& tokenNames, std::string newName) -> Token
	{
		auto token = hash(newName);
		tokenNames[token] = std::move(newName);
		return token;
	}

	[[nodiscard]] inline auto getTokenNameOrValue(Token token, const HashTable& tokenNames) -> std::string
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
}

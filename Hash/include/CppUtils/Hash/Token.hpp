#pragma once

#include <span>
#include <string>
#include <sstream>
#include <stdexcept>
#include <string_view>

namespace CppUtils::Hash
{
	using Token = std::uintptr_t;
	using TokenNames = std::unordered_map<Token, std::string>;

	template<class T = char>
	[[nodiscard]] constexpr auto hash(std::span<const T> span) noexcept -> Token
	{
		auto result = 0xcbf29ce484222325u;
		for (const auto& value : span)
			result = (static_cast<Token>(value) ^ result) * 0x100000001b3u;
		return result;
	}

	[[nodiscard]] constexpr auto hash(std::string_view string) noexcept -> Token
	{
		return hash<char>(string);
	}

	[[nodiscard]] inline auto getTokenNameOrValue(Token token, const TokenNames& tokenNames) -> std::string
	{
		if (tokenNames.contains(token))
			return tokenNames.at(token);
		else if (token <= WCHAR_MAX && std::isprint(static_cast<char>(token)))
			return std::string(1, static_cast<char>(token));
		else
		{
			auto os = std::ostringstream{};
			os << "0x" << std::hex << token;
			return os.str();
		}
	}

	class TokenException: public std::runtime_error
	{
	public:
		TokenException(std::string message, Token token):
			std::runtime_error{message},
			m_token{token}
		{}

		[[nodiscard]] auto getToken() const noexcept -> Token
		{
			return m_token;
		}

	private:
		Token m_token;
	};

	namespace Literals
	{
		[[nodiscard]] constexpr auto operator"" _token(const char* cstring, std::size_t) noexcept -> Token
		{
			return hash(cstring);
		}
	}
}

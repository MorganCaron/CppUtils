#pragma once

#include <string>
#include <iostream>
#include <typeindex>
#include <string_view>
#include <unordered_map>

#include <CppUtils/Hash/Hash.hpp>

namespace CppUtils::Type
{
	/// \brief The `Token` struct allows you to create identifiers from a character string.
	/// These identifiers can be used as key for ``std::map`` or ``std::unordered_map`` to have better performance than with ``std::string`` as key.
	struct Token final
	{
	public:
		using Index = std::size_t;

		constexpr Token():
			name{""},
			id{CppUtils::Hash::constexprHash("")}
		{};
		explicit constexpr Token(std::string_view c_name):
			name{c_name},
			id{CppUtils::Hash::constexprHash(name)}
		{}
		explicit Token(Index c_id):
			name{""},
			id{c_id}
		{
			if (m_typeNames.find(id) != m_typeNames.end())
				name = m_typeNames[id];
		}
		Token(const Token&) = default;
		Token(Token&&) noexcept = default;
		Token& operator=(const Token&) = default;
		Token& operator=(Token&&) noexcept = default;

		[[nodiscard]] inline constexpr bool operator==(const Token& rhs) const noexcept
		{
			return id == rhs.id;
		}
		[[nodiscard]] inline constexpr bool operator!=(const Token& rhs) const noexcept
		{
			return id != rhs.id;
		}

		[[nodiscard]] inline constexpr bool isEmpty() const noexcept
		{
			return id == CppUtils::Hash::constexprHash("");
		}

		struct hash_fn final
		{
			[[nodiscard]] inline constexpr std::size_t operator()(const Token& type) const noexcept
			{
				return type.id;
			}
		};

		inline void saveTypename()
		{
			if (name.empty() && id != CppUtils::Hash::constexprHash(""))
				return;
			if (m_typeNames.find(id) == m_typeNames.end())
				m_typeNames[id] = name;
			name = m_typeNames[id];
		}

		std::string_view name;
		Index id;

	private:
		static std::unordered_map<Index, std::string> m_typeNames;
	};

	inline std::ostream& operator<<(std::ostream& os, const Token& token) noexcept
	{
		os << token.name;
		return os;
	}

	namespace Literals
	{
		[[nodiscard]] constexpr Token operator"" _token(const char* cstring, std::size_t)
		{
			return Token{cstring};
		}
	}
}

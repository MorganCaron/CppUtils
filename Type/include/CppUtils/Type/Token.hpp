#pragma once

#include <string>
#include <typeindex>
#include <string_view>
#include <unordered_map>

#include <CppUtils/Hash/Hash.hpp>

namespace CppUtils::Type
{
	struct Token final
	{
	public:
		using Index = std::size_t;

		Token() = default;
		explicit constexpr Token(std::string_view c_name):
			name{c_name},
			id{CppUtils::Hash::constexprHash(name)}
		{}
		explicit constexpr Token(Index c_id):
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

	inline std::ostream& operator<<(std::ostream& os, const Token& token)
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

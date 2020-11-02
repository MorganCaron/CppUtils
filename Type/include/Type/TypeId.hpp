#pragma once

#include <string>
#include <typeindex>
#include <string_view>
#include <unordered_map>

#include <Hash/Hash.hpp>

namespace CppUtils::Type
{
	struct TypeId final
	{
	public:
		using Index = std::size_t;

		TypeId() = default;
		explicit constexpr TypeId(std::string_view c_name):
			name(c_name),
			id(CppUtils::Hash::constexprHash(name))
		{}
		TypeId(const TypeId&) = default;
		TypeId(TypeId&&) noexcept = default;
		TypeId& operator=(const TypeId&) = default;
		TypeId& operator=(TypeId&&) noexcept = default;

		[[nodiscard]] inline constexpr bool operator==(const TypeId& rhs) const noexcept
		{
			return id == rhs.id;
		}
		[[nodiscard]] inline constexpr bool operator!=(const TypeId& rhs) const noexcept
		{
			return id != rhs.id;
		}

		struct hash_fn
		{
			[[nodiscard]] inline constexpr std::size_t operator()(const TypeId &type) const noexcept
			{
				return type.id;
			}
		};

		inline void saveTypename()
		{
			if (m_typeNames.find(id) == m_typeNames.end())
				m_typeNames[id] = name;
			name = m_typeNames[id];
		}

		std::string_view name;
		Index id;

	private:
		static std::unordered_map<Index, std::string> m_typeNames;
	};

	inline std::ostream& operator<<(std::ostream& os, const TypeId& typeId)
	{
		os << typeId.name;
		return os;
	}

	namespace Literals
	{
		constexpr TypeId operator"" _typeId(const char* cstring, std::size_t)
		{
			return TypeId{cstring};
		}
	}
}

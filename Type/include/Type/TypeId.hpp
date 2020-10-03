#pragma once

#include <string>
#include <typeindex>
#include <string_view>
#include <unordered_map>

#include <Hash/Hash.hpp>

namespace CppUtils::Type
{
	using Index = std::size_t;
	
	template<typename T>
	class Type
	{
		const Index Id = std::hash<std::type_index>()(std::type_index(typeid(T)));
	};

	struct TypeId final
	{
		TypeId() = default;
		explicit constexpr TypeId(std::string_view c_name):
			name(c_name),
			id(CppUtils::Hash::constexprHash(name))
		{}
		TypeId(const TypeId&) = default;
		~TypeId() = default;
		TypeId& operator=(const TypeId&) = default;

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
}

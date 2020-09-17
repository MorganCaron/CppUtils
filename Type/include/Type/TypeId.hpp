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

		inline constexpr bool operator==(const TypeId& rhs) const noexcept
		{
			return id == rhs.id;
		}
		inline constexpr bool operator!=(const TypeId& rhs) const noexcept
		{
			return id != rhs.id;
		}

		std::string_view name;
		Index id;
	};

	class TypeIdStorage final
	{
	public:
		static inline void saveTypeId(TypeId& typeId)
		{
			if (m_typeIdNames.find(typeId.id) == m_typeIdNames.end())
				m_typeIdNames[typeId.id] = typeId.name;
			typeId.name = m_typeIdNames[typeId.id];
		}

	private:
		static std::unordered_map<Index, std::string> m_typeIdNames;
	};
}

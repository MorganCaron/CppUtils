#pragma once

#include <typeindex>

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
}

#pragma once

#include <string>
#include <iostream>
#include <string_view>

namespace CppUtils::Type
{
	class Named
	{
	public:
		Named() = default;
		explicit Named(std::string name): m_name{std::move(name)}
		{}

		inline void setName(std::string name)
		{
			m_name = std::move(name);
		}
		
		[[nodiscard]] inline std::string_view getName() const noexcept
		{
			return m_name;
		}

	private:
		std::string m_name;
	};
}

inline std::ostream& operator<<(std::ostream& os, const CppUtils::Type::Named& named) noexcept
{
	os << named.getName();
	return os;
}

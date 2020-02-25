#pragma once

#include <string>
#include <iostream>
#include <string_view>

namespace CppUtils::Class
{
	class Named
	{
	public:
		explicit Named(std::string name) : m_name{std::move(name)}
		{};

		inline void setName(std::string name)
		{
			m_name = std::move(name);
		}
		
		inline std::string_view getName() const noexcept
		{
			return m_name;
		}

	private:
		std::string m_name;
	};
}

std::ostream& operator<<(std::ostream& os, const CppUtils::Class::Named& named)
{
	os << named.getName();
	return os;
}

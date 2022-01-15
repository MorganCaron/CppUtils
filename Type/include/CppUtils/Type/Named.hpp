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

		void setName(std::string name)
		{
			m_name = std::move(name);
		}
		
		[[nodiscard]] std::string_view getName() const noexcept
		{
			return m_name;
		}

	private:
		std::string m_name;
	};

	inline std::ostream& operator<<(std::ostream& os, const Named& named) noexcept
	{
		os << named.getName();
		return os;
	}
}

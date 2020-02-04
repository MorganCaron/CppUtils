#pragma once

#include <memory>

namespace CppUtils::Class
{
	class ICloneable
	{
	public:
		virtual ~ICloneable() = default;
		virtual std::unique_ptr<ICloneable> clone() const = 0;
	};

	template<class Derived>
	class Cloneable : public virtual ICloneable
	{
	public:
		virtual ~Cloneable() = default;

		std::unique_ptr<ICloneable> clone() const override
		{
			return std::make_unique<Derived>(*this);
		}
	};
}

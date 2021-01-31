#pragma once

#include <memory>

namespace CppUtils::Type
{
	class ICloneable
	{
	public:
		virtual ~ICloneable() = default;
		[[nodiscard]] virtual std::unique_ptr<ICloneable> clone() const = 0;
	};

	template<typename Derived>
	class Cloneable: public virtual ICloneable
	{
	public:
		virtual ~Cloneable() = default;

		[[nodiscard]] std::unique_ptr<ICloneable> clone() const override
		{
			return std::make_unique<Derived>(*this);
		}
	};
}

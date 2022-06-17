#pragma once

namespace CppUtils::Type
{
	template<class Derived>
	class NonCopyable
	{
	public:
		NonCopyable(const NonCopyable&) = delete;
		Derived& operator=(const Derived&) = delete;

	protected:
		NonCopyable() = default;
		~NonCopyable() = default;
	};
}

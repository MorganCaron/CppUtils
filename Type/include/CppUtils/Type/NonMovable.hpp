#pragma once

namespace CppUtils::Type
{
	class NonMovable
	{
	public:
		NonMovable(NonMovable&&) = delete;
		NonMovable& operator=(NonMovable&&) = delete;

	protected:
		NonMovable() = default;
		~NonMovable() = default;
	};
}

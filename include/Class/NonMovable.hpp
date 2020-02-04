#pragma once

namespace CppUtils::Class
{
	class NonMovable
	{
	public:
		NonMovable(NonMovable&&) = delete;
		NonMovable& operator=(NonMovable&&) = delete;
	};
}

#pragma once

namespace CppUtils::Class
{
	template<class T>
	class NonCopyable
	{
	public:
		NonCopyable(const NonCopyable&) = delete;
		T& operator=(const T&) = delete;

	protected:
		NonCopyable() = default;
		~NonCopyable() = default;
	};
}

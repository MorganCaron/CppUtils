#pragma once

namespace CppUtils::Class
{
	class NonCopyable
	{
	public:
		NonCopyable(const NonCopyable&) = delete;
		NonCopyable& operator=(const NonCopyable&) = delete;
	};
}

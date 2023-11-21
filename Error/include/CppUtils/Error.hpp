#pragma once

#include <exception>

namespace CppUtils
{
	class ErrorHandler final
	{
	public:
		explicit ErrorHandler(std::terminate_handler handler) noexcept:
			m_lastHandler{std::set_terminate(handler)}
		{}

		~ErrorHandler() noexcept
		{
			std::set_terminate(m_lastHandler);
		}

	private:
		std::terminate_handler m_lastHandler;
	};
}

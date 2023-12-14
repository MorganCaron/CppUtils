#pragma once

#include <exception>

/*
#include <expected>

#define TRY(expected) \
	({ \
		auto&& result = expected; \
		if (!result.has_value()) [[unlikely]] \
			return std::unexpected{result.error()}; \
		result.value(); \
	})
*/

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

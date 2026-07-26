#pragma once

#include <expected>

#define UNWRAP_OR_RETURN(variable, expression) \
	auto result = (expression); \
	if (not result) \
		return; \
	auto& variable = *result

#define UNWRAP_OR_UNEXPECTED(variable, expression) \
	auto result = (expression); \
	if (not result) \
		return std::unexpected(result.error()); \
	auto& variable = *result

#if defined(__GNUC__) or defined(__clang__)
#	define TRY(expected) \
		({ \
			auto&& result = expected; \
			if (not result.has_value()) [[unlikely]] \
				return std::unexpected{result.error()}; \
			result.value(); \
		})

#	define TRY_VOID(expected) \
		({ \
			auto&& result = expected; \
			if (not result.has_value()) [[unlikely]] \
				return; \
			result.value(); \
		})

#	define TRY_OR_RETURN(expected) TRY_VOID(expected)
#endif

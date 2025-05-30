#pragma once

#include <expected>

#define TRY(expected) \
	({ \
		auto&& result = expected; \
		if (not result.has_value()) [[unlikely]] \
			return std::unexpected{result.error()}; \
		result.value(); \
	})

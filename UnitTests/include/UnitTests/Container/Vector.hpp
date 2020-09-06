#pragma once

#include <CppUtils.hpp>

namespace UnitTests::Container::Vector
{
	const auto tests = std::vector<CppUtils::Test::UnitTest>{

		CppUtils::Test::UnitTest("Container/Vector/merge", [] {
			const auto a = std::vector<std::string>{"a", "b"};
			const auto b = std::vector<std::string>{"c", "d"};
			const auto c = std::vector<std::string>{"e", "f"};
			const auto d = std::vector<std::string>{"g", "h"};

			const auto vector = CppUtils::Container::Vector::merge({std::cref(a), std::cref(b), std::cref(c), std::cref(d)});
			ASSERT(vector.size() == 8);
			for (const auto& string : vector)
				CppUtils::Logger::logInformation(string, false);
			CppUtils::Logger::logInformation("");
		})

	};
}

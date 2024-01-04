#pragma once

#include <CppUtils.hpp>

namespace CppUtils::UnitTest::Container::VariantVector
{
	auto _ = TestSuite{"Container/VariantVector", [](auto& suite) {
		using namespace std::literals;
		using Logger = CppUtils::Logger<"CppUtils">;

		suite.addTest("Empty VariantVector", [&] {
			auto variantVector = CppUtils::Container::VariantVector<int>{};
			suite.expect(std::empty(variantVector));
		});

		suite.addTest("One value", [&] {
			auto variantVector = CppUtils::Container::VariantVector<int>{42};
			suite.expectEqual(std::size(variantVector), 1uz);
		});

		suite.addTest("One type, multiple values", [&] {
			auto variantVector = CppUtils::Container::VariantVector<int>{1, 2, 3};
			suite.expectEqual(std::size(variantVector), 3uz);
		});

		suite.addTest("Multiple types", [&] {
			auto variantVector = CppUtils::Container::VariantVector<bool, int, float, std::string_view>{true, 42, 3.14f, "Hello World!"sv};
			
			suite.expectEqual(std::size(variantVector), 4uz);
			
			Logger::print("{}\n", variantVector.get<bool>(0));
			Logger::print("{}\n", variantVector.get<int>(1));
			Logger::print("{}\n", variantVector.get<float>(2));
			Logger::print("{}\n", variantVector.get<std::string_view>(3));
		});

		suite.addTest("pushBack", [&] {
			auto variantVector = CppUtils::Container::VariantVector<int>{};
			
			suite.expect(std::empty(variantVector));
			suite.expectEqual(std::size(variantVector), 0uz);

			variantVector.pushBack(42);

			suite.expect(!std::empty(variantVector));
			suite.expectEqual(std::size(variantVector), 1uz);
		});

		suite.addTest("get", [&] {
			auto variantVector = CppUtils::Container::VariantVector<int>{42};
			
			suite.expect(!std::empty(variantVector));
			suite.expectEqual(std::size(variantVector), 1uz);
			
			suite.expectEqual(variantVector.get<int>(0), 42);

			suite.expect(!std::empty(variantVector));
			suite.expectEqual(std::size(variantVector), 1uz);
		});

		suite.addTest("pop", [&] {
			auto variantVector = CppUtils::Container::VariantVector<int>{42};

			suite.expect(!std::empty(variantVector));
			suite.expectEqual(std::size(variantVector), 1uz);

			suite.expectEqual(variantVector.pop<int>(), 42);

			suite.expect(std::empty(variantVector));
			suite.expectEqual(std::size(variantVector), 0uz);
		});

	}};
}

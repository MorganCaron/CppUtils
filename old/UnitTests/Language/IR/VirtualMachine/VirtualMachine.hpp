#pragma once

#include <CppUtils.hpp>

namespace CppUtils::UnitTests::Language::IR::VirtualMachine
{
	TEST_GROUP("Language/IR/VirtualMachine")
	{
		using namespace std::literals;
		using namespace CppUtils::Type::Literals;

		addTest("Operations", [] {
			const auto virtualMachine = CppUtils::Language::IR::VirtualMachine::VirtualMachine<1'000>{};
			auto context = CppUtils::Language::IR::VirtualMachine::Context<1'000>{};
			auto result = virtualMachine.run("main"_token, R"(
				int main()
				{
					nop;
					a = (20 + 24) - 2;
					return a;
				}
				)"sv,
				context);
			CppUtils::Log::Logger::logInformation(std::to_string(result));
			EXPECT(result == 42);
		});

		/*addTest("Pointers", [] {
			const auto virtualMachine = CppUtils::Language::IR::VirtualMachine::VirtualMachine<std::uint64_t, 1000>{};
			auto context = CppUtils::Language::IR::VirtualMachine::Context<std::uint64_t, 1000>{};
			auto result = virtualMachine.run("main"_token, R"(
			address append(output, c)
			{
				*output = c;
				output += 1;
				*output = 0;
				return output;
			}

			int main()
			{
				return append("hello", '!');
			}
			)"sv, context);
			CppUtils::Log::Logger::logInformation(std::to_string(result));
			EXPECT(result == 42);
		});*/

		/*addTest("Conditions", [] {
			const auto virtualMachine = CppUtils::Language::IR::VirtualMachine::VirtualMachine<std::uint64_t, 1000>{};
			auto context = CppUtils::Language::IR::VirtualMachine::Context<std::uint64_t, 1000>{};
			auto result = virtualMachine.run("main"_token, R"(
			int getLength(text)
			{
				length = 0;
				while ((*text == 0) == 0)
				{
					length += 1;
					text += 1;
				}
				return length;
			}

			int main()
			{
				text = "Hello World!";
				return getLength(text);
			}
			)"sv, context);
			CppUtils::Log::Logger::logInformation(std::to_string(result));
			EXPECT(result == 12);
		});
	}
}


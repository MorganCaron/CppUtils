#include <iostream>
#include <vector>

#include "CppUtils.hpp"

int main()
{
	const auto tests = std::vector<CppUtils::Test>{

		CppUtils::Test("Unit tests", [] {
			ASSERT(true);
		}),

		CppUtils::Test("WebNode", [] {
			using StringWebNode = CppUtils::WebNode<std::string, std::string>;
			auto fruit = std::make_shared<StringWebNode>("Fruit");
			auto orange = std::make_shared<StringWebNode>("");
			auto color = std::make_shared<StringWebNode>("Orange");
			orange->attach("Class", fruit);
			orange->attach("Color", color);
			ASSERT(orange->get("Class")[0]->value == "Fruit");
			ASSERT(orange->get("Color")[0]->value == "Orange");
		})

	};

	return CppUtils::Test::executeTests(tests);
}

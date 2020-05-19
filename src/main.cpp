#include <iostream>
#include <vector>

#include <CppUtils.hpp>

int main()
{
	const auto tests = std::vector<CppUtils::Test::UnitTest>{

		CppUtils::Test::UnitTest("Unit tests", [] {
			ASSERT(true);
		}),

		CppUtils::Test::UnitTest("Logger", [] {
			CppUtils::Logger::logDebug("Message de debug");
			ASSERT(true);
		}),

		CppUtils::Test::UnitTest("MeshNode", [] {
			using StringMeshNode = CppUtils::Container::MeshNode<std::string, std::string>;
			auto fruit = std::make_shared<StringMeshNode>("fruit");
			auto banana = std::make_shared<StringMeshNode>("banana");
			auto lemon = std::make_shared<StringMeshNode>("lemon");
			auto color = std::make_shared<StringMeshNode>("color");
			auto yellow = std::make_shared<StringMeshNode>("yellow");
			auto orangeFruit = std::make_shared<StringMeshNode>("orange");
			auto orangeColor = std::make_shared<StringMeshNode>("orange");

			StringMeshNode::attach("Colors", yellow, "Elements", banana);
			StringMeshNode::attach("Colors", yellow, "Elements", lemon);
			StringMeshNode::attach("Colors", orangeColor, "Elements", orangeFruit);
			StringMeshNode::attach("Categories", color, "Elements", yellow);
			StringMeshNode::attach("Categories", fruit, "Elements", banana);
			StringMeshNode::attach("Categories", fruit, "Elements", lemon);
			StringMeshNode::attach("Categories", color, "Elements", orangeColor);
			StringMeshNode::attach("Categories", fruit, "Elements", orangeFruit);

			ASSERT(fruit->value == "fruit");
			const auto fruitName = fruit->get("Elements")[0];
			ASSERT(fruitName->value == "banana");
			const auto fruitColor = fruitName->get("Colors")[0];
			ASSERT(fruitColor->value == "yellow");
			
			for (const auto& aFruit : fruit->get("Elements"))
				CppUtils::Logger::logInformation(aFruit->value + " is a " + aFruit->get("Colors")[0]->value + " " + aFruit->get("Categories")[0]->value);
		})

	};

	return CppUtils::Test::UnitTest::executeTests(tests);
}

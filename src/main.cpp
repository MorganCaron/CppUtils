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

			StringMeshNode::assignTo("Colors", yellow, "Elements", banana);
			StringMeshNode::assignTo("Colors", yellow, "Elements", lemon);
			StringMeshNode::assignTo("Colors", orangeColor, "Elements", orangeFruit);
			StringMeshNode::assignCategoryToElement(color, yellow);
			StringMeshNode::assignCategoryToElement(fruit, banana);
			StringMeshNode::assignCategoryToElement(fruit, lemon);
			StringMeshNode::assignCategoryToElement(color, orangeColor);
			StringMeshNode::assignCategoryToElement(fruit, orangeFruit);

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

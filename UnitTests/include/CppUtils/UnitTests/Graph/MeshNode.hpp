#pragma once

#include <CppUtils.hpp>

namespace CppUtils::UnitTest::Graph::MeshNode
{
	auto _ = TestSuite{"Graph/MeshNode", [](auto& suite) {
		using Logger = CppUtils::Logger<"CppUtils">;
		using StringMeshNetwork = CppUtils::Graph::MeshNetwork<std::string, std::string>;
		using Node = StringMeshNetwork::Node;

		suite.addTest("One", [&] {
			auto meshNetwork = StringMeshNetwork{};
			auto banana = std::shared_ptr<Node>{meshNetwork.newNode("banana")};
			Logger::print("{}\n", banana->value);
			suite.expect(banana->value == "banana");
		});

		suite.addTest("Two", [&] {
			auto meshNetwork = StringMeshNetwork{};
			auto fruit = std::shared_ptr<Node>{meshNetwork.newNode("fruit")};
			auto banana = std::shared_ptr<Node>{meshNetwork.newNode("banana")};
			Node::attach("Categories", fruit, "Elements", banana);

			suite.expect(fruit->value == "fruit");
			const auto fruitName = std::shared_ptr<Node>{fruit->get("Elements")[0]};
			suite.expect(fruitName->value == "banana");

			for (const auto& aFruit : fruit->get("Elements"))
			{
				auto sharedFruit = std::shared_ptr<Node>{aFruit};
				Logger::print("{} is a {}\n", sharedFruit->value, std::shared_ptr<Node> { sharedFruit->get("Categories")[0] } -> value);
			}
		});

		suite.addTest("Many", [&] {
			auto meshNetwork = StringMeshNetwork{};
			auto fruit = std::shared_ptr<Node>{meshNetwork.newNode("fruit")};
			auto banana = std::shared_ptr<Node>{meshNetwork.newNode("banana")};
			auto lemon = std::shared_ptr<Node>{meshNetwork.newNode("lemon")};
			auto color = std::shared_ptr<Node>{meshNetwork.newNode("color")};
			auto yellow = std::shared_ptr<Node>{meshNetwork.newNode("yellow")};
			auto orangeFruit = std::shared_ptr<Node>{meshNetwork.newNode("orange")};
			auto orangeColor = std::shared_ptr<Node>{meshNetwork.newNode("orange")};

			Node::attach("Colors", yellow, "Elements", banana);
			Node::attach("Colors", yellow, "Elements", lemon);
			Node::attach("Colors", orangeColor, "Elements", orangeFruit);
			Node::attach("Categories", color, "Elements", yellow);
			Node::attach("Categories", fruit, "Elements", banana);
			Node::attach("Categories", fruit, "Elements", lemon);
			Node::attach("Categories", color, "Elements", orangeColor);
			Node::attach("Categories", fruit, "Elements", orangeFruit);

			suite.expect(fruit->value == "fruit");
			const auto fruitName = std::shared_ptr<Node>{fruit->get("Elements")[0]};
			suite.expect(fruitName->value == "banana");
			const auto fruitColor = std::shared_ptr<Node>{fruitName->get("Colors")[0]};
			suite.expect(fruitColor->value == "yellow");

			for (const auto& aFruit : fruit->get("Elements"))
			{
				auto sharedFruit = std::shared_ptr<Node>{aFruit};

				Logger::print("{} is a {} {}\n", sharedFruit->value, std::shared_ptr<Node> { sharedFruit->get("Colors")[0] } -> value, std::shared_ptr<Node> { sharedFruit->get("Categories")[0] } -> value);
			}
		});
	}};
}

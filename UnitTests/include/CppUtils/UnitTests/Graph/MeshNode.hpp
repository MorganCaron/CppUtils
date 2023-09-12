#pragma once

#include <CppUtils.hpp>

namespace CppUtils::UnitTests::Graph::MeshNode
{
	TEST_GROUP("Graph/MeshNode")
	{
		using StringMeshNetwork = CppUtils::Graph::MeshNetwork<std::string, std::string>;
		using Node = StringMeshNetwork::Node;

		addTest("One", [] {
			auto meshNetwork = StringMeshNetwork{};
			auto banana = std::shared_ptr<Node>{meshNetwork.newNode("banana")};
			CppUtils::Log::Logger{std::cout} << banana->value << '\n';
			EXPECT(banana->value == "banana");
		});

		addTest("Two", [] {
			auto meshNetwork = StringMeshNetwork{};
			auto fruit = std::shared_ptr<Node>{meshNetwork.newNode("fruit")};
			auto banana = std::shared_ptr<Node>{meshNetwork.newNode("banana")};
			Node::attach("Categories", fruit, "Elements", banana);

			EXPECT(fruit->value == "fruit");
			const auto fruitName = std::shared_ptr<Node>{fruit->get("Elements")[0]};
			EXPECT(fruitName->value == "banana");

			for (const auto& aFruit : fruit->get("Elements"))
			{
				auto sharedFruit = std::shared_ptr<Node>{aFruit};
				CppUtils::Log::Logger{std::cout}
					<< sharedFruit->value << " is a " << std::shared_ptr<Node>{sharedFruit->get("Categories")[0]}->value << '\n';
			}
		});

		addTest("Many", [] {
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

			EXPECT(fruit->value == "fruit");
			const auto fruitName = std::shared_ptr<Node>{fruit->get("Elements")[0]};
			EXPECT(fruitName->value == "banana");
			const auto fruitColor = std::shared_ptr<Node>{fruitName->get("Colors")[0]};
			EXPECT(fruitColor->value == "yellow");
			
			for (const auto& aFruit : fruit->get("Elements"))
			{
				auto sharedFruit = std::shared_ptr<Node>{aFruit};
				CppUtils::Log::Logger{std::cout}
					<< sharedFruit->value << " is a " << std::shared_ptr<Node>{sharedFruit->get("Colors")[0]}->value << " " << std::shared_ptr<Node>{sharedFruit->get("Categories")[0]}->value << '\n';
			}
		});
	}
}

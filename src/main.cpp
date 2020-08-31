#include <iostream>
#include <vector>

#include <CppUtils.hpp>

int main()
{
	const auto tests = std::vector<CppUtils::Test::UnitTest>{

		CppUtils::Test::UnitTest("Container/MeshNode/One", [] {
			using StringMeshNode = CppUtils::Container::MeshNode<std::string, std::string>;
			auto banana = std::make_shared<StringMeshNode>("banana");
			CppUtils::Logger::logInformation(banana->value);
			ASSERT(banana->value == "banana");
		}),

		CppUtils::Test::UnitTest("Container/MeshNode/Two", [] {
			using StringMeshNode = CppUtils::Container::MeshNode<std::string, std::string>;
			auto fruit = std::make_shared<StringMeshNode>("fruit");
			auto banana = std::make_shared<StringMeshNode>("banana");
			StringMeshNode::attach("Categories", fruit, "Elements", banana);

			ASSERT(fruit->value == "fruit");
			const auto fruitName = fruit->get("Elements")[0];
			ASSERT(fruitName->value == "banana");

			for (const auto& aFruit : fruit->get("Elements"))
				CppUtils::Logger::logInformation(aFruit->value + " is a " + aFruit->get("Categories")[0]->value);
		}),

		CppUtils::Test::UnitTest("Container/MeshNode/Many", [] {
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
		}),

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
		}),

		CppUtils::Test::UnitTest("FileSystem/File/String/WriteRead", [] {
			const auto filePath = std::filesystem::path{"test.tmp"};
			const auto originalString = "Hello world!";
			CppUtils::FileSystem::File::String::write(filePath, originalString);
			const auto fileContent = CppUtils::FileSystem::File::String::read(filePath);
			CppUtils::Logger::logInformation(fileContent);
			CppUtils::FileSystem::File::deleteFile(filePath);
			ASSERT(fileContent == originalString);
		}),

		CppUtils::Test::UnitTest("FileSystem/File/String/Append", [] {
			const auto filePath = std::filesystem::path{"test.tmp"};
			const auto firstString = "Hello ";
			const auto secondString = "world!";
			CppUtils::FileSystem::File::String::append(filePath, firstString);
			CppUtils::FileSystem::File::String::append(filePath, secondString);
			const auto fileContent = CppUtils::FileSystem::File::String::read(filePath);
			CppUtils::Logger::logInformation(fileContent);
			CppUtils::FileSystem::File::deleteFile(filePath);
			ASSERT(fileContent == std::string{firstString} + secondString);
		}),

		CppUtils::Test::UnitTest("FileSystem/File/Binary/WriteRead", [] {
			const auto filePath = std::filesystem::path{"test.tmp"};
			const auto originalString = 12345;
			CppUtils::FileSystem::File::Binary::write(filePath, originalString);
			const auto fileContent = CppUtils::FileSystem::File::Binary::read<decltype(originalString)>(filePath);
			CppUtils::Logger::logInformation(std::to_string(fileContent));
			CppUtils::FileSystem::File::deleteFile(filePath);
			ASSERT(fileContent == originalString);
		}),

		CppUtils::Test::UnitTest("Log/Logger", [] {
			CppUtils::Logger::logInformation("Information message");
			CppUtils::Logger::logImportant("Important message");
			CppUtils::Logger::logSuccess("Success message");
			CppUtils::Logger::logDebug("Debug message");
			CppUtils::Logger::logWarning("Warning message");
			CppUtils::Logger::logError("Error message");
			ASSERT(true);
		}),

		CppUtils::Test::UnitTest("Test/UnitTest", [] {
			ASSERT(true);
		}),

		CppUtils::Test::UnitTest("Type/Index", [] {
			const auto numberType0 = CppUtils::Type::TypeId{"Number"};
			const auto numberType1 = CppUtils::Type::TypeId{"Number"};
			const auto stringType = CppUtils::Type::TypeId{"String"};
			ASSERT(numberType0 == numberType1);
			ASSERT(numberType0 != stringType);
		})

	};

	return CppUtils::Test::UnitTest::executeTests(tests);
}

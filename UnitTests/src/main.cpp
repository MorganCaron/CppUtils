#include <UnitTests/Chrono/Chronometer.hpp>
#include <UnitTests/Container/Vector.hpp>
#include <UnitTests/FileSystem/File/Binary.hpp>
#include <UnitTests/FileSystem/File/String.hpp>
#include <UnitTests/Functional/Function.hpp>
#include <UnitTests/Graph/MeshNode.hpp>
#include <UnitTests/Graph/TreeNode.hpp>
#include <UnitTests/Log/ChronoLogger.hpp>
#include <UnitTests/Log/Logger.hpp>
#include <UnitTests/Parser/Cursor.hpp>
#include <UnitTests/String/String.hpp>
#include <UnitTests/Switch/Switch.hpp>
#include <UnitTests/Terminal/Parameters.hpp>
#include <UnitTests/Test/UnitTest.hpp>
#include <UnitTests/Type/Typed.hpp>
#include <UnitTests/Type/TypeId.hpp>

int main()
{
	const auto tests = CppUtils::Container::Vector::merge({
		std::cref(UnitTests::Chrono::Chronometer::tests),
		std::cref(UnitTests::Container::Vector::tests),
		std::cref(UnitTests::FileSystem::File::Binary::tests),
		std::cref(UnitTests::FileSystem::File::String::tests),
		std::cref(UnitTests::Functional::Function::tests),
		std::cref(UnitTests::Graph::MeshNode::tests),
		std::cref(UnitTests::Graph::TreeNode::tests),
		std::cref(UnitTests::Log::ChronoLogger::tests),
		std::cref(UnitTests::Log::Logger::tests),
		std::cref(UnitTests::Parser::Cursor::tests),
		std::cref(UnitTests::String::tests),
		std::cref(UnitTests::Switch::tests),
		std::cref(UnitTests::Terminal::Parameters::tests),
		std::cref(UnitTests::Test::UnitTest::tests),
		std::cref(UnitTests::Type::Typed::tests),
		std::cref(UnitTests::Type::TypeId::tests)
	});

	return CppUtils::Test::UnitTest::executeTests(tests);
}

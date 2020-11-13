#include <CppUtils/UnitTests/Chrono/Chronometer.hpp>
#include <CppUtils/UnitTests/Container/Vector.hpp>
#include <CppUtils/UnitTests/FileSystem/File/Binary.hpp>
#include <CppUtils/UnitTests/FileSystem/File/String.hpp>
#include <CppUtils/UnitTests/Functional/Function.hpp>
#include <CppUtils/UnitTests/Graph/MeshNode.hpp>
#include <CppUtils/UnitTests/Graph/TreeNode.hpp>
#include <CppUtils/UnitTests/Language/Grammar.hpp>
#include <CppUtils/UnitTests/Language/Lexer.hpp>
#include <CppUtils/UnitTests/Language/Parameters.hpp>
#include <CppUtils/UnitTests/Language/Parser.hpp>
#include <CppUtils/UnitTests/Log/ChronoLogger.hpp>
#include <CppUtils/UnitTests/Log/Logger.hpp>
#include <CppUtils/UnitTests/String/String.hpp>
#include <CppUtils/UnitTests/Switch/Switch.hpp>
#include <CppUtils/UnitTests/Test/UnitTest.hpp>
#include <CppUtils/UnitTests/Type/Typed.hpp>
#include <CppUtils/UnitTests/Type/TypeId.hpp>

int main()
{
	const auto tests = CppUtils::Container::Vector::merge({
		std::cref(CppUtils::UnitTests::Chrono::Chronometer::tests),
		std::cref(CppUtils::UnitTests::Container::Vector::tests),
		std::cref(CppUtils::UnitTests::FileSystem::File::Binary::tests),
		std::cref(CppUtils::UnitTests::FileSystem::File::String::tests),
		std::cref(CppUtils::UnitTests::Functional::Function::tests),
		std::cref(CppUtils::UnitTests::Graph::MeshNode::tests),
		std::cref(CppUtils::UnitTests::Graph::TreeNode::tests),
		std::cref(CppUtils::UnitTests::Language::Grammar::tests),
		std::cref(CppUtils::UnitTests::Language::Lexer::tests),
		std::cref(CppUtils::UnitTests::Language::Parameters::tests),
		std::cref(CppUtils::UnitTests::Language::Parser::tests),
		std::cref(CppUtils::UnitTests::Log::ChronoLogger::tests),
		std::cref(CppUtils::UnitTests::Log::Logger::tests),
		std::cref(CppUtils::UnitTests::String::tests),
		std::cref(CppUtils::UnitTests::Switch::tests),
		std::cref(CppUtils::UnitTests::Test::UnitTest::tests),
		std::cref(CppUtils::UnitTests::Type::Typed::tests),
		std::cref(CppUtils::UnitTests::Type::TypeId::tests)
	});

	return CppUtils::Test::UnitTest::executeTests(tests);
}

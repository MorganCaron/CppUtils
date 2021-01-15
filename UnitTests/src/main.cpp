#include <CppUtils/UnitTests/ASM/Lexer.hpp>
#include <CppUtils/UnitTests/ASM/VirtualMachine.hpp>
#include <CppUtils/UnitTests/Chrono/Chronometer.hpp>
#include <CppUtils/UnitTests/Container/Vector.hpp>
#include <CppUtils/UnitTests/FileSystem/File/Binary.hpp>
#include <CppUtils/UnitTests/FileSystem/File/String.hpp>
#include <CppUtils/UnitTests/Functional/Function.hpp>
#include <CppUtils/UnitTests/Graph/MeshNode.hpp>
#include <CppUtils/UnitTests/Graph/TreeNode.hpp>
#include <CppUtils/UnitTests/Language/IR/RegisterIRLexer.hpp>
#include <CppUtils/UnitTests/Language/IR/StackIRLexer.hpp>
#include <CppUtils/UnitTests/Language/Lexer/GrammarLexer.hpp>
#include <CppUtils/UnitTests/Language/Lexer/Lexer.hpp>
#include <CppUtils/UnitTests/Language/Parser/Cursor.hpp>
#include <CppUtils/UnitTests/Ini/IniLexer.hpp>
#include <CppUtils/UnitTests/Json/JsonLexer.hpp>
#include <CppUtils/UnitTests/Parameters/ParametersLexer.hpp>
#include <CppUtils/UnitTests/StringTree/StringTreeLexer.hpp>
#include <CppUtils/UnitTests/Log/ChronoLogger.hpp>
#include <CppUtils/UnitTests/Log/Logger.hpp>
#include <CppUtils/UnitTests/String/String.hpp>
#include <CppUtils/UnitTests/Switch/Switch.hpp>
#include <CppUtils/UnitTests/Test/Test.hpp>
#include <CppUtils/UnitTests/Type/Typed.hpp>
#include <CppUtils/UnitTests/Type/Token.hpp>

int main(const int argc, const char *argv[])
{
	try
	{
		const auto tests = CppUtils::Container::Vector::merge({
			std::cref(CppUtils::UnitTests::ASM::Lexer::tests),
			std::cref(CppUtils::UnitTests::ASM::VirtualMachine::tests),
			std::cref(CppUtils::UnitTests::Chrono::Chronometer::tests),
			std::cref(CppUtils::UnitTests::Container::Vector::tests),
			std::cref(CppUtils::UnitTests::FileSystem::File::Binary::tests),
			std::cref(CppUtils::UnitTests::FileSystem::File::String::tests),
			std::cref(CppUtils::UnitTests::Functional::Function::tests),
			std::cref(CppUtils::UnitTests::Graph::MeshNode::tests),
			std::cref(CppUtils::UnitTests::Graph::TreeNode::tests),
			std::cref(CppUtils::UnitTests::Ini::IniLexer::tests),
			std::cref(CppUtils::UnitTests::Json::JsonLexer::tests),
			std::cref(CppUtils::UnitTests::Language::IR::RegisterIRLexer::tests),
			std::cref(CppUtils::UnitTests::Language::IR::StackIRLexer::tests),
			std::cref(CppUtils::UnitTests::Language::Lexer::GrammarLexer::tests),
			std::cref(CppUtils::UnitTests::Language::Lexer::tests),
			std::cref(CppUtils::UnitTests::Language::Parser::Cursor::tests),
			std::cref(CppUtils::UnitTests::Parameters::ParametersLexer::tests),
			std::cref(CppUtils::UnitTests::StringTree::StringTreeLexer::tests),
			std::cref(CppUtils::UnitTests::Log::ChronoLogger::tests),
			std::cref(CppUtils::UnitTests::Log::Logger::tests),
			std::cref(CppUtils::UnitTests::String::tests),
			std::cref(CppUtils::UnitTests::Switch::tests),
			std::cref(CppUtils::UnitTests::Test::tests),
			std::cref(CppUtils::UnitTests::Type::Typed::tests),
			std::cref(CppUtils::UnitTests::Type::Token::tests)
		});

		const auto settings = CppUtils::Test::executeCommands(argc, argv);
		if (settings.abort)
			return EXIT_SUCCESS;
		return CppUtils::Test::executeTests(std::move(tests), std::move(settings));
	}
	catch (const std::exception& exception)
	{
		using namespace std::literals;
		CppUtils::Log::Logger::logError("An exception occurred:\n"s + exception.what());
		return EXIT_FAILURE;
	}
	return EXIT_SUCCESS;
}

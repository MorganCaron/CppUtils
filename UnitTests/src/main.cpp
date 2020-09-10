#include <UnitTests/Container/MeshNode.hpp>
#include <UnitTests/Container/Vector.hpp>
#include <UnitTests/FileSystem/File/Binary.hpp>
#include <UnitTests/FileSystem/File/String.hpp>
#include <UnitTests/Log/Logger.hpp>
#include <UnitTests/String/String.hpp>
#include <UnitTests/Terminal/Parameters.hpp>
#include <UnitTests/Test/UnitTest.hpp>
#include <UnitTests/Type/Index.hpp>

int main()
{
	const auto tests = CppUtils::Container::Vector::merge({
		std::cref(UnitTests::Container::MeshNode::tests),
		std::cref(UnitTests::Container::Vector::tests),
		std::cref(UnitTests::FileSystem::File::Binary::tests),
		std::cref(UnitTests::FileSystem::File::String::tests),
		std::cref(UnitTests::Log::Logger::tests),
		std::cref(UnitTests::String::tests),
		std::cref(UnitTests::Terminal::Parameters::tests),
		std::cref(UnitTests::Test::UnitTest::tests),
		std::cref(UnitTests::Type::Index::tests)
	});

	return CppUtils::Test::UnitTest::executeTests(tests);
}

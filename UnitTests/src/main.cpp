#include <CppUtils/UnitTests/UnitTests.hpp>

// Todo: std::span<const std::string_view> args
int main([[maybe_unused]] const int argc, [[maybe_unused]] const char* argv[])
{
	try
	{
		CppUtils::Terminal::setConsoleOutputUTF8();
		/*
		auto& settings = CppUtils::UnitTest::executeCommands(argc, argv);
		if (settings.abort)
			return EXIT_SUCCESS;
		*/
		auto settings = CppUtils::UnitTest::TestSettings{};
		return CppUtils::UnitTest::executeTests(std::move(settings));
	}
	catch (const std::exception& exception)
	{
		CppUtils::logException(exception);
		return EXIT_FAILURE;
	}
	return EXIT_SUCCESS;
}

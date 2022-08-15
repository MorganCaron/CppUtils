#include <CppUtils/UnitTests/UnitTests.hpp>

int main(const int argc, const char *argv[])
{
	try
	{
		CppUtils::Terminal::setConsoleOutputUTF8();
		auto& settings = CppUtils::UnitTest::executeCommands(argc, argv);
		if (settings.abort)
			return EXIT_SUCCESS;
		return CppUtils::UnitTest::executeTests(std::move(settings));
	}
	catch (const std::exception& exception)
	{
		CppUtils::Log::logException(exception);
		return EXIT_FAILURE;
	}
	return EXIT_SUCCESS;
}

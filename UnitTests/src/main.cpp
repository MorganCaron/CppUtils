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
		using namespace std::literals;
		CppUtils::Log::Logger::logError("An exception occurred:\n"s + exception.what());
		return EXIT_FAILURE;
	}
	return EXIT_SUCCESS;
}

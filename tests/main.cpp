import std;
import CppUtils;
import CppUtils.UnitTests;

constexpr auto exitSuccess = 0;
constexpr auto exitFailure = 1;

auto start([[maybe_unused]] std::span<const std::string_view> args) -> int
{
	try
	{
		/*
		auto& settings = CppUtils::UnitTest::executeCommands(argc, argv);
		if (settings.abort)
			return exitSuccess;
		*/
		auto settings = CppUtils::UnitTest::TestSettings{};
		return CppUtils::UnitTest::executeTests(std::move(settings));
	}
	catch (const std::exception& exception)
	{
		CppUtils::logException(exception);
		return exitFailure;
	}
	return exitSuccess;
}

auto main(const int argc, const char* argv[]) -> int
{
	CppUtils::Terminal::setConsoleOutputUTF8();
	return start(std::vector<std::string_view>{argv, argv + argc});
}

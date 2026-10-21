import std;
import CppUtils;

auto start(std::span<const std::string_view> arguments) -> int
{
	try
	{
		if (not std::ranges::empty(arguments))
			arguments = arguments.subspan(1);
		auto settings = CppUtils::UnitTest::executeCommands(arguments);
		if (settings.abort)
			return CppUtils::exitSuccess;
		return CppUtils::UnitTest::executeTests(std::move(settings));
	}
	catch (const std::exception& exception)
	{
		CppUtils::logException<CppUtils::Logger<"UnitTests">>(exception);
		return CppUtils::exitFailure;
	}
	return CppUtils::exitSuccess;
}

auto main(const int argc, const char* argv[]) -> int
{
	CppUtils::System::installCrashHandler();
	CppUtils::Terminal::setConsoleOutputUTF8();
	return start(std::vector<std::string_view>{argv, argv + argc});
}

import std;
import CppUtils;
import CppUtils.UnitTests;

auto start(std::span<const std::string_view> args) -> int
{
	try
	{
		auto settings = CppUtils::UnitTest::TestSettings{.fastAbort = false};
		for (auto i = 0uz; i < std::size(args); ++i)
			if (args[i] == "--filter" and i + 1 < std::size(args))
				settings.filter = std::string{args[++i]};
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
	CppUtils::Terminal::setConsoleOutputUTF8();
	return start(std::vector<std::string_view>{argv, argv + argc});
}

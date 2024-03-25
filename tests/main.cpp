import std;
import CppUtils;
import CppUtils.UnitTests;

auto start([[maybe_unused]] std::span<const std::string_view> args) -> int
{
	try
	{
		/*
		auto& settings = CppUtils::UnitTest::executeCommands(argc, argv);
		if (settings.abort)
			return CppUtils::exitSuccess;
		*/
		auto settings = CppUtils::UnitTest::TestSettings{};
		return CppUtils::UnitTest::executeTests(std::move(settings));
	}
	catch (const std::exception& exception)
	{
		CppUtils::logException(exception);
		return CppUtils::exitFailure;
	}
	return CppUtils::exitSuccess;
}

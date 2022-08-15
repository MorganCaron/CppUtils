#pragma once

#include <CppUtils.hpp>

namespace CppUtils::UnitTests::Log::Logger
{
	TEST_GROUP("Log/Logger")
	{
		addTest("", [] {
			CppUtils::Log::Logger{std::cout}
				<< Terminal::TextColor::TextColorEnum::Default << "Information message\n"
				<< Terminal::TextColor::TextColorEnum::Cyan << "Important message\n"
				<< Terminal::TextColor::TextColorEnum::Green << "Success message\n"
				<< Terminal::TextColor::TextColorEnum::Magenta << "Debug message\n"
				<< Terminal::TextColor::TextColorEnum::Blue << "Detail message\n"
				<< Terminal::TextColor::TextColorEnum::Yellow << "Warning message\n"
				<< Terminal::TextColor::TextColorEnum::Red << "Error message\n";
			TEST_ASSERT(true);
		});
	}
}

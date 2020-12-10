# [CppUtils](../README.md) / Log

[![Back to Parent Page](https://img.shields.io/badge/-Back_to_Parent_Page-blue?style=for-the-badge)](../README.md)

## Logger

In C++, the ``std::cout``, ``std::cerr`` and ``std::clog`` streams allow to display logs in the terminal.
But writing logs directly into the streams has its limits:
- No color in the terminal
- No activation/deactivation of certain types of logs
- No redirection of logs to another destination (like a log file)

The ``Logger`` class encapsulates these streams by adding these features.

<p align="center"><img src="resources/Logger.drawio.svg" alt="Logger diagram"/></p>

The macro ``ASSERT(condition);`` can be called in unit tests to make them fail if the condition is false.
It throws a TestException caught by the unit test.

### Example
```cpp
#include <CppUtils.hpp>

int main()
{
	const auto tests = std::vector<CppUtils::Test::UnitTest>{

		CppUtils::Test::UnitTest{"UnitTest 1", [] {
			ASSERT("azerty" != "qwerty");
		}},

		CppUtils::Test::UnitTest{"UnitTest 2", [] {
			ASSERT("azerty" == "qwerty");
		}}

	};

	return CppUtils::Test::UnitTest::executeTests(tests);
}
```

---

## ChronoLogger

The ``ChronoLogger`` class is a logger that allows to measure an elapsed time.

The ``ChronoLogger{}`` constructor starts the chronometer automatically.

It automatically writes a log line at each stop of the chronometer.

The logger can be enabled and disabled with the ``enable()`` and ``disable()`` methods.

<p align="center"><img src="resources/ChronoLogger.drawio.svg" alt="ChronoLogger diagram"/></p>

### Example
```cpp
#include <CppUtils.hpp>

int main()
{
	using namespace std::chrono_literals;

	auto chronoLogger = CppUtils::Log::ChronoLogger{"Test"};
	std::this_thread::sleep_for(5s);
	chronoLogger.stop();

	return 0;
}
```

---

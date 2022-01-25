#pragma once

#include <chrono>
#include <string>

namespace CppUtils::Chrono
{
	template <typename Rep, typename Period>
	[[nodiscard]] std::string durationToString(std::chrono::duration<Rep, Period> duration)
	{
		const auto hour = std::chrono::duration_cast<std::chrono::hours>(duration);
		duration -= hour;
		const auto min = std::chrono::duration_cast<std::chrono::minutes>(duration);
		duration -= min;
		const auto sec = std::chrono::duration_cast<std::chrono::seconds>(duration);
		duration -= sec;
		const auto millisec = std::chrono::duration_cast<std::chrono::milliseconds>(duration);
		duration -= millisec;
		const auto microsec = std::chrono::duration_cast<std::chrono::microseconds>(duration);
		duration -= microsec;
		const auto ns = std::chrono::duration_cast<std::chrono::nanoseconds>(duration);

		auto str = std::string{};
		if (hour.count() > 0)
			str += " " + std::to_string(hour.count()) + "h";
		if (min.count() > 0)
			str += " " + std::to_string(min.count()) + "m";
		if (sec.count() > 0)
			str += " " + std::to_string(sec.count()) + "s";
		if (millisec.count() > 0)
			str += " " + std::to_string(millisec.count()) + "ms";
		if (microsec.count() > 0)
			str += " " + std::to_string(microsec.count()) + "us";
		if (ns.count() > 0)
			str += " " + std::to_string(ns.count()) + "ns";
		return !str.empty() ? str.substr(1) : "0ns";
	}

	template<typename Clock = std::chrono::high_resolution_clock>
	requires std::chrono::is_clock_v<Clock>
	class Chronometer final
	{
	public:
		Chronometer()
		{
			start();
		}

		void start()
		{
			m_start = Clock::now();
		}

		void stop()
		{
			m_end = Clock::now();
		}

		[[nodiscard]] auto getDuration() const
		{
			return m_end - m_start;
		}

	private:
		std::chrono::time_point<Clock> m_start;
		std::chrono::time_point<Clock> m_end;
	};
}

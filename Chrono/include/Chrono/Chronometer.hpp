#pragma once

#include <chrono>
#include <string>

namespace CppUtils::Chrono
{
	class Chronometer
	{
	public:
		Chronometer(): m_start(std::chrono::high_resolution_clock::now())
		{}

		inline void start()
		{
			m_start = std::chrono::high_resolution_clock::now();
		}

		inline void stop()
		{
			m_end = std::chrono::high_resolution_clock::now();
		}

		inline auto getDuration()
		{
			return m_end - m_start;
		}

		inline std::string getText()
		{
			auto duration = getDuration();
			const auto hour = duration_cast<std::chrono::hours>(duration);
			duration -= hour;
			const auto min = duration_cast<std::chrono::minutes>(duration);
			duration -= min;
			const auto sec = duration_cast<std::chrono::seconds>(duration);
			duration -= sec;
			const auto millisec = duration_cast<std::chrono::milliseconds>(duration);
			duration -= millisec;
			const auto microsec = duration_cast<std::chrono::microseconds>(duration);
			duration -= microsec;
			const auto ns = duration_cast<std::chrono::nanoseconds>(duration);

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

	public:
		std::chrono::time_point<std::chrono::high_resolution_clock> m_start;
		std::chrono::time_point<std::chrono::high_resolution_clock> m_end;
	};
}

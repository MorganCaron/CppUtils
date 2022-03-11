#pragma once

#include <CppUtils/Chrono/Chronometer.hpp>
#include <CppUtils/Log/Logger.hpp>

namespace CppUtils::Log
{
	class ChronoLogger final
	{
	public:
		explicit ChronoLogger(std::string name, bool enabled = true):
			m_name{std::move(name)},
			m_enabled{enabled},
			m_chrono{}
		{}

		inline void start()
		{
			if (!m_enabled)
				return;
			m_chrono.start();
		}

		inline void stop()
		{
			if (!m_enabled)
				return;
			m_chrono.stop();
			Logger::logDebug(m_name + " duration: " + CppUtils::Chrono::durationToString(m_chrono.getDuration()));
		}

		inline void enable() noexcept
		{
			m_enabled = true;
		}

		inline void disable() noexcept
		{
			m_enabled = false;
		}

	private:
		std::string m_name;
		bool m_enabled;
		Chrono::Chronometer<> m_chrono;
	};
}

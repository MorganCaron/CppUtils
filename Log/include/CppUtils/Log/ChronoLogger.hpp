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

		auto start() -> void
		{
			if (!m_enabled)
				return;
			m_chrono.start();
		}

		auto stop() -> void
		{
			using Logger = Logger<"CppUtils">;
			if (!m_enabled)
				return;
			m_chrono.stop();
			Logger::print<"detail">("{} duration: {}", m_name, CppUtils::Chrono::durationToString(m_chrono.getDuration()));
		}

		auto enable() noexcept -> void
		{
			m_enabled = true;
		}

		auto disable() noexcept -> void
		{
			m_enabled = false;
		}

	private:
		std::string m_name;
		bool m_enabled;
		Chrono::Chronometer<> m_chrono;
	};
}

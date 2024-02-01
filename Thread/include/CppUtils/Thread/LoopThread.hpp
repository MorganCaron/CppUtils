#pragma once

#include <atomic>
#include <chrono>
#include <functional>
#include <memory>
#include <thread>
#include <utility>

namespace CppUtils::Thread
{
	class LoopThread final
	{
	public:
		LoopThread() = delete;

		template<Chrono::Concept::Duration Duration = std::chrono::nanoseconds>
		explicit LoopThread(std::function<void()> function, std::function<void()> interruptFunction = nullptr, Duration&& interval = Duration::zero()):
			m_function{std::move(function)},
			m_interruptFunction{std::move(interruptFunction)}
		{
			start(interval);
		}

		LoopThread(const LoopThread&) = delete;
		LoopThread(LoopThread&& src) noexcept:
			m_function{std::move(src.m_function)},
			m_running{src.isRunning()},
			m_thread{std::move(src.m_thread)}
		{}
		LoopThread& operator=(const LoopThread&) = delete;
		LoopThread& operator=(LoopThread&& rhs) noexcept
		{
			m_function = std::move(rhs.m_function);
			m_running = rhs.isRunning();
			m_thread = std::move(rhs.m_thread);
			return *this;
		}

		~LoopThread()
		{
			stop();
		}

		[[nodiscard]] auto isRunning() const noexcept -> bool
		{
			return m_running.load();
		}

		template<Chrono::Concept::Duration Duration = std::chrono::nanoseconds>
		auto start(Duration&& interval = Duration::zero()) -> void
		{
			if (isRunning())
				stop();
			m_thread = std::jthread{&LoopThread::run<std::remove_cvref_t<Duration>>, this, interval};
		}

		auto stop() -> void
		{
			if (!isRunning())
				return;
			m_running = false;
			if (m_interruptFunction)
				m_interruptFunction();
			m_thread.join();
		}

	private:
		template<Chrono::Concept::Duration Duration = std::chrono::nanoseconds>
		auto run(Duration&& interval = Duration::zero()) -> void
		{
			m_running = true;
			while (isRunning())
			{
				m_function();
				if (interval != Duration::zero())
					std::this_thread::sleep_for(interval);
			}
		}

		std::function<void()> m_function;
		std::function<void()> m_interruptFunction;
		std::atomic_bool m_running = false;
		std::jthread m_thread;
	};
}

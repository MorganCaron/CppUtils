#pragma once

#include <atomic>
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

		explicit LoopThread(std::function<void()> function, std::function<void()> interruptFunction = nullptr):
			m_function{std::move(function)},
			m_interruptFunction{std::move(interruptFunction)}
		{
			start();
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

		auto start() -> void
		{
			if (isRunning())
				stop();
			m_thread = std::jthread{&LoopThread::run, this};
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
		auto run() -> void
		{
			m_running = true;
			while (isRunning())
				m_function();
		}

		std::function<void()> m_function;
		std::function<void()> m_interruptFunction;
		std::atomic_bool m_running = false;
		std::jthread m_thread;
	};
}

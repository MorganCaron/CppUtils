#pragma once

#include <atomic>
#include <chrono>
#include <memory>
#include <thread>
#include <utility>
#include <functional>

using namespace std::chrono_literals;

namespace CppUtils::Thread
{
	class LoopThread final
	{
	public:
		LoopThread() = delete;

		explicit LoopThread(const std::function<void()>& function): m_function(function), m_running(false)
		{}

		template<typename Rep, typename Period>
		explicit LoopThread(const std::function<void()>& function,
			const std::chrono::duration<Rep, Period>& interval): m_function(function), m_running(false)
		{
			start(interval);
		}
		
		LoopThread(const LoopThread&) = delete;
		LoopThread(LoopThread&& src) noexcept:
			m_function(std::move(src.m_function)),
			m_running(src.isRunning()),
			m_thread(std::move(src.m_thread))
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

		[[nodiscard]] inline bool isRunning() const noexcept
		{
			return m_running.load();
		}

		template<typename Rep, typename Period>
		void start(const std::chrono::duration<Rep, Period>& interval)
		{
			if (isRunning())
				stop();
			m_thread = std::thread(&LoopThread::run<Rep, Period>, this, interval);
		}

		void stop()
		{
			if (!isRunning())
				return;
			m_running = false;
			m_thread.join();
		}

	private:
		template<typename Rep, typename Period>
		void run(const std::chrono::duration<Rep, Period>& interval)
		{
			m_running = true;
			while (isRunning())
			{
				m_function();
				std::this_thread::sleep_for(interval);
			}
		}

		std::function<void()> m_function;
		std::atomic<bool> m_running;
		std::thread m_thread;
	};
}

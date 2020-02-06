#pragma once

#include <atomic>
#include <chrono>
#include <memory>
#include <thread>
#include <utility>
#include <functional>

using namespace std::chrono_literals;

namespace CppUtils
{
	class LoopThread
	{
	private:
		struct ThreadData
		{
			explicit ThreadData(const std::function<void()>& m_function)
				: function(m_function), running(false)
			{};

			std::function<void()> function;
			std::atomic<bool> running;
			std::thread thread;
		};

		template<class Rep, class Period>
		void run(const std::chrono::duration<Rep, Period>& interval)
		{
			m_threadData->running = true;
			while (isRunning())
			{
				m_threadData->function();
				std::this_thread::sleep_for(interval);
			}
		}

	public:
		LoopThread() = delete;

		explicit LoopThread(const std::function<void()>& function)
			: m_threadData(std::make_unique<ThreadData>(function))
		{}

		template<class Rep, class Period>
		explicit LoopThread(const std::function<void()>& function,
			const std::chrono::duration<Rep, Period>& interval)
			: m_threadData(std::make_unique<ThreadData>(function))
		{
			start(interval);
		}
		
		LoopThread(const LoopThread&) = delete;
		LoopThread(LoopThread&&) noexcept = default;
		LoopThread& operator=(const LoopThread&) = delete;
		LoopThread& operator=(LoopThread&&) noexcept = default;

		virtual ~LoopThread()
		{
			stop();
		}

		inline bool isRunning() const noexcept
		{
			return m_threadData->running.load();
		}

		template<class Rep, class Period>
		void start(const std::chrono::duration<Rep, Period>& interval)
		{
			if (isRunning())
				stop();
			m_threadData->thread = std::thread(&LoopThread::run<Rep, Period>, this, interval);
		}

		void stop()
		{
			if (!isRunning())
				return;
			m_threadData->running = false;
			m_threadData->thread.join();
		}

	private:
		std::unique_ptr<ThreadData> m_threadData;
	};
}

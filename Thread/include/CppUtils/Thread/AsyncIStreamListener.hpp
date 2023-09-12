#pragma once

#include <thread>
#include <future>
#include <iostream>
#include <functional>

#include <CppUtils/Thread/LoopThread.hpp>

namespace CppUtils::Thread
{
	template<class Type>
	class AsyncIStreamListener
	{
	public:
		AsyncIStreamListener() = delete;

		explicit AsyncIStreamListener(std::basic_istream<Type>& istream,
			const std::function<void(Type)>& function):
			m_istream{std::ref(istream)},
			m_function{std::move(function)},
			m_loopThread{std::bind(&AsyncIStreamListener::listener, this)}
		{}

		AsyncIStreamListener(const AsyncIStreamListener&) = delete;
		AsyncIStreamListener(AsyncIStreamListener&&) noexcept = default;
		auto operator=(const AsyncIStreamListener&) -> AsyncIStreamListener& = delete;
		auto operator=(AsyncIStreamListener&&) noexcept -> AsyncIStreamListener& = default;

		[[nodiscard]] auto isRunning() const noexcept -> bool
		{
			return m_loopThread.isRunning();
		}

		auto start() -> void
		{
			m_loopThread.start();
		}

		auto stop() -> void
		{
			m_loopThread.stop();
		}

	private:
		auto listener() -> void
		{
			Type value;
			m_istream.get() >> value;
			m_function(value);
		}

		std::reference_wrapper<std::istream> m_istream;
		std::function<void(Type)> m_function;
		LoopThread m_loopThread;
	};
}

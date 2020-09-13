#pragma once

#include <thread>
#include <future>
#include <iostream>
#include <functional>

#include <Thread/LoopThread.hpp>

namespace CppUtils::Thread
{
	template<typename Type>
	class AsyncIStreamListener
	{
	public:
		AsyncIStreamListener() = delete;

		explicit AsyncIStreamListener(std::basic_istream<Type>& istream,
			const std::function<void(Type data)>& m_function):
			m_istream(std::ref(istream)),
			m_function(std::move(m_function)),
			m_loopThread(std::bind(&AsyncIStreamListener::listener, this), 0s)
		{}

		AsyncIStreamListener(const AsyncIStreamListener&) = delete;
		AsyncIStreamListener(AsyncIStreamListener&&) noexcept = default;
		AsyncIStreamListener& operator=(const AsyncIStreamListener&) = delete;
		AsyncIStreamListener& operator=(AsyncIStreamListener&&) noexcept = default;

	private:
		void listener()
		{
			Type value;
			m_istream.get() >> value;
			m_function(value);
		}

		std::reference_wrapper<std::istream> m_istream;
		std::function<void(Type data)> m_function;
		LoopThread m_loopThread;
	};
}

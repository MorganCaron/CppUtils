#pragma once

#include <set>

namespace CppUtils::DesignPattern
{
	template<typename Event>
	class Observer
	{
	public:
		virtual ~Observer() = default;
		virtual void notification(const Event &event) = 0;
	};

	template<typename Event>
	class Observable
	{
	public:
		void notify(const Event &event)
		{
			for (auto &observer : m_observers)
				observer->notification(event);
		}
		void subscribe(Observer<Event> &observer)
		{
			m_observers.insert(&observer);
		}
		void unsubscribe(Observer<Event> &observer)
		{
			m_observers.erase(&observer);
		}

	private:
		std::set<Observer<Event>*> m_observers;
	};
}

#pragma once

#include <vector>
#include <iostream>

namespace CppUtils
{
	class Switch
	{
	public:
		using Id = std::size_t;

		static inline Id newId(bool enable = false)
		{
			static auto id = Id{0};
			if (enable)
				m_enabledIds.push_back(id);
			return id++;
		}

		static inline bool isEnabled(const Id id)
		{
			return (std::find(m_enabledIds.begin(), m_enabledIds.end(), id) != m_enabledIds.end());
		}

		static inline void enable(const Id id)
		{
			if (!isEnabled(id))
				m_enabledIds.push_back(id);
		}

		static inline void disable(const Id id)
		{
			const auto it = std::find(m_enabledIds.begin(), m_enabledIds.end(), id);
			if (it != m_enabledIds.end())
				m_enabledIds.erase(it);
		}

		template<typename... Ids>
		static inline void enable(const Id id, Ids... ids)
		{
			enable(id);
			enable(ids...);
		}
		template<typename... Ids>
		static inline void disable(const Id id, Ids... ids)
		{
			disable(id);
			disable(ids...);
		}

		static inline std::vector<Id> getEnabledIds()
		{
			return m_enabledIds;
		}

		static inline void setEnabledIds(std::vector<Id> enabledIds)
		{
			m_enabledIds = std::move(enabledIds);
		}

	private:
		static std::vector<Id> m_enabledIds;
	};
}

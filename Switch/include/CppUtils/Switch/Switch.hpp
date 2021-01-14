#pragma once

#include <span>
#include <vector>
#include <stdexcept>
#include <unordered_map>

#include <CppUtils/Type/Token.hpp>

namespace CppUtils
{
	class Switch final
	{
	public:
		using Id = std::size_t;
		using Tag = Type::Token;

		[[nodiscard]] static inline Id newId(bool enabled, std::span<const Tag> tags = {})
		{
			static auto id = Id{0};
			setValue(id, enabled);
			for (const auto& tag : tags)
				m_tags[tag].push_back(id);
			return id++;
		}

		[[nodiscard]] static inline bool valueExists(Id id) noexcept
		{
			return (m_idStates.find(id) != m_idStates.end());
		}

		[[nodiscard]] static inline bool getValue(Id id)
		{
			if (!valueExists(id))
				throw std::runtime_error{"Undefined Switch id"};
			return m_idStates.at(id);
		}

		static inline void setValue(Id id, bool value)
		{
			m_idStates[id] = value;
		}

		static inline void deleteId(Id id)
		{
			m_idStates.erase(id);
			for (auto& [tag, ids] : m_tags)
			{
				const auto it = std::find(ids.begin(), ids.end(), id);
				if (it != ids.end())
					ids.erase(it);
			}
		}

		[[nodiscard]] static inline std::span<const Id> getIds(const Tag& tag) noexcept
		{
			if (m_tags.find(tag) != m_tags.end())
				return m_tags.at(tag);
			return std::span<const Id>{};
		}

		[[nodiscard]] static std::unordered_map<Id, bool> getValues(const Tag& tag)
		{
			auto values = std::unordered_map<Id, bool>{};
			for (const auto& id : getIds(tag))
				values[id] = getValue(id);
			return values;
		}

		static inline void setValues(const std::unordered_map<Id, bool>& values)
		{
			for (const auto& [id, value] : values)
				setValue(id, value);
		}

		static inline void setValue(const Tag& tag, bool value)
		{
			for (const auto& id : getIds(tag))
				setValue(id, value);
		}

	private:
		static std::unordered_map<Id, bool> m_idStates;
		static std::unordered_map<Tag, std::vector<Id>, Tag::hash_fn> m_tags;
	};
}

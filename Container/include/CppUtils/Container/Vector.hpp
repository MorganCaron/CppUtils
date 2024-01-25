#pragma once

#include <memory>
#include <unordered_map>
#include <vector>

namespace CppUtils::Container::Vector
{
	template<class T>
	auto merge(std::vector<T>& dest, const std::vector<T>& src) -> void
	{
		dest.reserve(dest.size() + src.size());
		dest.insert(dest.end(), src.begin(), src.end());
	}

	template<class T>
	[[nodiscard]] auto merge(const std::initializer_list<std::reference_wrapper<const std::vector<T>>>& vectors) -> std::vector<T>
	{
		auto size = 0uz;
		for (const auto& vector : vectors)
			size += vector.get().size();
		auto newVector = std::vector<T>{};
		newVector.reserve(size);
		for (const auto& vector : vectors)
			newVector.insert(newVector.end(), vector.get().begin(), vector.get().end());
		return newVector;
	}
}

#pragma once

#include <memory>
#include <vector>
#include <unordered_map>

namespace CppUtils::Container::Vector
{
	template<typename T>
	inline void merge(std::vector<T>& dest, const std::vector<T>& src)
	{
		dest.reserve(dest.size() + src.size());
		dest.insert(dest.end(), src.begin(), src.end());
	}

	template<typename T>
	[[nodiscard]] std::vector<T> merge(const std::initializer_list<std::reference_wrapper<const std::vector<T>>>& vectors)
	{
		auto size = std::size_t{0};
		for(const auto &vector : vectors)
			size += vector.get().size();
		auto newVector = std::vector<T>{};
		newVector.reserve(size);
		for(const auto& vector : vectors)
			newVector.insert(newVector.end(), vector.get().begin(), vector.get().end());
		return newVector;
	}
}

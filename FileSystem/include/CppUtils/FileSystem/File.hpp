#pragma once

#include <cstdio>
#include <filesystem>
#include <fstream>
#include <string_view>
#include <vector>

namespace CppUtils::FileSystem::File
{
	template<typename FuncType>
	inline auto forFilesWithExtension(const std::string& path, const std::string& ext, FuncType&& function) -> void
	{
		for (const auto& file : std::filesystem::directory_iterator(path))
			if (file.path().extension() == ext)
				function(file.path().string());
	}

	inline auto deleteFile(const std::filesystem::path& filePath) -> void
	{
		std::remove(filePath.string().c_str());
	}

	namespace Binary
	{
		template<class T>
		inline auto write(const std::filesystem::path& filePath, const T& buffer) -> void
		{
			auto file = std::ofstream{filePath, std::ios::binary};
			if (!file.is_open())
				throw std::runtime_error{"Failed to open " + filePath.string() + " file"};
			file.write(reinterpret_cast<const char*>(&buffer), sizeof(buffer));
		}

		template<class T>
		[[nodiscard]] inline auto read(const std::filesystem::path& filePath) -> T
		{
			auto file = std::ifstream{filePath, std::ios::binary};
			if (!file.is_open())
				throw std::runtime_error{"Failed to open " + filePath.string() + " file"};
			auto buffer = T{};
			file.read(reinterpret_cast<char*>(&buffer), sizeof(buffer));
			return buffer;
		}

		template<class T>
		inline auto writeVector(const std::filesystem::path& filePath, const std::vector<T>& vector) -> void
		{
			auto file = std::ofstream{filePath, std::ios::binary};
			if (!file.is_open())
				throw std::runtime_error{"Failed to open " + filePath.string() + " file"};
			file.write(reinterpret_cast<const char*>(std::data(vector)), static_cast<std::streamsize>(std::size(vector) * sizeof(T)));
		}

		template<class T>
		[[nodiscard]] inline auto readVector(const std::filesystem::path& filePath) -> std::vector<T>
		{
			auto file = std::ifstream{filePath, std::ios::binary};
			if (!file.is_open())
				throw std::runtime_error{"Failed to open " + filePath.string() + " file"};
			const auto nbElements = std::filesystem::file_size(filePath) / sizeof(T);
			auto vector = std::vector<T>{};
			vector.resize(nbElements);
			file.read(reinterpret_cast<char*>(std::data(vector)), static_cast<std::streamsize>(nbElements * sizeof(T)));
			return vector;
		}
	}

	namespace String
	{
		inline auto write(const std::filesystem::path& filePath, std::string_view content) -> void
		{
			auto file = std::ofstream{filePath};
			if (!file.is_open())
				throw std::runtime_error{"Failed to open " + filePath.string() + " file"};
			file << content;
		}

		inline auto append(const std::filesystem::path& filePath, std::string_view content) -> void
		{
			auto file = std::ofstream{filePath, std::ios::app};
			if (!file.is_open())
				throw std::runtime_error{"Failed to open " + filePath.string() + " file"};
			file << content;
		}

		[[nodiscard]] inline auto read(const std::filesystem::path& filePath) -> std::string
		{
			auto file = std::ifstream{filePath};
			if (!file.is_open())
				throw std::runtime_error{"Failed to open " + filePath.string() + " file"};
			return std::string{std::istreambuf_iterator<char>{file}, std::istreambuf_iterator<char>{}};
		}
	}
}

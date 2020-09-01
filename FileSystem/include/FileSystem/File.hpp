#pragma once

#include <cstdio>
#include <fstream>
#include <filesystem>
#include <string_view>

using namespace std::string_literals;

namespace CppUtils::FileSystem::File
{
	template <typename FuncType>
	inline void forFilesWithExtension(const std::string& path, const std::string& ext, FuncType&& function)
	{
		for (const auto& file : std::filesystem::directory_iterator(path))
			if (file.path().extension() == ext)
				function(file.path().string());
	}

	inline void deleteFile(const std::filesystem::path& filePath)
	{
		std::remove(filePath.string().c_str());
	}
	
	namespace Binary
	{
		template<typename T>
		inline void write(const std::filesystem::path& filePath, const T& buffer)
		{
			auto file = std::ofstream{filePath, std::ios::binary};
			if (!file.is_open())
				throw std::runtime_error("Failed to open " + filePath.string() + " file");
			file.write(reinterpret_cast<const char*>(&buffer), sizeof(buffer));
		}

		template<typename T>
		[[nodiscard]] inline T read(const std::filesystem::path& filePath)
		{
			auto file = std::ifstream{filePath, std::ios::binary};
			if (!file.is_open())
				throw std::runtime_error("Failed to open " + filePath.string() + " file");
			auto buffer = T{};
			file.read(reinterpret_cast<char*>(&buffer), sizeof(buffer));
			return buffer;
		}

		template<typename T>
		inline void writeVector(const std::filesystem::path& filePath, const std::vector<T>& vector)
		{
			auto file = std::ofstream{filePath, std::ios::binary};
			if (!file.is_open())
				throw std::runtime_error("Failed to open " + filePath.string() + " file");
			std::copy(vector.begin(), vector.end(), std::ostreambuf_iterator<char>(file));
		}

		template<typename T>
		[[nodiscard]] inline std::vector<T> readVector(const std::filesystem::path& filePath)
		{
			auto file = std::ifstream{filePath, std::ios::binary};
			if (!file.is_open())
				throw std::runtime_error("Failed to open " + filePath.string() + " file");
			return std::vector<T>{std::istreambuf_iterator<char>{file}, std::istreambuf_iterator<char>{}};
		}
	}
	
	namespace String
	{
		inline void write(const std::filesystem::path& filePath, std::string_view content)
		{
			auto file = std::ofstream{filePath};
			if (!file.is_open())
				throw std::runtime_error("Failed to open " + filePath.string() + " file");
			file << content;
		}

		inline void append(const std::filesystem::path& filePath, std::string_view content)
		{
			auto file = std::ofstream{filePath, std::ios::app};
			if (!file.is_open())
				throw std::runtime_error("Failed to open " + filePath.string() + " file");
			file << content;
		}
		
		[[nodiscard]] inline std::string read(const std::filesystem::path& filePath)
		{
			auto file = std::ifstream{filePath};
			if (!file.is_open())
				throw std::runtime_error("Failed to open " + filePath.string() + " file");
			return std::string{std::istreambuf_iterator<char>{file}, std::istreambuf_iterator<char>{}};
		}
	}
}

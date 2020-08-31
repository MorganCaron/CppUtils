#pragma once

#include <cstdio>
#include <fstream>
#include <filesystem>
#include <string_view>

using namespace std::string_literals;

namespace CppUtils::FileSystem::File
{
	template <typename FuncType>
	void forFilesWithExtension(const std::string& path, const std::string& ext, FuncType&& function)
	{
		for (const auto& file : std::filesystem::directory_iterator(path))
			if (file.path().extension() == ext)
				function(file.path().string());
	}

	inline void deleteFile(const std::filesystem::path& filePath)
	{
		std::remove(filePath.string().c_str());
	}
	
	namespace String
	{
		void write(const std::filesystem::path& filePath, std::string_view content)
		{
			auto file = std::ofstream{filePath};
			if (!file.is_open())
				throw std::runtime_error("Failed to open " + filePath.string() + " file");
			file << content;
		}

		void append(const std::filesystem::path& filePath, std::string_view content)
		{
			auto file = std::ofstream{filePath, std::ios::app};
			if (!file.is_open())
				throw std::runtime_error("Failed to open " + filePath.string() + " file");
			file << content;
		}
		
		[[nodiscard]] std::string read(const std::filesystem::path& filePath)
		{
			auto file = std::ifstream{filePath};
			if (!file.is_open())
				throw std::runtime_error("Failed to open " + filePath.string() + " file");
			return std::string{(std::istreambuf_iterator<char>{file}), std::istreambuf_iterator<char>{}};
		}
	}
	
	namespace Binary
	{
		template<typename Type>
		void write(const std::filesystem::path& filePath, const Type& buffer)
		{
			auto file = std::ofstream{filePath, std::ios::binary};
			if (!file.is_open())
				throw std::runtime_error("Failed to open " + filePath.string() + " file");
			file.write(reinterpret_cast<const char*>(&buffer), sizeof(buffer));
		}

		template<typename Type>
		[[nodiscard]] Type read(const std::filesystem::path& filePath)
		{
			auto file = std::ifstream{filePath, std::ios::binary};
			if (!file.is_open())
				throw std::runtime_error("Failed to open " + filePath.string() + " file");
			auto buffer = Type{};
			file.read(reinterpret_cast<char*>(&buffer), sizeof(buffer));
			return buffer;
		}
	}
}

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
	
	[[nodiscard]] inline std::string readString(const std::filesystem::path& filePath)
	{
		auto ifstream = std::ifstream(filePath);
		return std::string{(std::istreambuf_iterator<char>{ifstream}), std::istreambuf_iterator<char>{}};
	}

	inline void writeString(const std::filesystem::path& filePath, std::string_view content)
	{
		auto file = std::ofstream(filePath, std::fstream::out);
		if (!file.is_open())
			throw std::runtime_error("Echec d'ouverture du fichier. Existe t'il?");
		file << content;
	}

	inline void appendString(const std::filesystem::path& filePath, std::string_view content)
	{
		auto file = std::ofstream(filePath, std::fstream::out | std::ofstream::app);
		if (!file.is_open())
			throw std::runtime_error("Echec d'ouverture du fichier. Existe t'il?");
		file << content;
	}

	inline void deleteFile(const std::filesystem::path& filePath)
	{
		std::remove(filePath.string().c_str());
	}
}

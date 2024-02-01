#pragma once

#include <filesystem>
#include <functional>
#include <mutex>
#include <unordered_map>

#include <CppUtils/Thread/LoopThread.hpp>

namespace CppUtils::FileSystem
{
	enum FileStatus : std::uint8_t
	{
		Created = 0b1,
		Modified = 0b10,
		Deleted = 0b100
	};

	using WatchedFileStatus = std::uint8_t;

	inline constexpr auto allFileStatus = WatchedFileStatus{
		FileStatus::Created |
		FileStatus::Modified |
		FileStatus::Deleted};

	class FileWatcher final
	{
	public:
		using SubscribeFunction = std::function<void(const std::filesystem::path&, FileStatus)>;

	private:
		struct FileInfos final
		{
			FileInfos() = default;
			explicit FileInfos(const std::filesystem::path& path):
				path{path},
				exists{std::filesystem::exists(path)},
				lastWriteTime{
					std::filesystem::exists(path) ?
						std::filesystem::last_write_time(path) :
						std::filesystem::file_time_type{}}
			{}

			std::filesystem::path path;
			bool exists;
			std::filesystem::file_time_type lastWriteTime;
		};

		struct WatchFileOptions
		{
			SubscribeFunction subscribedFunction;
			WatchedFileStatus watchStatus;
		};

		struct WatchDirectoryOptions final: public WatchFileOptions
		{
			WatchDirectoryOptions() = default;
			WatchDirectoryOptions(SubscribeFunction subscribedFunction, WatchedFileStatus watchStatus, bool recursively):
				WatchFileOptions{subscribedFunction, watchStatus},
				recursively{recursively},
				firstScan{true}
			{}

			bool recursively;
			bool firstScan;
		};

		struct WatchedFile final
		{
			WatchedFile() = default;
			WatchedFile(const std::filesystem::path& path, WatchFileOptions options):
				infos{path},
				options{options}
			{}

			FileInfos infos;
			WatchFileOptions options;
		};

		using FilesInfos = std::unordered_map<std::filesystem::path, FileInfos>;

		struct WatchedDirectory final
		{
			WatchedDirectory() = default;
			WatchedDirectory(const std::filesystem::path& path, WatchDirectoryOptions options):
				path{path},
				filesInfos{},
				options{options}
			{}

			std::filesystem::path path;
			FilesInfos filesInfos;
			WatchDirectoryOptions options;
		};

		using WatchedFiles = std::unordered_map<std::filesystem::path, WatchedFile>;
		using WatchedDirectories = std::unordered_map<std::filesystem::path, WatchedDirectory>;

	public:
		FileWatcher() = delete;
		template<Chrono::Concept::Duration Duration = std::chrono::seconds>
		explicit FileWatcher(Duration&& interval = Duration{1}):
			m_loopThread{std::bind(&FileWatcher::listener, this), nullptr, interval}
		{}

		~FileWatcher()
		{
			stop();
		}

		[[nodiscard]] auto isRunning() const noexcept -> bool
		{
			return m_loopThread.isRunning();
		}

		template<Chrono::Concept::Duration Duration = std::chrono::seconds>
		auto start(Duration&& interval = Duration{1}) -> void
		{
			m_loopThread.start(interval);
		}

		auto stop() -> void
		{
			m_loopThread.stop();
		}

		auto watchFile(
			const std::filesystem::path& filePath,
			SubscribeFunction function,
			WatchedFileStatus watchStatus = allFileStatus) -> void
		{
			[[maybe_unused]] auto lockGuard = std::unique_lock<std::mutex>{m_mutex};
			m_watchedFiles[filePath] = WatchedFile{filePath, WatchFileOptions{std::move(function), watchStatus}};
		}

		auto unwatchFile(const std::filesystem::path& filePath) -> void
		{
			[[maybe_unused]] auto lockGuard = std::unique_lock<std::mutex>{m_mutex};
			m_watchedFiles.erase(filePath);
		}

		auto watchDirectory(
			const std::filesystem::path& directory,
			SubscribeFunction function,
			WatchedFileStatus watchStatus = allFileStatus,
			bool recursively = true) -> void
		{
			[[maybe_unused]] auto lockGuard = std::unique_lock<std::mutex>{m_mutex};
			m_watchedDirectories[directory] = WatchedDirectory{directory,
				WatchDirectoryOptions{std::move(function), watchStatus, recursively}};
		}

		auto unwatchDirectory(const std::filesystem::path& directory) -> void
		{
			[[maybe_unused]] auto lockGuard = std::unique_lock<std::mutex>{m_mutex};
			m_watchedDirectories.erase(directory);
		}

	private:
		auto checkFile(FileInfos& infos, const WatchFileOptions& options) -> void
		{
			auto& [path, exists, lastWriteTime] = infos;
			const auto& [subscribedFunction, watchStatus] = options;
			if (std::filesystem::exists(path))
			{
				if (auto currentLastWriteTime = std::filesystem::last_write_time(path);
					not exists)
				{
					lastWriteTime = currentLastWriteTime;
					exists = true;
					if (watchStatus & FileStatus::Created)
						subscribedFunction(path, FileStatus::Created);
				}
				else if (lastWriteTime != currentLastWriteTime)
				{
					lastWriteTime = currentLastWriteTime;
					if (watchStatus & FileStatus::Modified)
						subscribedFunction(path, FileStatus::Modified);
				}
			}
			else if (exists)
			{
				exists = false;
				if (watchStatus & FileStatus::Deleted)
					subscribedFunction(path, FileStatus::Deleted);
			}
		}

		auto checkDirectory(WatchedDirectory& watchedDirectory) -> void
		{
			auto& [directoryPath, filesInfos, options] = watchedDirectory;
			forFiles(directoryPath, [&filesInfos, &options](const auto& filePath) -> void {
				if (auto fileInfosIt = filesInfos.find(filePath); fileInfosIt == std::cend(filesInfos))
				{
					fileInfosIt = filesInfos.emplace(std::make_pair(filePath, FileInfos{filePath})).first;
					if (not options.firstScan)
						fileInfosIt->second.exists = false;
				}
			}, options.recursively);
			if (options.firstScan)
				options.firstScan = false;
			auto filesToUnwatch = std::vector<std::filesystem::path>{};
			for (auto& [filePath, fileInfos] : filesInfos)
			{
				checkFile(fileInfos, options);
				if (not std::filesystem::exists(filePath))
					filesToUnwatch.push_back(filePath);
			}
			for (const auto& fileToUnwatch : filesToUnwatch)
				filesInfos.erase(fileToUnwatch);
		}

		auto listener() -> void
		{
			[[maybe_unused]] auto lockGuard = std::unique_lock<std::mutex>{m_mutex};
			for (auto& [filePath, watchedFile] : m_watchedFiles)
				checkFile(watchedFile.infos, watchedFile.options);
			for (auto& [directory, WatchedDirectory] : m_watchedDirectories)
				checkDirectory(WatchedDirectory);
		}

		std::mutex m_mutex;
		WatchedFiles m_watchedFiles;
		WatchedDirectories m_watchedDirectories;
		Thread::LoopThread m_loopThread;
	};
}

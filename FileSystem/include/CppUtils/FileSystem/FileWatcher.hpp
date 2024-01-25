#pragma once

#include <filesystem>
#include <mutex>
#include <set>
#include <unordered_map>

#include <CppUtils/Thread/LoopThread.hpp>

namespace CppUtils::FileSystem
{
	class FileWatcher final
	{
	public:
		enum class FileStatus : char
		{
			Created,
			Modified,
			Deleted
		};

		FileWatcher() = delete;

		template<class Rep, class Period>
		explicit FileWatcher(const std::function<void(const std::filesystem::path& filePath, FileStatus)>& function, const std::chrono::duration<Rep, Period>& interval):
			m_function{std::move(function)},
			m_loopThread{std::bind(&FileWatcher::listener, this), interval}
		{}

		FileWatcher(const FileWatcher&) = delete;
		FileWatcher(FileWatcher&&) noexcept = default;
		FileWatcher& operator=(const FileWatcher&) = delete;
		FileWatcher& operator=(FileWatcher&&) noexcept = default;

		[[nodiscard]] auto isRunning() const noexcept -> bool
		{
			return m_loopThread.isRunning();
		}

		template<class Rep, class Period>
		auto start(const std::chrono::duration<Rep, Period>& interval) -> void
		{
			m_loopThread.start(interval);
		}

		auto stop() -> void
		{
			m_loopThread.stop();
		}

		auto watchPath(const std::filesystem::path& filePath) -> void
		{
			[[maybe_unused]] auto lockGuard = std::lock_guard<std::mutex>{m_mutex};
			m_watchedFiles.insert(filePath);
			m_fileStatus[filePath] = std::filesystem::last_write_time(filePath);
		}

		auto unwatchPath(const std::filesystem::path& filePath) -> void
		{
			[[maybe_unused]] auto lockGuard = std::lock_guard<std::mutex>{m_mutex};
			m_watchedFiles.erase(filePath);
			m_fileStatus.erase(filePath);
		}

	private:
		auto listener() -> void
		{
			[[maybe_unused]] auto lockGuard = std::lock_guard<std::mutex>{m_mutex};
			for (const auto& filePath : m_watchedFiles)
			{
				if (std::filesystem::exists(filePath))
				{
					auto lastWriteTime = std::filesystem::last_write_time(filePath);
					auto fileStatusIt = m_fileStatus.find(filePath);
					if (fileStatusIt == m_fileStatus.end())
					{
						m_function(filePath, FileStatus::Created);
						m_fileStatus[filePath] = lastWriteTime;
					}
					else if (fileStatusIt->second != lastWriteTime)
					{
						m_function(filePath, FileStatus::Modified);
						m_fileStatus[filePath] = lastWriteTime;
					}
				}
				else
				{
					m_function(filePath, FileStatus::Deleted);
					m_fileStatus.erase(filePath);
				}
			}
		}

		std::mutex m_mutex;
		std::set<std::filesystem::path> m_watchedFiles;
		std::unordered_map<std::filesystem::path, std::filesystem::file_time_type> m_fileStatus;
		std::function<void(const std::filesystem::path& filePath, FileStatus)> m_function;
		Thread::LoopThread m_loopThread;
	};
}

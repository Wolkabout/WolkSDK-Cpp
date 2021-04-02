/*
 * Copyright (c) 2021 WolkAbout.
 */

#include "LogManager.h"

#include "utilities/FileSystemUtils.h"
#include "utilities/Logger.h"
#include "utilities/Timer.h"

#include <chrono>
#include <ctime>
#include <exception>
#include <iostream>

namespace wolkabout
{
LogManager::LogManager(const std::string& logDirectory, const std::string& logExtension, const int& maxSize,
                       const std::chrono::hours& uploadEvery, const std::chrono::hours& deleteAfter,
                       std::shared_ptr<LogUploader> logUploader)
: m_logDirectory(logDirectory)
, m_logExtension(logExtension)
, m_maxSize(maxSize)
, m_uploadEvery(uploadEvery)
, m_deleteAfter(deleteAfter)
, m_running(false)
, m_logUploader(std::move(logUploader))
{
    if (!FileSystemUtils::isDirectoryPresent(m_logDirectory))
        throw std::logic_error("Provided log directory '" + m_logDirectory + "' does not exist!");
}

LogManager::~LogManager()
{
    stop();
}

bool LogManager::isRunning()
{
    return m_running;
}

void LogManager::restart()
{
    stop();

    start();
}
void LogManager::start()
{
    if (m_running)
        return;

    m_running = true;

    if (!m_logUploader)
    {
    }
}

void LogManager::stop()
{
    if (!m_running)
        return;
}

const std::string& LogManager::getLogDirectory() const
{
    return m_logDirectory;
}
void LogManager::setLogDirectory(const std::string& logDirectory)
{
    m_logDirectory = logDirectory;
}
int LogManager::getMaxSize() const
{
    return m_maxSize;
}
void LogManager::setMaxSize(int maxSize)
{
    m_maxSize = maxSize;
}
const std::chrono::hours& LogManager::getUploadEvery() const
{
    return m_uploadEvery;
}
void LogManager::setUploadEvery(const std::chrono::hours& uploadEvery)
{
    m_uploadEvery = uploadEvery;
}
const std::chrono::hours& LogManager::getDeleteAfter() const
{
    return m_deleteAfter;
}
void LogManager::setDeleteAfter(const std::chrono::hours& deleteAfter)
{
    m_deleteAfter = deleteAfter;
}

const std::string& LogManager::getLogExtension() const
{
    return m_logExtension;
}

void LogManager::setLogExtension(const std::string& logExtension)
{
    m_logExtension = logExtension;
}

std::vector<std::string> LogManager::getLogsToDelete()
{
    std::vector<std::string> files;

    if (m_deleteAfter == std::chrono::hours(0))
    {
        LOG(DEBUG) << "Not deleting log files.";
        return files;
    }

    std::vector<std::string> logFiles = FileSystemUtils::listFiles(m_logDirectory);
    std::remove_if(logFiles.begin(), logFiles.end(),
                   [&](const std::string file) { return file.substr(file.find_last_of(".") + 1) != m_logExtension; });

    // TODO: Get file last modified
    std::remove_if(logFiles.begin(), logFiles.end(), [&](const std::string file) {
        std::difftime(std::chrono::system_clock::to_time_t(std::chrono::system_clock::now()),
                      FileSystemUtils::getFileLastModified(file) > 0.0);
    });
    // TODO: Compare (current time - last modified) > m_deleteAfter

    return files;
}
std::vector<std::string> LogManager::getLogsToUpload()
{
    std::vector<std::string> files;

    if (m_deleteAfter == std::chrono::hours(0))
    {
        LOG(DEBUG) << "Not uploading log files.";
        return files;
    }

    // TODO: Get remote logs
    // TODO: Compare local and remote files, removing local from remote

    return files;
}
}    // namespace wolkabout

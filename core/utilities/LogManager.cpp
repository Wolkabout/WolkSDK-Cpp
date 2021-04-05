/*
 * Copyright (c) 2021 WolkAbout.
 */

#include "LogManager.h"

#include "utilities/FileSystemUtils.h"
#include "utilities/Logger.h"
#include "utilities/StringUtils.h"
#include "utilities/Timer.h"

#include <algorithm>
#include <ctime>
#include <exception>
#include <iostream>

namespace wolkabout
{
LogManager::LogManager(const std::string& logDirectory, const std::string& logExtension, const int& maxSize,
                       const std::chrono::hours& deleteEvery, const std::chrono::hours& deleteAfter,
                       const std::chrono::hours& uploadEvery, const std::chrono::hours& uploadAfter,
                       std::shared_ptr<LogUploader> logUploader)
: m_logDirectory(logDirectory)
, m_deleteEvery(deleteEvery)
, m_logExtension(logExtension)
, m_maxSize(maxSize)
, m_deleteAfter(deleteAfter)
, m_uploadEvery(uploadEvery)
, m_logUploader(std::move(logUploader))
, m_uploadAfter(uploadAfter)
, m_running(false)
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

    if (m_deleteEvery != std::chrono::hours(0))
    {
        deleteOldLogs();
        m_deleteTimer.run(m_deleteEvery, [&] { deleteOldLogs(); });
    }

    if (m_uploadEvery != std::chrono::hours(0) && m_logUploader)
    {
        uploadLogs();
        m_uploadTimer.run(m_uploadEvery, [&] { uploadLogs(); });
    }
}

void LogManager::stop()
{
    if (!m_running)
        return;

    m_running = false;

    m_uploadTimer.stop();
}

const std::string& LogManager::getLogDirectory() const
{
    return m_logDirectory;
}

void LogManager::setLogDirectory(const std::string& logDirectory)
{
    // WARNING: Use with caution or nuke a directory!
    m_logDirectory = logDirectory;

    if (m_deleteEvery != std::chrono::hours(0) || m_deleteAfter != std::chrono::hours(0))
    {
        restart();
    }
}

int LogManager::getMaxSize() const
{
    return m_maxSize;
}

void LogManager::setMaxSize(int maxSize)
{
    m_maxSize = maxSize;

    if (m_deleteEvery != std::chrono::hours(0) || m_deleteAfter != std::chrono::hours(0))
    {
        restart();
    }
}

const std::chrono::hours& LogManager::getUploadEvery() const
{
    return m_uploadEvery;
}

void LogManager::setUploadEvery(const std::chrono::hours& uploadEvery)
{
    m_uploadEvery = uploadEvery;

    if (m_uploadEvery == std::chrono::hours(0))
    {
        m_uploadTimer.stop();
        return;
    }
    restart();
}

const std::chrono::hours& LogManager::getDeleteEvery() const
{
    return m_deleteEvery;
}

void LogManager::setDeleteEvery(const std::chrono::hours& deleteEvery)
{
    m_deleteEvery = deleteEvery;

    if (m_deleteEvery != std::chrono::hours(0) || m_deleteAfter != std::chrono::hours(0))
    {
        restart();
    }
}
const std::chrono::hours& LogManager::getDeleteAfter() const
{
    return m_deleteAfter;
}

void LogManager::setDeleteAfter(const std::chrono::hours& deleteAfter)
{
    m_deleteAfter = deleteAfter;

    if (m_deleteEvery != std::chrono::hours(0) || m_deleteAfter != std::chrono::hours(0))
    {
        restart();
    }
}

const std::string& LogManager::getLogExtension() const
{
    return m_logExtension;
}

void LogManager::setLogExtension(const std::string& logExtension)
{
    m_logExtension = logExtension;

    if (m_deleteEvery != std::chrono::hours(0) || m_deleteAfter != std::chrono::hours(0))
    {
        restart();
    }
}

std::vector<std::string> LogManager::getLogsToDelete()
{
    std::vector<std::string> logFiles;

    if (m_deleteAfter == std::chrono::hours(0))
    {
        LOG(DEBUG) << "Not deleting log files.";
        return logFiles;
    }

    logFiles = getLogFiles();

    logFiles.erase(std::remove_if(logFiles.begin(), logFiles.end(),
                                  [&](const std::string& file) -> bool {
                                      return std::chrono::duration<double>(std::difftime(
                                               std::chrono::system_clock::to_time_t(std::chrono::system_clock::now()),
                                               FileSystemUtils::getLastModified(file))) < m_deleteAfter;
                                  }),
                   logFiles.end());

    return logFiles;
}

std::vector<std::string> LogManager::getLogsToUpload()
{
    std::vector<std::string> logFiles;

    if (m_uploadAfter == std::chrono::hours(0))
    {
        LOG(DEBUG) << "Not uploading log files.";
        return logFiles;
    }

    if (!m_logUploader)
    {
        LOG(WARN) << "No LogUploader, no files will be uploaded.";
        return logFiles;
    }

    logFiles = getLogFiles();

    std::vector<std::string> remoteFiles = m_logUploader->getRemoteLogs();

    logFiles.erase(std::remove_if(logFiles.begin(), logFiles.end(),
                                  [&](const std::string& file) -> bool {
                                      return std::find(remoteFiles.begin(), remoteFiles.end(), file) !=
                                             remoteFiles.end();
                                  }),
                   logFiles.end());

    logFiles.erase(std::remove_if(logFiles.begin(), logFiles.end(),
                                  [&](const std::string& file) -> bool {
                                      return std::chrono::duration<double>(std::difftime(
                                               std::chrono::system_clock::to_time_t(std::chrono::system_clock::now()),
                                               FileSystemUtils::getLastModified(file))) < m_uploadAfter;
                                  }),
                   logFiles.end());

    return logFiles;
}

std::vector<std::string> LogManager::getLogFiles()
{
    std::vector<std::string> logFiles;

    logFiles = FileSystemUtils::listFiles(m_logDirectory);
    logFiles.erase(std::remove_if(logFiles.begin(), logFiles.end(),
                                  [&](const std::string& file) -> bool {
                                      return !wolkabout::StringUtils::endsWith(file, m_logExtension);
                                  }),
                   logFiles.end());
    return logFiles;
}

const std::chrono::hours& LogManager::getUploadAfter() const
{
    return m_uploadAfter;
}

void LogManager::setUploadAfter(const std::chrono::hours& uploadAfter)
{
    m_uploadAfter = uploadAfter;

    if (m_uploadAfter != std::chrono::hours(0))
    {
        restart();
    }
}

void LogManager::uploadLogs()
{
    if (m_uploadAfter == std::chrono::hours(0))
    {
        LOG(INFO) << "Upload after is set to 0, not uploading logs";
        return;
    }

    if (!m_logUploader)
    {
        LOG(WARN) << "No log uploader, not attempting upload of logs.";
        return;
    }

    LOG(INFO) << "Start uploading log files";

    std::vector<std::string> logs = getLogsToUpload();

    for (auto& log : logs)
    {
        LOG(INFO) << "Uploading '" << log << "'";
        if (!m_logUploader->upload(log))
        {
            LOG(WARN) << "Failed to upload '" << log << "'";
        }
    }

    LOG(INFO) << "Ended uploading log files";
}

void LogManager::deleteOldLogs()
{
    if (m_deleteAfter == std::chrono::hours(0))
    {
        LOG(INFO) << "Delete after is set to 0, not deleting logs";
        return;
    }

    LOG(INFO) << "Starting to delete stale log files";

    std::vector<std::string> oldLogs = getLogsToDelete();

    for (auto& oldLog : oldLogs)
    {
        LOG(INFO) << "Deleting '" << oldLog << "'";
        if (!wolkabout::FileSystemUtils::deleteFile(oldLog))
        {
            LOG(WARN) << "Failed to delete log '" << oldLog << "'";
        }
    }
    LOG(INFO) << "Ended deleting stale log files";
}

}    // namespace wolkabout

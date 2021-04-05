/*
 * Copyright (c) 2021 WolkAbout.
 */

#ifndef LOGMANAGER_H
#define LOGMANAGER_H

#include "LogUploader.h"
#include "Timer.h"

#include <atomic>
#include <chrono>
#include <memory>
#include <string>
#include <vector>

namespace wolkabout
{
class LogManager
{
public:
    explicit LogManager(const std::string& logDirectory, const std::string& logExtension, const int& maxSize,
                        const std::chrono::hours& deleteEvery = std::chrono::hours(0),
                        const std::chrono::hours& deleteAfter = std::chrono::hours(0),
                        const std::chrono::hours& uploadEvery = std::chrono::hours(0),
                        const std::chrono::hours& uploadAfter = std::chrono::hours(0),
                        std::shared_ptr<LogUploader> logUploader = nullptr);

    ~LogManager();

    bool isRunning();
    void start();
    void stop();
    void restart();

    const std::string& getLogDirectory() const;
    void setLogDirectory(const std::string& logDirectory);
    int getMaxSize() const;
    void setMaxSize(int maxSize);
    const std::chrono::hours& getUploadEvery() const;
    void setUploadEvery(const std::chrono::hours& uploadEvery);
    const std::chrono::hours& getDeleteAfter() const;
    void setDeleteAfter(const std::chrono::hours& deleteAfter);
    const std::string& getLogExtension() const;
    void setLogExtension(const std::string& logExtension);
    const std::chrono::hours& getUploadAfter() const;
    void setUploadAfter(const std::chrono::hours& uploadAfter);
    void uploadLogs();
    void deleteOldLogs();
    const std::chrono::hours& getDeleteEvery() const;
    void setDeleteEvery(const std::chrono::hours& deleteEvery);

private:
    std::vector<std::string> getLogsToUpload();
    std::vector<std::string> getLogsToDelete();
    std::vector<std::string> getLogFiles();

    wolkabout::Timer m_uploadTimer;
    wolkabout::Timer m_deleteTimer;
    std::chrono::hours m_uploadEvery;
    std::chrono::hours m_uploadAfter;
    std::chrono::hours m_deleteEvery;
    std::chrono::hours m_deleteAfter;
    std::atomic_bool m_running;
    std::shared_ptr<LogUploader> m_logUploader = nullptr;
    std::string m_logDirectory;
    std::string m_logExtension;
    int m_maxSize;
};

}    // namespace wolkabout
#endif    // LOGMANAGER_H

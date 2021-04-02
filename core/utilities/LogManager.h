/*
 * Copyright (c) 2021 WolkAbout.
 */

#ifndef LOGMANAGER_H
#define LOGMANAGER_H

#include "LogUploader.h"

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
                        const std::chrono::hours& uploadEvery = std::chrono::hours(0),
                        const std::chrono::hours& deleteAfter = std::chrono::hours(0),
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

private:
    std::vector<std::string> getLogsToUpload();
    std::vector<std::string> getLogsToDelete();

    std::string m_logDirectory;
    std::string m_logExtension;

public:
    void setLogExtension(const std::string& logExtension);

private:
    int m_maxSize;
    std::chrono::hours m_uploadEvery;
    std::chrono::hours m_deleteAfter;
    std::atomic_bool m_running;
    std::shared_ptr<LogUploader> m_logUploader = nullptr;
};

}    // namespace wolkabout
#endif    // LOGMANAGER_H

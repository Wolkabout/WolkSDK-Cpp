/**
 * Copyright (c) 2022 Wolkabout.
 */

#ifndef LOGMANAGER_H
#define LOGMANAGER_H

#include "core/utilities/LogUploader.h"
#include "core/utilities/Timer.h"

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
                        const std::chrono::hours& deleteStaleAfter = std::chrono::hours(0),
                        const std::chrono::hours& uploadEvery = std::chrono::hours(0),
                        const std::chrono::hours& uploadAfter = std::chrono::hours(0),
                        std::shared_ptr<LogUploader> logUploader = nullptr);

    ~LogManager();

    bool isRunning();
    void start();
    void stop();
    void restart();

    void uploadLogs();
    void deleteOldLogs();
    void checkLogOverflow();

    const std::string& getLogDirectory() const;
    void setLogDirectory(const std::string& logDirectory);
    int getMaxSize() const;
    void setMaxSize(int maxSize);

    /**
     * Default getter for the Log Uploader of this Log Manager.
     *
     * @return Shared pointer to the log uploader.
     */
    std::shared_ptr<LogUploader> getLogUploader() const;

    const std::chrono::hours& getUploadEvery() const;
    void setUploadEvery(const std::chrono::hours& uploadEvery);
    const std::chrono::hours& getUploadAfter() const;
    void setUploadAfter(const std::chrono::hours& uploadAfter);
    const std::chrono::hours& getDeleteEvery() const;
    void setDeleteEvery(const std::chrono::hours& deleteEvery);
    const std::chrono::hours& getDeleteAfter() const;
    void setDeleteAfter(const std::chrono::hours& deleteAfter);
    const std::string& getLogExtension() const;
    void setLogExtension(const std::string& logExtension);
    void setLogUploader(const std::shared_ptr<wolkabout::LogUploader>& logUploader);

private:
    std::vector<std::string> getLogsToUpload();
    std::vector<std::string> getLogsToDelete();
    std::vector<std::string> getLogFileNames();
    double getTotalLogSize();

    std::string m_logDirectory;
    std::string m_logExtension;
    int m_maxSize;

    wolkabout::Timer m_overflowTimer;
    wolkabout::Timer m_deleteTimer;
    wolkabout::Timer m_uploadTimer;
    std::atomic_bool m_running;

    std::chrono::hours m_deleteEvery;
    std::chrono::hours m_deleteAfter;
    std::chrono::hours m_uploadEvery;
    std::chrono::hours m_uploadAfter;

    std::shared_ptr<LogUploader> m_logUploader = nullptr;
};

}    // namespace wolkabout
#endif    // LOGMANAGER_H

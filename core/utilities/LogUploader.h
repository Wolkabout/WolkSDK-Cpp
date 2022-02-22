/**
 * Copyright (c) 2022 Wolkabout
 */
#ifndef LOGUPLOADER_H
#define LOGUPLOADER_H

#include <string>
#include <vector>

namespace wolkabout
{
/**
 * This is an abstract class that is meant for providing means of uploading log files to a remote destination.
 */
class LogUploader
{
public:
    virtual ~LogUploader() = default;

    /**
     * @brief Attempt to upload a log file to a remote server
     * @param pathToLogFile Path where the log file is located on the file system
     * @return Success of the operation
     */
    virtual bool upload(const std::string& pathToLogFile) = 0;

    /**
     * @brief Get all the name of log files that are present on the remote server
     * @return Vector of file names as string
     */
    virtual std::vector<std::string> getRemoteLogs() = 0;
};
}    // namespace wolkabout

#endif    // LOGUPLOADER_H

/*
 * Copyright (c) 2021 WolkAbout
 */
#ifndef AWSLOGUPLOADER_H
#define AWSLOGUPLOADER_H

#include "core/utilities/LogUploader.h"

#include <aws/core/utils/memory/stl/AWSString.h>

namespace wolkabout
{
class AwsLogUploader : public wolkabout::LogUploader
{
public:
    AwsLogUploader(const std::string& bucketName, const std::string& region);

    /**
     * The default getter for the bucket name this uploader is connected to.
     *
     * @return The bucket name as string.
     */
    std::string getBucketName();

    /**
     * The default getter for the region this uploader is connected to.
     *
     * @return The region name as string.
     */
    std::string getRegion();

    /**
     * @brief Attempt to upload a log file to a remote server
     * @param pathToLogFile Path where the log file is located on the file system
     * @return Success of the operation
     */
    bool upload(const std::string& pathToLogFile) override;

    /**
     * @brief Get all the name of log files that are present on the remote server
     * @return Vector of file names as string
     */
    std::vector<std::string> getRemoteLogs() override;

private:
    Aws::String m_bucketName;
    Aws::String m_region;
};

}    // namespace wolkabout

#endif    // AWSLOGUPLOADER_H

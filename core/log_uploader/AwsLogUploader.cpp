/*
 * Copyright (c) 2021 WolkAbout
 */

#include "AwsLogUploader.h"

#include "utilities/Logger.h"

#include <algorithm>
#include <fstream>

namespace wolkabout
{
AwsLogUploader::AwsLogUploader(const std::string& bucketName, const std::string& region)
{
    m_bucketName = Aws::String(bucketName.c_str(), bucketName.size());
    m_region = Aws::String(region.c_str(), region.size());
}

bool wolkabout::AwsLogUploader::upload(const std::string& pathToLogFile)
{
    Aws::String objectName(pathToLogFile.c_str(), pathToLogFile.size());

    // Verify that the file exists.
    struct stat buffer
    {
    };

    if (stat(objectName.c_str(), &buffer) == -1)
    {
        LOG(ERROR) << "File '" << objectName << "' does not exist.";
        return false;
    }

    std::vector<std::string> remoteLogs = getRemoteLogs();
    const auto& it = std::find(remoteLogs.cbegin(), remoteLogs.cend(), pathToLogFile);
    if (it != remoteLogs.cend())
    {
        LOG(WARN) << "File '" << objectName << "' already present in bucket.";
        return true;
    }

    Aws::SDKOptions options;
    Aws::InitAPI(options);

    Aws::Client::ClientConfiguration config;

    if (!m_region.empty())
    {
        config.region = m_region;
    }

    Aws::S3::S3Client s3_client(config);

    Aws::S3::Model::PutObjectRequest request;
    request.SetBucket(m_bucketName);
    request.SetKey(objectName);

    std::shared_ptr<Aws::IOStream> input_data = Aws::MakeShared<Aws::FStream>(
      "SampleAllocationTag", objectName.c_str(), std::ios_base::in | std::ios_base::binary);

    request.SetBody(input_data);

    Aws::S3::Model::PutObjectOutcome outcome = s3_client.PutObject(request);

    if (outcome.IsSuccess())
    {
        LOG(INFO) << "Added object '" << objectName << "' to bucket '" << m_bucketName << "'.";
        Aws::ShutdownAPI(options);
        return true;
    }
    else
    {
        LOG(ERROR) << "Add object failed, outcome was: " << outcome.GetError().GetMessage();
        Aws::ShutdownAPI(options);
        return false;
    }
}

std::vector<std::string> wolkabout::AwsLogUploader::getRemoteLogs()
{
    std::vector<std::string> remoteLogs;

    Aws::SDKOptions options;
    options.loggingOptions.logLevel = Aws::Utils::Logging::LogLevel::Debug;
    Aws::InitAPI(options);
    {
        Aws::Client::ClientConfiguration config;
        if (!m_region.empty())
        {
            config.region = m_region;
        }
        Aws::S3::S3Client s3_client(config);
        Aws::S3::Model::ListObjectsRequest request;
        request.WithBucket(m_bucketName);

        auto outcome = s3_client.ListObjects(request);
        if (outcome.IsSuccess())
        {
            LOG(DEBUG) << "Objects in bucket '" << m_bucketName << "':";
            Aws::Vector<Aws::S3::Model::Object> objects = outcome.GetResult().GetContents();
            for (Aws::S3::Model::Object& object : objects)
            {
                LOG(DEBUG) << object.GetKey();
                remoteLogs.emplace_back(std::string(object.GetKey().c_str(), object.GetKey().size()));
            }
        }
        else
        {
            LOG(ERROR) << "Failed to get bucket content: " << outcome.GetError().GetMessage();
        }
    }
    Aws::ShutdownAPI(options);

    return remoteLogs;
}

}    // namespace wolkabout

/**
 * Copyright 2023 Wolkabout Technology s.r.o
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *      http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */

#ifndef WOLKABOUTCORE_LOGUPLOADER_H
#define WOLKABOUTCORE_LOGUPLOADER_H

#include <string>
#include <vector>

namespace wolkabout::legacy
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
}    // namespace wolkabout::legacy

#endif    // WOLKABOUTCORE_LOGUPLOADER_H

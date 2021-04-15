/*
 * Copyright 2019 WolkAbout Technology s.r.o.
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

#ifndef FILEURLDOWNLOADSTATUS_H
#define FILEURLDOWNLOADSTATUS_H

#include "core/model/FileTransferStatus.h"
#include "core/model/WolkOptional.h"

#include <string>

namespace wolkabout
{
class FileUrlDownloadStatus
{
public:
    FileUrlDownloadStatus(std::string fileUrl, FileTransferStatus status);
    /**
     * Status is ERROR
     * @param fileUrl
     * @param errorCode
     */
    FileUrlDownloadStatus(std::string fileUrl, FileTransferError errorCode);

    /**
     * Status is FILE_READY
     * @param fileUrl
     * @param fileName
     */
    FileUrlDownloadStatus(std::string fileUrl, std::string fileName);

    const std::string& getUrl() const;
    FileTransferStatus getStatus() const;
    const WolkOptional<FileTransferError>& getErrorCode() const;
    const WolkOptional<std::string>& getFileName() const;

private:
    std::string m_fileUrl;
    FileTransferStatus m_status;
    WolkOptional<FileTransferError> m_errorCode;
    WolkOptional<std::string> m_fileName;
};
}    // namespace wolkabout

#endif    // FILEURLDOWNLOADSTATUS_H

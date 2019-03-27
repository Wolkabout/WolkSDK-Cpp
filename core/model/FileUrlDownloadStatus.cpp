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

#include "model/FileUrlDownloadStatus.h"

#include <utility>

namespace wolkabout
{
FileUrlDownloadStatus::FileUrlDownloadStatus(std::string fileUrl, FileTransferStatus status)
: m_fileUrl{std::move(fileUrl)}, m_status{status}, m_errorCode{}, m_fileName{}
{
}

FileUrlDownloadStatus::FileUrlDownloadStatus(std::string fileUrl, FileTransferError errorCode)
: m_fileUrl{std::move(fileUrl)}, m_status{FileTransferStatus::ERROR}, m_errorCode{errorCode}, m_fileName{}
{
}

FileUrlDownloadStatus::FileUrlDownloadStatus(std::string fileUrl, std::string fileName)
: m_fileUrl{std::move(fileUrl)}, m_status{FileTransferStatus::FILE_READY}, m_errorCode{}, m_fileName{fileName}
{
}

const std::string& FileUrlDownloadStatus::getUrl() const
{
    return m_fileUrl;
}

FileTransferStatus FileUrlDownloadStatus::getStatus() const
{
    return m_status;
}

const WolkOptional<FileTransferError>& FileUrlDownloadStatus::getErrorCode() const
{
    return m_errorCode;
}

const WolkOptional<std::string>& FileUrlDownloadStatus::getFileName() const
{
    return m_fileName;
}
}    // namespace wolkabout

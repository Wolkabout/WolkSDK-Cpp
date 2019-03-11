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

#include "FileUploadStatus.h"

namespace wolkabout
{
FileUploadStatus::FileUploadStatus(std::string fileName, FileTransferStatus status)
: m_fileName{fileName}, m_status{status}, m_errorCode{}
{
}

FileUploadStatus::FileUploadStatus(std::string fileName, FileTransferError errorCode)
: m_fileName{fileName}, m_status{FileTransferStatus::ERROR}, m_errorCode{errorCode}
{
}

const std::string& FileUploadStatus::getFileName() const
{
    return m_fileName;
}

FileTransferStatus FileUploadStatus::getStatus() const
{
    return m_status;
}

const WolkOptional<FileTransferError>& FileUploadStatus::getErrorCode() const
{
    return m_errorCode;
}
}    // namespace wolkabout

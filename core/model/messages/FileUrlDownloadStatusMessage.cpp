/**
 * Copyright 2022 Wolkabout Technology s.r.o.
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

#include "core/model/messages/FileUrlDownloadStatusMessage.h"

namespace wolkabout
{
FileUrlDownloadStatusMessage::FileUrlDownloadStatusMessage(std::string fileUrl, std::string fileName,
                                                           FileTransferStatus status, FileTransferError error)
: m_fileUrl(std::move(fileUrl)), m_fileName(std::move(fileName)), m_status(status), m_error(error)
{
}

FileUrlDownloadStatusMessage::FileUrlDownloadStatusMessage(std::string fileUrl, FileTransferError error)
: m_fileUrl(std::move(fileUrl)), m_status(FileTransferStatus::ERROR), m_error(error)
{
}

const std::string& FileUrlDownloadStatusMessage::getFileUrl() const
{
    return m_fileUrl;
}

const std::string& FileUrlDownloadStatusMessage::getFileName() const
{
    return m_fileName;
}

FileTransferStatus FileUrlDownloadStatusMessage::getStatus() const
{
    return m_status;
}

FileTransferError FileUrlDownloadStatusMessage::getError() const
{
    return m_error;
}

MessageType FileUrlDownloadStatusMessage::getMessageType() const
{
    return MessageType::FILE_URL_DOWNLOAD_STATUS;
}
}    // namespace wolkabout

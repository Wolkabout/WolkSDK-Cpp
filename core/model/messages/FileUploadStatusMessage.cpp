/**
 * Copyright 2021 Wolkabout s.r.o.
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

#include "core/model/messages/FileUploadStatusMessage.h"

namespace wolkabout
{
FileUploadStatusMessage::FileUploadStatusMessage(std::string name, FileTransferStatus status, FileTransferError error)
: m_name(std::move(name)), m_status(status), m_error(error)
{
}

FileUploadStatusMessage::FileUploadStatusMessage(std::string name, FileTransferError error)
: m_name(std::move(name)), m_status(FileTransferStatus::ERROR), m_error(error)
{
}

const std::string& FileUploadStatusMessage::getName() const
{
    return m_name;
}

FileTransferStatus FileUploadStatusMessage::getStatus() const
{
    return m_status;
}

FileTransferError FileUploadStatusMessage::getError() const
{
    return m_error;
}

MessageType FileUploadStatusMessage::getMessageType() const
{
    return MessageType::FILE_UPLOAD_STATUS;
}
}    // namespace wolkabout

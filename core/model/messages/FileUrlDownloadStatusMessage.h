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

#ifndef WOLKABOUTCORE_FILEURLDOWNLOADSTATUSMESSAGE_H
#define WOLKABOUTCORE_FILEURLDOWNLOADSTATUSMESSAGE_H

#include "core/model/messages/MessageModel.h"

namespace wolkabout
{
class FileUrlDownloadStatusMessage : public MessageModel
{
public:
    FileUrlDownloadStatusMessage(std::string fileUrl, std::string fileName, FileTransferStatus status,
                                 FileTransferError error = FileTransferError::NONE);

    FileUrlDownloadStatusMessage(std::string fileUrl, FileTransferError error);

    const std::string& getFileUrl() const;

    const std::string& getFileName() const;

    FileTransferStatus getStatus() const;

    FileTransferError getError() const;

    MessageType getMessageType() const override;

private:
    std::string m_fileUrl;
    std::string m_fileName;
    FileTransferStatus m_status;
    FileTransferError m_error;
};
}    // namespace wolkabout

#endif    // WOLKABOUTCORE_FILEURLDOWNLOADSTATUSMESSAGE_H

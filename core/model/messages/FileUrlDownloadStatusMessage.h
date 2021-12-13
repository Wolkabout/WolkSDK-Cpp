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

#ifndef WOLKABOUTCORE_FILEURLDOWNLOADSTATUSMESSAGE_H
#define WOLKABOUTCORE_FILEURLDOWNLOADSTATUSMESSAGE_H

#include "core/model/messages/MessageModel.h"

namespace wolkabout
{
class FileUrlDownloadStatusMessage : public MessageModel
{
public:
    explicit FileUrlDownloadStatusMessage(std::string fileUrl, std::string fileName, FileUploadStatus status,
                                          FileUploadError error = FileUploadError::NONE);

    const std::string& getFileUrl() const;

    const std::string& getFileName() const;

    FileUploadStatus getStatus() const;

    FileUploadError getError() const;

    MessageType getMessageType() const override;

private:
    std::string m_fileUrl;
    std::string m_fileName;
    FileUploadStatus m_status;
    FileUploadError m_error;
};
}    // namespace wolkabout

#endif    // WOLKABOUTCORE_FILEURLDOWNLOADSTATUSMESSAGE_H

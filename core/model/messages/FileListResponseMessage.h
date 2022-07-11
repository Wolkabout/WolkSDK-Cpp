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

#ifndef WOLKABOUTCORE_FILELISTRESPONSEMESSAGE_H
#define WOLKABOUTCORE_FILELISTRESPONSEMESSAGE_H

#include "core/model/messages/MessageModel.h"

namespace wolkabout
{
class FileListResponseMessage : public MessageModel
{
public:
    explicit FileListResponseMessage(std::vector<FileInformation> files);

    const std::vector<FileInformation>& getFiles() const;

    MessageType getMessageType() const override;

private:
    std::vector<FileInformation> m_files;
};
}    // namespace wolkabout

#endif    // WOLKABOUTCORE_FILELISTRESPONSEMESSAGE_H

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

#ifndef WOLKABOUTCORE_FILEURLDOWNLOADABORTMESSAGE_H
#define WOLKABOUTCORE_FILEURLDOWNLOADABORTMESSAGE_H

#include "core/model/messages/MessageObject.h"

namespace wolkabout
{
class FileUrlDownloadAbortMessage : public MessageObject
{
public:
    explicit FileUrlDownloadAbortMessage(std::string path);

    const std::string& getPath() const;

    MessageType getMessageType() override;

private:
    std::string m_path;
};
}    // namespace wolkabout

#endif    // WOLKABOUTCORE_FILEURLDOWNLOADABORTMESSAGE_H

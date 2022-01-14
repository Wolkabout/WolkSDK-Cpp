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

#ifndef WOLKABOUTCORE_FILEBINARYRESPONSEMESSAGE_H
#define WOLKABOUTCORE_FILEBINARYRESPONSEMESSAGE_H

#include "core/model/messages/MessageModel.h"

namespace wolkabout
{
class FileBinaryResponseMessage : public MessageModel
{
public:
    explicit FileBinaryResponseMessage(const std::string& payload);

    const std::string& getPreviousHash() const;

    const std::vector<std::uint8_t>& getData() const;

    const std::string& getCurrentHash() const;

    MessageType getMessageType() const override;

private:
    std::string m_previousHash;
    std::vector<std::uint8_t> m_data;
    std::string m_currentHash;
};
}    // namespace wolkabout

#endif    // WOLKABOUTCORE_FILEBINARYRESPONSEMESSAGE_H

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

#include "core/model/messages/FileBinaryResponseMessage.h"

#include "core/utilities/ByteUtils.h"

namespace wolkabout
{
FileBinaryResponseMessage::FileBinaryResponseMessage(const std::string& payload)
{
    if (payload.length() > 64)
    {
        m_previousHash = payload.substr(0, 64);
        m_data = ByteUtils::toByteArray(payload.substr(64, payload.length() - 128));
        m_currentHash = payload.substr(payload.length() - 64);
    }
}

const std::string& FileBinaryResponseMessage::getPreviousHash() const
{
    return m_previousHash;
}

const std::vector<std::uint8_t>& FileBinaryResponseMessage::getData() const
{
    return m_data;
}

const std::string& FileBinaryResponseMessage::getCurrentHash() const
{
    return m_currentHash;
}

MessageType FileBinaryResponseMessage::getMessageType()
{
    return MessageType::FILE_BINARY_RESPONSE;
}
}    // namespace wolkabout

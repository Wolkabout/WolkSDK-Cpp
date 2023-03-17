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

#include "core/model/messages/FileBinaryResponseMessage.h"

#include "core/utilities/ByteUtils.h"

namespace wolkabout
{
FileBinaryResponseMessage::FileBinaryResponseMessage(const std::string& payload)
{
    if (payload.length() > 64)
    {
        m_previousHash = payload.substr(0, 32);
		m_data = legacy::ByteUtils::toByteArray(payload.substr(32, payload.length() - 64));
        m_currentHash = payload.substr(payload.length() - 32);
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

MessageType FileBinaryResponseMessage::getMessageType() const
{
    return MessageType::FILE_BINARY_RESPONSE;
}
}    // namespace wolkabout

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

#include "core/model/messages/FileBinaryRequestMessage.h"

namespace wolkabout
{
FileBinaryRequestMessage::FileBinaryRequestMessage(std::string name, std::uint64_t chunkIndex)
: m_name(std::move(name)), m_chunkIndex(chunkIndex)
{
}

const std::string& FileBinaryRequestMessage::getName() const
{
    return m_name;
}

std::uint64_t FileBinaryRequestMessage::getChunkIndex() const
{
    return m_chunkIndex;
}

MessageType FileBinaryRequestMessage::getMessageType() const
{
    return MessageType::FILE_BINARY_REQUEST;
}
}    // namespace wolkabout
/**
 * Copyright 2023 Wolkabout Technology s.r.o.
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

#include "core/model/messages/FileUploadInitiateMessage.h"

namespace wolkabout
{
FileUploadInitiateMessage::FileUploadInitiateMessage(std::string name, std::uint64_t size, std::string hash)
: m_name(std::move(name)), m_size(size), m_hash(std::move(hash))
{
}

const std::string& FileUploadInitiateMessage::getName() const
{
    return m_name;
}

uint64_t FileUploadInitiateMessage::getSize() const
{
    return m_size;
}

const std::string& FileUploadInitiateMessage::getHash() const
{
    return m_hash;
}

MessageType FileUploadInitiateMessage::getMessageType() const
{
    return MessageType::FILE_UPLOAD_INIT;
}
}    // namespace wolkabout

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

#include "core/model/messages/FirmwareUpdateInstallMessage.h"

namespace wolkabout
{
FirmwareUpdateInstallMessage::FirmwareUpdateInstallMessage(std::string file) : m_file(std::move(file)) {}

const std::string& FirmwareUpdateInstallMessage::getFile() const
{
    return m_file;
}

MessageType FirmwareUpdateInstallMessage::getMessageType() const
{
    return MessageType::FIRMWARE_UPDATE_INSTALL;
}
}    // namespace wolkabout

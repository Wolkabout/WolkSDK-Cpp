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

#include "core/model/messages/RegisteredDevicesRequestMessage.h"

#include <utility>

namespace wolkabout
{
RegisteredDevicesRequestMessage::RegisteredDevicesRequestMessage(std::chrono::milliseconds timestampFrom,
                                                                 std::string deviceType, std::string externalId)
: m_timestampFrom(timestampFrom), m_deviceType(std::move(deviceType)), m_externalId(std::move(externalId))
{
}

std::chrono::milliseconds RegisteredDevicesRequestMessage::getTimestampFrom() const
{
    return m_timestampFrom;
}

const std::string& RegisteredDevicesRequestMessage::getDeviceType() const
{
    return m_deviceType;
}

const std::string& RegisteredDevicesRequestMessage::getExternalId() const
{
    return m_externalId;
}

MessageType RegisteredDevicesRequestMessage::getMessageType() const
{
    return MessageType::REGISTERED_DEVICES_REQUEST;
}
}    // namespace wolkabout

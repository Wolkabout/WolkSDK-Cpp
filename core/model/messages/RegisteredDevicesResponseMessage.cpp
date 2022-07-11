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

#include "core/model/messages/RegisteredDevicesResponseMessage.h"

#include <utility>

namespace wolkabout
{
RegisteredDeviceInformation::RegisteredDeviceInformation(std::string key, std::string id, std::string type)
: deviceKey(std::move(key)), externalId(std::move(id)), deviceType(std::move(type))
{
}

RegisteredDevicesResponseMessage::RegisteredDevicesResponseMessage(
  std::chrono::milliseconds timestampFrom, std::string deviceType, std::string externalId,
  std::vector<RegisteredDeviceInformation> matchingDevices)
: m_timestampFrom(timestampFrom)
, m_deviceType(std::move(deviceType))
, m_externalId(std::move(externalId))
, m_matchingDevices(std::move(matchingDevices))
{
}

std::chrono::milliseconds RegisteredDevicesResponseMessage::getTimestampFrom() const
{
    return m_timestampFrom;
}

const std::string& RegisteredDevicesResponseMessage::getDeviceType() const
{
    return m_deviceType;
}

const std::string& RegisteredDevicesResponseMessage::getExternalId() const
{
    return m_externalId;
}

const std::vector<RegisteredDeviceInformation>& RegisteredDevicesResponseMessage::getMatchingDevices() const
{
    return m_matchingDevices;
}

MessageType RegisteredDevicesResponseMessage::getMessageType() const
{
    return MessageType::REGISTERED_DEVICES_RESPONSE;
}
}    // namespace wolkabout

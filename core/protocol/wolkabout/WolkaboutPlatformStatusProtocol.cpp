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

#include "core/protocol/wolkabout/WolkaboutPlatformStatusProtocol.h"

#include "core/protocol/wolkabout/WolkaboutProtocol.h"
#include "core/utilities/Logger.h"

namespace wolkabout
{
std::vector<std::string> WolkaboutPlatformStatusProtocol::getInboundChannels() const
{
    return {WolkaboutProtocol::PLATFORM_TO_DEVICE_DIRECTION + WolkaboutProtocol::CHANNEL_DELIMITER +
            toString(MessageType::PLATFORM_CONNECTION_STATUS)};
}

std::vector<std::string> WolkaboutPlatformStatusProtocol::getInboundChannelsForDevice(
  const std::string& /** deviceKey */) const
{
    return {};
}

MessageType WolkaboutPlatformStatusProtocol::getMessageType(const Message& message)
{
    LOG(TRACE) << METHOD_INFO;
    return WolkaboutProtocol::getMessageType(message);
}

DeviceType WolkaboutPlatformStatusProtocol::getDeviceType(const Message& message)
{
    LOG(TRACE) << METHOD_INFO;
    return WolkaboutProtocol::getDeviceType(message);
}

std::string WolkaboutPlatformStatusProtocol::getDeviceKey(const Message& message) const
{
    LOG(TRACE) << METHOD_INFO;
    return WolkaboutProtocol::getDeviceKey(message);
}

std::unique_ptr<PlatformStatusMessage> WolkaboutPlatformStatusProtocol::parsePlatformStatusMessage(
  const std::shared_ptr<Message>& message)
{
    LOG(TRACE) << METHOD_INFO;
    const auto errorPrefix = "Failed to parse 'PlatformStatus' message";

    // Check if the message is a PlatformStatus message
    auto type = getMessageType(*message);
    if (type != MessageType::PLATFORM_CONNECTION_STATUS)
    {
        LOG(ERROR) << errorPrefix << " -> The message is not a 'PlatformStatus' message.";
        return nullptr;
    }

    // Check if the content can be parsed into a connection status
    auto connectionStatus = fromString(WolkaboutProtocol::removeQuotes(message->getContent()));
    if (connectionStatus == ConnectivityStatus::NONE)
    {
        LOG(ERROR) << errorPrefix << " -> The connection status value of the message is not valid.";
        return nullptr;
    }
    return std::unique_ptr<PlatformStatusMessage>(new PlatformStatusMessage(connectionStatus));
}
}    // namespace wolkabout

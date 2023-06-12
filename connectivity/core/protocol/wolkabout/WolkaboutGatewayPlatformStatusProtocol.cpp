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

#include "core/protocol/wolkabout/WolkaboutGatewayPlatformStatusProtocol.h"

#include "core/protocol/wolkabout/WolkaboutProtocol.h"
#include "core/utility/Logger.h"

using namespace wolkabout::legacy;

namespace wolkabout
{
std::vector<std::string> WolkaboutGatewayPlatformStatusProtocol::getInboundChannels() const
{
    return {};
}

std::vector<std::string> WolkaboutGatewayPlatformStatusProtocol::getInboundChannelsForDevice(const std::string&) const
{
    return {};
}

MessageType WolkaboutGatewayPlatformStatusProtocol::getMessageType(const Message& message)
{
    return WolkaboutProtocol::getMessageType(message);
}

DeviceType WolkaboutGatewayPlatformStatusProtocol::getDeviceType(const Message& message)
{
    return WolkaboutProtocol::getDeviceType(message);
}

std::string WolkaboutGatewayPlatformStatusProtocol::getDeviceKey(const Message& message) const
{
    return WolkaboutProtocol::getDeviceKey(message);
}

std::string WolkaboutGatewayPlatformStatusProtocol::getResponseChannelForMessage(MessageType type,
                                                                                 const std::string& deviceKey) const
{
    return {};
}

std::unique_ptr<Message> WolkaboutGatewayPlatformStatusProtocol::makeOutboundMessage(
  const std::string& /** deviceKey */, const PlatformStatusMessage& message)
{
    LOG(TRACE) << METHOD_INFO;

    // Check whether the status from the message parses into a valid value
    auto statusString = toString(message.getStatus());
    if (statusString.empty())
    {
        LOG(ERROR) << "Failed to serialize 'PlatformStatusMessage' -> The status value in the message is not valid.";
        return nullptr;
    }

    // Create the message
    return std::unique_ptr<Message>{new Message{statusString, WolkaboutProtocol::PLATFORM_TO_DEVICE_DIRECTION +
                                                                WolkaboutProtocol::CHANNEL_DELIMITER +
                                                                toString(MessageType::PLATFORM_CONNECTION_STATUS)}};
}
}    // namespace wolkabout

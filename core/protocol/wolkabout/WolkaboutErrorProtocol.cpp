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

#include "core/protocol/wolkabout/WolkaboutErrorProtocol.h"

#include "core/protocol/wolkabout/WolkaboutProtocol.h"
#include "core/utilities/Logger.h"

namespace wolkabout
{
WolkaboutErrorProtocol::WolkaboutErrorProtocol(bool isGateway)
: m_incomingDirection(isGateway ? WolkaboutProtocol::PLATFORM_TO_GATEWAY_DIRECTION :
                                  WolkaboutProtocol::PLATFORM_TO_DEVICE_DIRECTION)
{
}

std::vector<std::string> WolkaboutErrorProtocol::getInboundChannels() const
{
    return {};
}

std::vector<std::string> WolkaboutErrorProtocol::getInboundChannelsForDevice(const std::string& deviceKey) const
{
    return {m_incomingDirection + WolkaboutProtocol::CHANNEL_DELIMITER + deviceKey +
            WolkaboutProtocol::CHANNEL_DELIMITER + toString(MessageType::ERROR)};
}

MessageType WolkaboutErrorProtocol::getMessageType(const Message& message)
{
    LOG(TRACE) << METHOD_INFO;
    return WolkaboutProtocol::getMessageType(message);
}

DeviceType WolkaboutErrorProtocol::getDeviceType(const Message& message)
{
    LOG(TRACE) << METHOD_INFO;
    return WolkaboutProtocol::getDeviceType(message);
}

std::string WolkaboutErrorProtocol::getDeviceKey(const Message& message) const
{
    LOG(TRACE) << METHOD_INFO;
    return WolkaboutProtocol::getDeviceKey(message);
}

std::unique_ptr<ErrorMessage> WolkaboutErrorProtocol::parseError(const std::shared_ptr<Message>& message)
{
    LOG(TRACE) << METHOD_INFO;
    const auto errorPrefix = "Failed to parse 'Error' message";

    // Check that the message is a RegisteredDeviceResponse message.
    auto type = getMessageType(*message);
    if (type != MessageType::ERROR)
    {
        LOG(ERROR) << errorPrefix << " -> The message is not a 'Error' message.";
        return nullptr;
    }

    // Extract the extra data
    auto deviceKey = getDeviceKey(*message);
    if (deviceKey.empty())
    {
        LOG(ERROR) << errorPrefix << " -> The device key is empty.";
        return nullptr;
    }
    auto time = std::chrono::system_clock::now();

    // Make the message
    return std::unique_ptr<ErrorMessage>(new ErrorMessage(deviceKey, message->getContent(), time));
}
}    // namespace wolkabout
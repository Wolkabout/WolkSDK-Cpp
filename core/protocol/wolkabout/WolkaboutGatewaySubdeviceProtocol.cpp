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

#include "core/protocol/wolkabout/WolkaboutGatewaySubdeviceProtocol.h"

#include "core/protocol/wolkabout/WolkaboutProtocol.h"
#include "core/utilities/Logger.h"
#include "core/utilities/json.hpp"

using json = nlohmann::json;

namespace wolkabout
{
static void to_json(json& j, const GatewaySubdeviceMessage& message)
{
    j["device"] = WolkaboutProtocol::getDeviceKey(message.getMessage());
    j["payload"] = json::parse(message.getMessage().getContent());
}

std::vector<std::string> WolkaboutGatewaySubdeviceProtocol::getInboundChannels() const
{
    return {};
}

std::vector<std::string> WolkaboutGatewaySubdeviceProtocol::getInboundChannelsForDevice(
  const std::string& deviceKey) const
{
    return {WolkaboutProtocol::PLATFORM_TO_GATEWAY_DIRECTION + WolkaboutProtocol::CHANNEL_DELIMITER + deviceKey +
            WolkaboutProtocol::CHANNEL_DELIMITER + WolkaboutProtocol::WILDCARD_SINGLE_LEVEL};
}

MessageType WolkaboutGatewaySubdeviceProtocol::getMessageType(const Message& message)
{
    return WolkaboutProtocol::getMessageType(message);
}

DeviceType WolkaboutGatewaySubdeviceProtocol::getDeviceType(const Message& message)
{
    return WolkaboutProtocol::getDeviceType(message);
}

std::string WolkaboutGatewaySubdeviceProtocol::getDeviceKey(const Message& message) const
{
    return WolkaboutProtocol::getDeviceKey(message);
}

std::unique_ptr<Message> WolkaboutGatewaySubdeviceProtocol::makeOutboundMessage(const std::string& deviceKey,
                                                                                const GatewaySubdeviceMessage& message)
{
    LOG(TRACE) << METHOD_INFO;

    // Find the message type of the sub-message
    auto subMessageType = getMessageType(message.getMessage());
    if (subMessageType > MessageType::PLATFORM_CONNECTION_STATUS)
    {
        LOG(ERROR) << "The sub-message has a type that is invalid for a sub-device to send!";
        return nullptr;
    }

    // Create the message
    return std::unique_ptr<Message>{new Message{
      json{message}.dump(), WolkaboutProtocol::GATEWAY_TO_PLATFORM_DIRECTION + WolkaboutProtocol::CHANNEL_DELIMITER +
                              deviceKey + WolkaboutProtocol::CHANNEL_DELIMITER + toString(subMessageType)}};
}

std::vector<GatewaySubdeviceMessage> WolkaboutGatewaySubdeviceProtocol::parseIncomingSubdeviceMessage(
  std::shared_ptr<Message> message)
{
    LOG(TRACE) << METHOD_INFO;

    // Check the type of the received message
    auto type = getMessageType(*message);

    // Make place for all the messages
    auto messages = std::vector<GatewaySubdeviceMessage>{};

    // Obtain the payloads
    auto json = json::parse(message->getContent());
    for (const auto& pair : json.items())
    {
        // Get the value from the array
        const auto& value = pair.value();

        // Extract necessities to make a message
        auto payload = value["payload"];
        auto deviceKey = value["device"].get<std::string>();
        messages.emplace_back(Message{payload.dump(), WolkaboutProtocol::PLATFORM_TO_DEVICE_DIRECTION +
                                                        WolkaboutProtocol::CHANNEL_DELIMITER + deviceKey +
                                                        WolkaboutProtocol::CHANNEL_DELIMITER + toString(type)});
    }
    return messages;
}
}    // namespace wolkabout

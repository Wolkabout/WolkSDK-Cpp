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

#include "core/protocol/wolkabout/WolkaboutGatewaySubdeviceProtocol.h"

#include "core/protocol/wolkabout/WolkaboutProtocol.h"
#include "core/utilities/Logger.h"
#include "core/utilities/StringUtils.h"

#include <nlohmann/json.hpp>

using json = nlohmann::json;

namespace wolkabout
{
static void to_json(json& j, const GatewaySubdeviceMessage& message)
{
    if (!message.getMessage().getContent().empty())
    {
        j["device"] = WolkaboutProtocol::getDeviceKey(message.getMessage());
        j["payload"] = json::parse(message.getMessage().getContent());
    }
    else
    {
        j = WolkaboutProtocol::getDeviceKey(message.getMessage());
    }
}

WolkaboutGatewaySubdeviceProtocol::WolkaboutGatewaySubdeviceProtocol(bool isGateway)
: m_incomingDirection{isGateway ? WolkaboutProtocol::PLATFORM_TO_GATEWAY_DIRECTION :
                                  WolkaboutProtocol::DEVICE_TO_PLATFORM_DIRECTION}
{
}

std::vector<std::string> WolkaboutGatewaySubdeviceProtocol::getInboundChannels() const
{
    return {};
}

std::vector<std::string> WolkaboutGatewaySubdeviceProtocol::getInboundChannelsForDevice(
  const std::string& deviceKey) const
{
    return {m_incomingDirection + WolkaboutProtocol::CHANNEL_DELIMITER + deviceKey +
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

std::string WolkaboutGatewaySubdeviceProtocol::getResponseChannelForMessage(MessageType /* type */,
                                                                            const std::string& /* deviceKey */) const
{
    return {};
}

std::unique_ptr<Message> WolkaboutGatewaySubdeviceProtocol::makeOutboundMessage(const std::string& deviceKey,
                                                                                const GatewaySubdeviceMessage& message)
{
    LOG(TRACE) << METHOD_INFO;

    // Find the message type of the sub-message
    auto subMessageType = getMessageType(message.getMessage());
    if (subMessageType >= MessageType::DEVICE_REGISTRATION)
    {
        LOG(ERROR) << "Failed to generate outbound subdevice message -> The sub-message has a type that is invalid for "
                      "a subdevice to send!";
        return nullptr;
    }

    try
    {
        // Create the message
        return std::unique_ptr<Message>{
          new Message{json{message}.dump(), WolkaboutProtocol::GATEWAY_TO_PLATFORM_DIRECTION +
                                              WolkaboutProtocol::CHANNEL_DELIMITER + deviceKey +
                                              WolkaboutProtocol::CHANNEL_DELIMITER + toString(subMessageType)}};
    }
    catch (const std::exception& exception)
    {
        LOG(ERROR) << "Failed to generate outbound subdevice message -> '" << exception.what() << "'.";
        return nullptr;
    }
}

std::vector<GatewaySubdeviceMessage> WolkaboutGatewaySubdeviceProtocol::parseIncomingSubdeviceMessage(
  std::shared_ptr<Message> message)
{
    LOG(TRACE) << METHOD_INFO;

    // Check the type of the received message
    auto type = getMessageType(*message);
    if (type >= MessageType::DEVICE_REGISTRATION && type <= MessageType::REGISTERED_DEVICES_RESPONSE)
        return {GatewaySubdeviceMessage{*message}};

    // Make place for all the messages
    auto messages = std::vector<GatewaySubdeviceMessage>{};

    // Make the lambda expression that will analyze a payload object
    auto extractMessage = [&](const json& j) {
        // Extract necessities to make a message
        auto payload = j["payload"];
        auto deviceKey = j["device"].get<std::string>();

        // If the payload needs to be transformed, we're going to do it here
        auto payloadDump = payload.dump();
        if (type == MessageType::FILE_BINARY_RESPONSE)
            payloadDump = StringUtils::base64Decode(WolkaboutProtocol::removeQuotes(payloadDump));
        messages.emplace_back(Message{payloadDump, WolkaboutProtocol::PLATFORM_TO_DEVICE_DIRECTION +
                                                     WolkaboutProtocol::CHANNEL_DELIMITER + deviceKey +
                                                     WolkaboutProtocol::CHANNEL_DELIMITER + toString(type)});
    };

    try
    {
        // Obtain the payloads
        auto json = json::parse(message->getContent());
        if (json.is_object())
            extractMessage(json);
        else if (json.is_array())
            for (const auto& pair : json.items())
                extractMessage(pair.value());
        return messages;
    }
    catch (const std::exception& exception)
    {
        LOG(ERROR) << "Failed to parse incoming subdevice message -> '" << exception.what() << "'.";
        return messages;
    }
}
}    // namespace wolkabout

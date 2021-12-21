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

#include "core/protocol/wolkabout/WolkaboutRegistrationProtocol.h"

#include "core/protocol/wolkabout/WolkaboutProtocol.h"
#include "core/utilities/Logger.h"
#include "core/utilities/json.hpp"

using namespace nlohmann;

namespace wolkabout
{
static void from_json(const json& j, std::vector<RegisteredDeviceInformation>& r)
{
    // Check that the json is an array
    if (!j.is_array())
        throw std::runtime_error("Received payload is not an array.");

    // Now we need to go through every object in the array
    for (const auto& pair : j.items())
    {
        // Check that this json is an object
        const auto& device = pair.value();
        if (!device.is_object())
            throw std::runtime_error("One of array members is not an object.");

        // Obtain the values
        auto deviceKey = device["deviceKey"].get<std::string>();
        auto deviceType = device["deviceType"].get<std::string>();
        auto externalId = std::string{};
        if (device.find("externalId") != device.cend())
            externalId = device["externalId"].get<std::string>();

        // Put the information in the array
        r.emplace_back(deviceKey, externalId, deviceType);
    }
}

WolkaboutRegistrationProtocol::WolkaboutRegistrationProtocol(bool isGateway)
: m_isGateway(isGateway)
, m_outgoingDirection(m_isGateway ? WolkaboutProtocol::GATEWAY_TO_PLATFORM_DIRECTION :
                                    WolkaboutProtocol::DEVICE_TO_PLATFORM_DIRECTION)
, m_incomingDirection(m_isGateway ? WolkaboutProtocol::PLATFORM_TO_GATEWAY_DIRECTION :
                                    WolkaboutProtocol::PLATFORM_TO_DEVICE_DIRECTION)
{
}

std::vector<std::string> WolkaboutRegistrationProtocol::getInboundChannels() const
{
    return {};
}

std::vector<std::string> WolkaboutRegistrationProtocol::getInboundChannelsForDevice(const std::string& deviceKey) const
{
    return {m_incomingDirection + WolkaboutProtocol::CHANNEL_DELIMITER + deviceKey +
            WolkaboutProtocol::CHANNEL_DELIMITER + toString(MessageType::REGISTERED_DEVICES_RESPONSE)};
}

MessageType WolkaboutRegistrationProtocol::getMessageType(std::shared_ptr<Message> message)
{
    return WolkaboutProtocol::getMessageType(message);
}

std::string WolkaboutRegistrationProtocol::extractDeviceKeyFromChannel(const std::string& topic) const
{
    return WolkaboutProtocol::extractDeviceKeyFromChannel(topic);
}

std::unique_ptr<Message> WolkaboutRegistrationProtocol::makeOutboundMessage(const std::string& deviceKey,
                                                                            const DeviceRegistrationMessage& request)
{
    LOG(TRACE) << METHOD_INFO;
    const auto errorPrefix = "Failed to generate outbound 'DeviceRegistration' message";

    // Verify that the message is valid
    if (request.getDevices().empty())
    {
        LOG(ERROR) << errorPrefix << " -> The list of devices is empty.";
        return nullptr;
    }

    // Make the topic for the message
    const auto topic = m_outgoingDirection + WolkaboutProtocol::CHANNEL_DELIMITER + deviceKey +
                       WolkaboutProtocol::CHANNEL_DELIMITER + toString(MessageType::DEVICE_REGISTRATION);

    // Parse the message
    auto payload = json::array();
    for (const auto& device : request.getDevices())
    {
        // Check that the name is not empty!
        if (device.name.empty())
        {
            LOG(ERROR) << errorPrefix << " -> One of the devices has an empty name.";
            return nullptr;
        }

        // Make the json object for this device
        auto devicePayload = json{{"name", device.name},
                                  {"parameters", json::object()},
                                  {"feeds", json::array()},
                                  {"attributes", json::array()}};
        if (!device.key.empty())
            devicePayload["key"] = device.key;
        if (!device.guid.empty())
            devicePayload["guid"] = device.guid;
        for (const auto& parameter : device.parameters)
            devicePayload["parameters"][toString(parameter.first)] = parameter.second;
        for (const auto& feed : device.feeds)
            devicePayload["feeds"] += json{{"name", feed.second.name},
                                           {"type", feed.second.type == FeedType::IN_OUT ? "IN_OUT" : "IN"},
                                           {"unitGuid", feed.second.unit},
                                           {"reference", feed.second.reference}};
        for (const auto& attribute : device.attributes)
            devicePayload["attributes"] += json{{"name", attribute.second.name},
                                                {"dataType", toString(attribute.second.type)},
                                                {"value", attribute.second.value}};

        // Append the payload in the array
        payload += devicePayload;
    }
    return std::unique_ptr<Message>(new Message{payload.dump(), topic});
}

std::unique_ptr<Message> WolkaboutRegistrationProtocol::makeOutboundMessage(const std::string& deviceKey,
                                                                            const DeviceRemovalMessage& request)
{
    LOG(TRACE) << METHOD_INFO;
    const auto errorPrefix = "Failed to generate outbound 'DeviceRemoval' message";

    // Verify that the message is valid
    if (request.getKeys().empty())
    {
        LOG(ERROR) << errorPrefix << " -> The message is empty!";
        return nullptr;
    }

    // Create the topic for the message
    const auto topic = m_outgoingDirection + WolkaboutProtocol::CHANNEL_DELIMITER + deviceKey +
                       WolkaboutProtocol::CHANNEL_DELIMITER + toString(MessageType::DEVICE_REMOVAL);

    // Create the payload for the message
    auto payload = json::array();
    for (const auto& key : request.getKeys())
    {
        if (key.empty())
        {
            LOG(ERROR) << errorPrefix << " -> One of the keys in the array is empty!";
            return nullptr;
        }
        payload += key;
    }
    return std::unique_ptr<Message>(new Message(payload.dump(), topic));
}

std::unique_ptr<Message> WolkaboutRegistrationProtocol::makeOutboundMessage(
  const std::string& deviceKey, const RegisteredDevicesRequestMessage& request)
{
    LOG(TRACE) << METHOD_INFO;

    // Create the topic for the message
    const auto topic = m_outgoingDirection + WolkaboutProtocol::CHANNEL_DELIMITER + deviceKey +
                       WolkaboutProtocol::CHANNEL_DELIMITER + toString(MessageType::REGISTERED_DEVICES_REQUEST);
    auto payload = json{{"timestampFrom", request.getTimestampFrom().count()}};
    if (!request.getDeviceType().empty())
        payload["deviceType"] = request.getDeviceType();
    if (!request.getExternalId().empty())
        payload["externalId"] = request.getExternalId();
    return std::unique_ptr<Message>(new Message{payload.dump(), topic});
}

std::unique_ptr<RegisteredDevicesResponseMessage> WolkaboutRegistrationProtocol::parseRegisteredDevicesResponse(
  const std::shared_ptr<Message>& message)
{
    LOG(TRACE) << METHOD_INFO;
    const auto errorPrefix = "Failed to parse 'RegisteredDevicesResponse' message";

    // Check that the message is a RegisteredDeviceResponse message.
    auto type = getMessageType(message);
    if (type != MessageType::REGISTERED_DEVICES_RESPONSE)
    {
        LOG(ERROR) << errorPrefix << " -> The message is not a 'RegisteredDeviceResponse' message.";
        return nullptr;
    }

    try
    {
        // Parse the information
        auto j = json::parse(message->getContent());
        if (!j.is_object())
        {
            LOG(ERROR) << errorPrefix << " -> The payload is not a valid JSON object.";
            return nullptr;
        }

        auto timestampFrom = j["timestampFrom"].get<std::uint64_t>();
        auto deviceType = std::string{};
        if (j.find("deviceType") != j.end())
            deviceType = j["deviceType"].get<std::string>();
        auto externalId = std::string{};
        if (j.find("externalId") != j.end())
            externalId = j["externalId"].get<std::string>();
        auto matchingDevices = j["matchingDevices"].get<std::vector<RegisteredDeviceInformation>>();

        // Return the message
        return std::unique_ptr<RegisteredDevicesResponseMessage>(new RegisteredDevicesResponseMessage{
          std::chrono::milliseconds(timestampFrom), deviceType, externalId, std::move(matchingDevices)});
    }
    catch (const std::exception& exception)
    {
        LOG(ERROR) << errorPrefix << " -> An exception was thrown '" << exception.what() << "'.";
        return nullptr;
    }
}
}    // namespace wolkabout
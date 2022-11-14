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

#include "core/protocol/wolkabout/WolkaboutRegistrationProtocol.h"

#include "core/protocol/wolkabout/WolkaboutProtocol.h"
#include "core/utilities/Logger.h"

#include <nlohmann/json.hpp>

using namespace nlohmann;

namespace wolkabout
{
static void to_json(json& j, const std::vector<DeviceRegistrationData>& r)
{
    for (const auto& device : r)
    {
        // Check that the name is not empty!
        if (device.name.empty())
            throw std::runtime_error("One of the devices has an empty name.");

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
            devicePayload["feeds"] += json{{"name", feed.second.getName()},
                                           {"type", feed.second.getFeedType() == FeedType::IN_OUT ? "IN_OUT" : "IN"},
                                           {"unitGuid", feed.second.getUnit()},
                                           {"reference", feed.second.getReference()}};
        for (const auto& attribute : device.attributes)
            devicePayload["attributes"] += json{{"name", attribute.second.getName()},
                                                {"dataType", toString(attribute.second.getDataType())},
                                                {"value", attribute.second.getValue()}};

        // Append the payload in the array
        j.emplace_back(devicePayload);
    }
}

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
        auto deviceType = std::string{};
        const auto deviceTypeIt = device.find("deviceType");
        if (deviceTypeIt != device.cend() && deviceTypeIt->is_string())
            deviceType = deviceTypeIt->get<std::string>();
        auto externalId = std::string{};
        const auto externalIdIt = device.find("externalId");
        if (externalIdIt != device.cend() && externalIdIt->is_string())
            externalId = device["externalId"].get<std::string>();

        // Put the information in the array
        r.emplace_back(deviceKey, externalId, deviceType);
    }
}

WolkaboutRegistrationProtocol::WolkaboutRegistrationProtocol(bool isGateway)
: m_outgoingDirection(isGateway ? WolkaboutProtocol::GATEWAY_TO_PLATFORM_DIRECTION :
                                  WolkaboutProtocol::DEVICE_TO_PLATFORM_DIRECTION)
, m_incomingDirection(isGateway ? WolkaboutProtocol::PLATFORM_TO_GATEWAY_DIRECTION :
                                  WolkaboutProtocol::PLATFORM_TO_DEVICE_DIRECTION)
{
}

std::vector<std::string> WolkaboutRegistrationProtocol::getInboundChannels() const
{
    return {};
}

std::vector<std::string> WolkaboutRegistrationProtocol::getInboundChannelsForDevice(const std::string& deviceKey) const
{
    return {getChildrenSynchronizationTopic(deviceKey), getDeviceRegistrationResponseTopic(deviceKey),
            getRegisteredDevicesTopic(deviceKey)};
}

MessageType WolkaboutRegistrationProtocol::getMessageType(const Message& message)
{
    return WolkaboutProtocol::getMessageType(message);
}

DeviceType WolkaboutRegistrationProtocol::getDeviceType(const Message& message)
{
    return WolkaboutProtocol::getDeviceType(message);
}

std::string WolkaboutRegistrationProtocol::getDeviceKey(const Message& message) const
{
    return WolkaboutProtocol::getDeviceKey(message);
}

std::string WolkaboutRegistrationProtocol::getResponseChannelForMessage(MessageType type,
                                                                        const std::string& deviceKey) const
{
    switch (type)
    {
    case MessageType::CHILDREN_SYNCHRONIZATION_REQUEST:
        return getChildrenSynchronizationTopic(deviceKey);
    case MessageType::DEVICE_REGISTRATION:
        return getDeviceRegistrationResponseTopic(deviceKey);
    case MessageType::REGISTERED_DEVICES_REQUEST:
        return getRegisteredDevicesTopic(deviceKey);
    default:
        return "";
    }
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
    try
    {
        auto payload = json::array();
        payload = request.getDevices();
        return std::unique_ptr<Message>(new Message{payload.dump(), topic});
    }
    catch (const std::exception& exception)
    {
        LOG(ERROR) << errorPrefix << " -> " << exception.what();
        return nullptr;
    }
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
    for (const auto& key : request.getKeys())
    {
        if (key.empty())
        {
            LOG(ERROR) << errorPrefix << " -> One of the keys in the array is empty!";
            return nullptr;
        }
    }

    // Create the topic for the message
    const auto topic = m_outgoingDirection + WolkaboutProtocol::CHANNEL_DELIMITER + deviceKey +
                       WolkaboutProtocol::CHANNEL_DELIMITER + toString(MessageType::DEVICE_REMOVAL);

    try
    {
        // Create the payload for the message
        auto payload = json::array();
        payload = request.getKeys();
        return std::unique_ptr<Message>(new Message(payload.dump(), topic));
    }
    catch (const std::exception& exception)
    {
        LOG(ERROR) << errorPrefix << " -> '" << exception.what() << "'.";
        return nullptr;
    }
}

std::unique_ptr<Message> WolkaboutRegistrationProtocol::makeOutboundMessage(
  const std::string& deviceKey, const ChildrenSynchronizationRequestMessage& request)
{
    LOG(TRACE) << METHOD_INFO;

    // Create the topic for the message
    const auto topic = m_outgoingDirection + WolkaboutProtocol::CHANNEL_DELIMITER + deviceKey +
                       WolkaboutProtocol::CHANNEL_DELIMITER + toString(request.getMessageType());
    return std::unique_ptr<Message>(new Message{"", topic});
}

std::unique_ptr<Message> WolkaboutRegistrationProtocol::makeOutboundMessage(
  const std::string& deviceKey, const RegisteredDevicesRequestMessage& request)
{
    LOG(TRACE) << METHOD_INFO;

    // Create the topic for the message
    const auto topic = m_outgoingDirection + WolkaboutProtocol::CHANNEL_DELIMITER + deviceKey +
                       WolkaboutProtocol::CHANNEL_DELIMITER + toString(MessageType::REGISTERED_DEVICES_REQUEST);

    try
    {
        auto payload = json{{"timestampFrom", request.getTimestampFrom().count()}};
        if (!request.getDeviceType().empty())
            payload["deviceType"] = request.getDeviceType();
        if (!request.getExternalId().empty())
            payload["externalId"] = request.getExternalId();
        return std::unique_ptr<Message>(new Message{payload.dump(), topic});
    }
    catch (const std::exception& exception)
    {
        LOG(ERROR) << "Failed to generate outbound 'RegisteredDevicesRequest' message -> '" << exception.what() << "'.";
        return nullptr;
    }
}

std::unique_ptr<ChildrenSynchronizationResponseMessage>
WolkaboutRegistrationProtocol::parseChildrenSynchronizationResponse(const std::shared_ptr<Message>& message)
{
    LOG(TRACE) << METHOD_INFO;
    const auto errorPrefix = "Failed to parse 'ChildrenSynchronizationResponse' message";

    // Check that the message is a RegisteredDeviceResponse message.
    auto type = getMessageType(*message);
    if (type != MessageType::CHILDREN_SYNCHRONIZATION_RESPONSE)
    {
        LOG(ERROR) << errorPrefix << " -> The message is not a 'ChildrenSynchronizationResponse' message.";
        return nullptr;
    }

    try
    {
        WolkaboutProtocol::validateJSONPayload(*message);

        // Parse the information
        auto j = json::parse(message->getContent());
        auto children = j.get<std::vector<std::string>>();
        return std::unique_ptr<ChildrenSynchronizationResponseMessage>(
          new ChildrenSynchronizationResponseMessage{children});
    }
    catch (const std::exception& exception)
    {
        LOG(ERROR) << errorPrefix << " -> '" << exception.what() << "'.";
        return nullptr;
    }
}

std::unique_ptr<DeviceRegistrationResponseMessage> WolkaboutRegistrationProtocol::parseDeviceRegistrationResponse(
  const std::shared_ptr<Message>& message)
{
    LOG(TRACE) << METHOD_INFO;
    const auto errorPrefix = "Failed to parse 'DeviceRegistrationResponse' message";

    // Check that the message is a DeviceRegistrationResponse message.
    auto type = getMessageType(*message);
    if (type != MessageType::DEVICE_REGISTRATION_RESPONSE)
    {
        LOG(ERROR) << errorPrefix << " -> The message is not a 'DeviceRegistrationResponse' message.";
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

        // Take out the data
        const auto success = j["success"].get<std::vector<std::string>>();
        const auto failed = j["failed"].get<std::vector<std::string>>();

        // Return the message
        return std::unique_ptr<DeviceRegistrationResponseMessage>(
          new DeviceRegistrationResponseMessage{success, failed});
    }
    catch (const std::exception& exception)
    {
        LOG(ERROR) << errorPrefix << " -> An exception was thrown '" << exception.what() << "'.";
        return nullptr;
    }
}

std::unique_ptr<RegisteredDevicesResponseMessage> WolkaboutRegistrationProtocol::parseRegisteredDevicesResponse(
  const std::shared_ptr<Message>& message)
{
    LOG(TRACE) << METHOD_INFO;
    const auto errorPrefix = "Failed to parse 'RegisteredDevicesResponse' message";

    // Check that the message is a RegisteredDeviceResponse message.
    auto type = getMessageType(*message);
    if (type != MessageType::REGISTERED_DEVICES_RESPONSE)
    {
        LOG(ERROR) << errorPrefix << " -> The message is not a 'RegisteredDeviceResponse' message.";
        return nullptr;
    }

    try
    {
        WolkaboutProtocol::validateJSONPayload(*message);

        // Parse the information
        auto j = json::parse(message->getContent());

        auto timestampFrom = j["timestampFrom"].get<std::uint64_t>();
        auto deviceType = std::string{};
        const auto deviceTypeIt = j.find("deviceType");
        if (deviceTypeIt != j.end() && deviceTypeIt->is_string())
            deviceType = j["deviceType"].get<std::string>();
        auto externalId = std::string{};
        const auto externalIdIt = j.find("externalId");
        if (externalIdIt != j.end() && externalIdIt->is_string())
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

std::string WolkaboutRegistrationProtocol::getChildrenSynchronizationTopic(const std::string& deviceKey) const
{
    return m_incomingDirection + WolkaboutProtocol::CHANNEL_DELIMITER + deviceKey +
           WolkaboutProtocol::CHANNEL_DELIMITER + toString(MessageType::CHILDREN_SYNCHRONIZATION_RESPONSE);
}

std::string WolkaboutRegistrationProtocol::getDeviceRegistrationResponseTopic(const std::string& deviceKey) const
{
    return m_incomingDirection + WolkaboutProtocol::CHANNEL_DELIMITER + deviceKey +
           WolkaboutProtocol::CHANNEL_DELIMITER + toString(MessageType::DEVICE_REGISTRATION_RESPONSE);
}

std::string WolkaboutRegistrationProtocol::getRegisteredDevicesTopic(const std::string& deviceKey) const
{
    return m_incomingDirection + WolkaboutProtocol::CHANNEL_DELIMITER + deviceKey +
           WolkaboutProtocol::CHANNEL_DELIMITER + toString(MessageType::REGISTERED_DEVICES_RESPONSE);
}
}    // namespace wolkabout

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

#include "core/protocol/wolkabout/WolkaboutGatewayRegistrationProtocol.h"

#include "core/protocol/wolkabout/WolkaboutProtocol.h"
#include "core/utilities/Logger.h"
#include "core/utilities/json.hpp"

using namespace nlohmann;

namespace wolkabout
{
static void to_json(json& j, const RegisteredDevicesResponseMessage& message)
{
    j["timestampFrom"] = message.getTimestampFrom().count();
    if (!message.getDeviceType().empty())
        j["deviceType"] = message.getDeviceType();
    if (!message.getExternalId().empty())
        j["externalId"] = message.getExternalId();
    for (const auto& node : message.getMatchingDevices())
        j["matchingDevices"].emplace_back(node);
}

static void to_json(json& j, const RegisteredDeviceInformation& information)
{
    j["deviceKey"] = information.deviceKey;
    j["deviceType"] = information.deviceType;
    if (!information.externalId.empty())
        j["externalId"] = information.externalId;
}

template <class T> static T extractValueIfPresent(const json& j, const std::string& key)
{
    if (j.find(key) != j.cend())
        return j[key].get<T>();
    return {};
}

static void from_json(const json& j, std::vector<Feed>& feeds)
{
    if (!j.is_array())
        throw std::runtime_error("The field for reading feeds is not an array!");

    for (const auto& item : j.items())
    {
        const auto& value = item.value();

        // Check the values
        const auto name = value["name"].get<std::string>();
        if (name.empty())
            throw std::runtime_error("One of the names in the feeds is empty!");
        const auto reference = value["reference"].get<std::string>();
        if (reference.empty())
            throw std::runtime_error("One of the references in the feeds is empty!");
        const auto type = feedTypeFromString(value["type"].get<std::string>());
        if (type == FeedType::NONE)
            throw std::runtime_error("One of the feed types in the feeds is invalid!");
        const auto unitType = value["unitGuid"].get<std::string>();
        feeds.emplace_back(Feed{name, reference, type, unitType});
    }
}

static void from_json(const json& j, std::vector<Attribute>& attributes)
{
    if (!j.is_array())
        throw std::runtime_error("The field for reading attributes is not an array!");

    for (const auto& item : j.items())
    {
        const auto& value = item.value();

        // Check the values
        const auto name = value["name"].get<std::string>();
        if (name.empty())
            throw std::runtime_error("One of the names in the feeds is empty!");
        const auto type = dataTypeFromString(value["dataType"].get<std::string>());
        if (type == DataType::NONE)
            throw std::runtime_error("One of the feed types in the feeds is invalid!");
        const auto attributeValue = value["value"].get<std::string>();
        attributes.emplace_back(Attribute{name, type, attributeValue});
    }
}

static void from_json(const json& j, std::vector<Parameter>& parameters)
{
    if (!j.is_object())
        throw std::runtime_error("The field for reading parameters is not an object!");

    for (const auto& item : j.items())
    {
        // Parse the key into the ParameterName
        const auto& key = item.key();
        const auto& value = item.value();
        const auto parameterName = parameterNameFromString(key);
        if (parameterName == ParameterName::UNKNOWN)
            throw std::runtime_error("One of the keys is not a valid ParameterName.");

        // Put the parameter in the vector
        parameters.emplace_back(parameterName, value);
    }
}

static void from_json(const json& j, DeviceRegistrationData& data)
{
    data.name = j["name"].get<std::string>();
    data.key = extractValueIfPresent<std::string>(j, "key");
    data.guid = extractValueIfPresent<std::string>(j, "guid");
    for (const auto& parameter : j["parameters"].get<std::vector<Parameter>>())
        data.parameters.emplace(parameter.first, parameter.second);
    for (const auto& feed : j["feeds"].get<std::vector<Feed>>())
        data.feeds.emplace(feed.getReference(), feed);
    for (const auto& attribute : j["attributes"].get<std::vector<Attribute>>())
        data.attributes.emplace(attribute.getName(), attribute);
}

std::vector<std::string> WolkaboutGatewayRegistrationProtocol::getInboundChannels() const
{
    return {};
}

std::vector<std::string> WolkaboutGatewayRegistrationProtocol::getInboundChannelsForDevice(
  const std::string& deviceKey) const
{
    return {WolkaboutProtocol::DEVICE_TO_PLATFORM_DIRECTION + WolkaboutProtocol::CHANNEL_DELIMITER + deviceKey +
              WolkaboutProtocol::CHANNEL_DELIMITER + toString(MessageType::DEVICE_REGISTRATION),
            WolkaboutProtocol::DEVICE_TO_PLATFORM_DIRECTION + WolkaboutProtocol::CHANNEL_DELIMITER + deviceKey +
              WolkaboutProtocol::CHANNEL_DELIMITER + toString(MessageType::DEVICE_REMOVAL),
            WolkaboutProtocol::DEVICE_TO_PLATFORM_DIRECTION + WolkaboutProtocol::CHANNEL_DELIMITER + deviceKey +
              WolkaboutProtocol::CHANNEL_DELIMITER + toString(MessageType::REGISTERED_DEVICES_REQUEST)};
}

MessageType WolkaboutGatewayRegistrationProtocol::getMessageType(const Message& message)
{
    auto type = WolkaboutProtocol::getMessageType(message);
    if (type == MessageType::REGISTERED_DEVICES_RESPONSE)
        return MessageType::REGISTERED_DEVICES_REQUEST;
    return type;
}

DeviceType WolkaboutGatewayRegistrationProtocol::getDeviceType(const Message& message)
{
    return WolkaboutProtocol::getDeviceType(message);
}

std::string WolkaboutGatewayRegistrationProtocol::getDeviceKey(const Message& message) const
{
    return WolkaboutProtocol::getDeviceKey(message);
}

std::string WolkaboutGatewayRegistrationProtocol::getResponseChannelForMessage(MessageType /* type */,
                                                                               const std::string& /* deviceKey */) const
{
    return {};
}

std::unique_ptr<Message> WolkaboutGatewayRegistrationProtocol::makeOutboundMessage(
  const std::string& deviceKey, const RegisteredDevicesResponseMessage& message)
{
    LOG(TRACE) << METHOD_INFO;
    const auto errorPrefix = "Failed to generate outbound 'RegisteredDeviceResponse' message";

    // Create the topic
    try
    {
        return std::unique_ptr<Message>{new Message{
          json(message).dump(), WolkaboutProtocol::PLATFORM_TO_DEVICE_DIRECTION + WolkaboutProtocol::CHANNEL_DELIMITER +
                                  deviceKey + WolkaboutProtocol::CHANNEL_DELIMITER +
                                  toString(MessageType::REGISTERED_DEVICES_RESPONSE)}};
    }
    catch (const std::exception& exception)
    {
        LOG(ERROR) << errorPrefix << " -> Failed to parse the payload into a JSON - '" << exception.what() << "'.";
        return nullptr;
    }
}

std::unique_ptr<DeviceRegistrationMessage> WolkaboutGatewayRegistrationProtocol::parseDeviceRegistrationMessage(
  const std::shared_ptr<Message>& message)
{
    LOG(TRACE) << METHOD_INFO;
    const auto errorPrefix = "Failed to parse incoming 'DeviceRegistration' message";

    // Check that the message is a DeviceRegistration message.
    auto type = getMessageType(*message);
    if (type != MessageType::DEVICE_REGISTRATION)
    {
        LOG(ERROR) << errorPrefix << " -> The message is not a 'DeviceRegistration' message.";
        return nullptr;
    }

    try
    {
        // Parse the payload into json
        auto payload = json::parse(message->getContent());
        return std::unique_ptr<DeviceRegistrationMessage>(
          new DeviceRegistrationMessage{payload.get<std::vector<DeviceRegistrationData>>()});
    }
    catch (const std::exception& exception)
    {
        LOG(ERROR) << errorPrefix << " -> Failed to parse the incoming payload - '" << exception.what() << "'.";
        return nullptr;
    }
}

std::unique_ptr<DeviceRemovalMessage> WolkaboutGatewayRegistrationProtocol::parseDeviceRemovalMessage(
  const std::shared_ptr<Message>& message)
{
    LOG(TRACE) << METHOD_INFO;
    const auto errorPrefix = "Failed to parse incoming 'DeviceRemoval' message";

    // Check that the message is a DeviceRemoval message.
    auto type = getMessageType(*message);
    if (type != MessageType::DEVICE_REMOVAL)
    {
        LOG(ERROR) << errorPrefix << " -> The message is not a 'DeviceRemoval' message.";
        return nullptr;
    }

    try
    {
        // Parse the payload into json
        auto payload = json::parse(message->getContent());
        auto keys = payload.get<std::vector<std::string>>();
        for (const auto& key : keys)
        {
            if (key.empty())
            {
                LOG(ERROR) << errorPrefix << " -> One of the keys in the array is empty!";
                return nullptr;
            }
        }
        return std::unique_ptr<DeviceRemovalMessage>(new DeviceRemovalMessage{keys});
    }
    catch (const std::exception& exception)
    {
        LOG(ERROR) << errorPrefix << " -> Failed to parse the incoming payload - '" << exception.what() << "'.";
        return nullptr;
    }
}

std::unique_ptr<RegisteredDevicesRequestMessage>
WolkaboutGatewayRegistrationProtocol::parseRegisteredDevicesRequestMessage(const std::shared_ptr<Message>& message)
{
    LOG(TRACE) << METHOD_INFO;
    const auto errorPrefix = "Failed to parse incoming 'RegisteredDevicesRequest' message";

    // Check that the message is a RegisteredDevicesRequest message.
    auto type = getMessageType(*message);
    if (type != MessageType::REGISTERED_DEVICES_REQUEST)
    {
        LOG(ERROR) << errorPrefix << " -> The message is not a 'RegisteredDevicesRequest' message.";
        return nullptr;
    }

    try
    {
        // Parse the payload into json
        auto payload = json::parse(message->getContent());
        return std::unique_ptr<RegisteredDevicesRequestMessage>(
          new RegisteredDevicesRequestMessage{std::chrono::milliseconds{payload["timestampFrom"].get<std::uint64_t>()},
                                              extractValueIfPresent<std::string>(payload, "deviceType"),
                                              extractValueIfPresent<std::string>(payload, "externalId")});
    }
    catch (const std::exception& exception)
    {
        LOG(ERROR) << errorPrefix << " -> Failed to parse the incoming payload - '" << exception.what() << "'.";
        return nullptr;
    }
}
}    // namespace wolkabout

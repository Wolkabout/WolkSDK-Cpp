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

#include "core/protocol/wolkabout/WolkaboutDataProtocol.h"

#include "core/Types.h"
#include "core/model/Feed.h"
#include "core/model/Reading.h"
#include "core/protocol/wolkabout/WolkaboutProtocol.h"
#include "core/utilities/Logger.h"
#include "core/utilities/json.hpp"

#include <memory>
#include <string>

using nlohmann::json;

namespace wolkabout
{
static void to_json(json& j, const Feed& feed)
{
    // Check the feed type first
    auto typeString = toString(feed.getFeedType());
    if (typeString.empty())
        throw std::runtime_error("The FeedType value is not valid.");
    j = json{
      {"name", feed.getName()}, {"type", typeString}, {"unitGuid", feed.getUnit()}, {"reference", feed.getReference()}};
}

static void to_json(json& j, const Attribute& attribute)
{
    // Check the data type first
    auto typeString = toString(attribute.getDataType());
    if (typeString.empty())
        throw std::runtime_error("The DataType value is not valid.");
    j = json{{"name", attribute.getName()}, {"dataType", typeString}, {"value", attribute.getValue()}};
}

static void from_json(const json& j, std::vector<Reading>& r)
{
    // Check that the json is an array
    if (!j.is_array())
        throw std::runtime_error("Received payload is not an array.");

    // Now we need to go through every object in the array
    for (const auto& timePair : j.items())
    {
        // Check that this json is an object
        const auto& readings = timePair.value();
        if (!readings.is_object())
            throw std::runtime_error("One of array members is not an object.");

        // Obtain the timestamp from the object
        auto timestampIt = readings.find(WolkaboutProtocol::TIMESTAMP_KEY);
        if (timestampIt == readings.end())
            throw std::runtime_error("Missing key '" + WolkaboutProtocol::TIMESTAMP_KEY + "' in array member.");
        if (!timestampIt->is_number_unsigned())
            throw std::runtime_error("Value of '" + WolkaboutProtocol::TIMESTAMP_KEY + "' is not an unsigned integer.");
        const auto timestamp = timestampIt.value().get<std::uint64_t>();

        // Create a list of all readings that will be read for this single timestamp.
        auto read = std::vector<Reading>{};

        // Now read all the readings
        for (const auto& valuePair : readings.items())
        {
            // Skip the timestamp
            const auto& key = valuePair.key();
            const auto& value = valuePair.value();
            if (key == WolkaboutProtocol::TIMESTAMP_KEY)
                continue;

            // Now take out the value
            switch (value.type())
            {
            case nlohmann::detail::value_t::string:
            case nlohmann::detail::value_t::boolean:
            case nlohmann::detail::value_t::number_integer:
            case nlohmann::detail::value_t::number_unsigned:
            case nlohmann::detail::value_t::number_float:
            {
                read.emplace_back(Reading{key, WolkaboutProtocol::removeQuotes(value.dump()), timestamp});
                break;
            }
            case nlohmann::detail::value_t::array:
            {
                // In case it is an array, we have to verify that each value is numeric inside the array
                auto subValues = std::vector<std::string>{};
                for (const auto& subValuePair : value.items())
                {
                    const auto& subValue = subValuePair.value();
                    if (!subValue.is_number())
                        throw std::runtime_error("Vector of values contains non-numeric values.");
                    subValues.emplace_back(WolkaboutProtocol::removeQuotes(subValue.dump()));
                }
                read.emplace_back(Reading{key, subValues, timestamp});
                break;
            }
            default:
                throw std::runtime_error("Value under key is not a number/boolean/string/array.");
            }
        }

        // Now add all the readings into the vector
        r.insert(r.cend(), read.cbegin(), read.cend());
    }
}

static void from_json(const json& j, std::vector<Parameter>& p)
{
    // Check that the json is an object
    if (!j.is_object())
        throw std::runtime_error("Received payload is not an object.");

    // Now we need to go through every pair in the object
    for (const auto& parameterPair : j.items())
    {
        // Check that the parameter name is valid
        const auto& key = parameterPair.key();
        const auto parameterName = parameterNameFromString(key);
        if (parameterName == ParameterName::UNKNOWN)
            throw std::runtime_error("Received an invalid parameter name.");

        // We need to check that the value is of valid type
        const auto& value = parameterPair.value();
        if (!(value.is_string() || value.is_boolean() || value.is_number() || value.is_null()))
            throw std::runtime_error("Received a value of invalid type for a parameter.");

        // Add the parameter in the vector
        p.emplace_back(parameterName, WolkaboutProtocol::removeQuotes(value.dump()));
    }
}

std::vector<std::string> WolkaboutDataProtocol::getInboundChannels() const
{
    return {};
}

std::vector<std::string> WolkaboutDataProtocol::getInboundChannelsForDevice(const std::string& deviceKey) const
{
    return {WolkaboutProtocol::PLATFORM_TO_DEVICE_DIRECTION + WolkaboutProtocol::CHANNEL_DELIMITER + deviceKey +
              WolkaboutProtocol::CHANNEL_DELIMITER + toString(MessageType::PARAMETER_SYNC),
            WolkaboutProtocol::PLATFORM_TO_DEVICE_DIRECTION + WolkaboutProtocol::CHANNEL_DELIMITER + deviceKey +
              WolkaboutProtocol::CHANNEL_DELIMITER + toString(MessageType::FEED_VALUES),
            WolkaboutProtocol::PLATFORM_TO_DEVICE_DIRECTION + WolkaboutProtocol::CHANNEL_DELIMITER + deviceKey +
              WolkaboutProtocol::CHANNEL_DELIMITER + toString(MessageType::DETAILS_SYNCHRONIZATION_RESPONSE)};
}

std::string WolkaboutDataProtocol::getDeviceKey(const Message& message) const
{
    LOG(TRACE) << METHOD_INFO;
    return WolkaboutProtocol::getDeviceKey(message);
}

DeviceType WolkaboutDataProtocol::getDeviceType(const Message& message)
{
    LOG(TRACE) << METHOD_INFO;
    return WolkaboutProtocol::getDeviceType(message);
}

MessageType WolkaboutDataProtocol::getMessageType(const Message& message)
{
    LOG(TRACE) << METHOD_INFO;
    return WolkaboutProtocol::getMessageType(message);
}

std::unique_ptr<Message> WolkaboutDataProtocol::makeOutboundMessage(const std::string& deviceKey,
                                                                    FeedRegistrationMessage feedRegistrationMessage)
{
    LOG(TRACE) << METHOD_INFO;

    try
    {
        // Create the topic
        const auto topic = WolkaboutProtocol::DEVICE_TO_PLATFORM_DIRECTION + WolkaboutProtocol::CHANNEL_DELIMITER +
                           deviceKey + WolkaboutProtocol::CHANNEL_DELIMITER +
                           toString(feedRegistrationMessage.getMessageType());

        // Create the content
        auto feeds = feedRegistrationMessage.getFeeds();
        auto payload = json(feeds);
        return std::unique_ptr<Message>(new Message{payload.dump(), topic});
    }
    catch (const std::exception& exception)
    {
        LOG(ERROR) << "Failed to serialize 'FeedRegistrationMessage' -> '" << exception.what() << "'.";
        return nullptr;
    }
}

std::unique_ptr<Message> WolkaboutDataProtocol::makeOutboundMessage(const std::string& deviceKey,
                                                                    FeedRemovalMessage feedRemovalMessage)
{
    LOG(TRACE) << METHOD_INFO;

    // Check that the array is not empty
    if (feedRemovalMessage.getReferences().empty())
    {
        LOG(ERROR) << "Failed to serialize 'FeedRemovalMessage' -> The feed reference array is empty.";
        return nullptr;
    }

    // Create the topic
    const auto topic = WolkaboutProtocol::DEVICE_TO_PLATFORM_DIRECTION + WolkaboutProtocol::CHANNEL_DELIMITER +
                       deviceKey + WolkaboutProtocol::CHANNEL_DELIMITER + toString(feedRemovalMessage.getMessageType());

    try
    {
        // Create the content
        auto feeds = feedRemovalMessage.getReferences();
        auto payload = json(feeds);
        return std::unique_ptr<Message>(new Message{payload.dump(), topic});
    }
    catch (const std::exception& exception)
    {
        LOG(ERROR) << "Failed to serialize 'FeedRemovalMessage' -> '" << exception.what() << "'.";
        return nullptr;
    }
}

std::unique_ptr<Message> WolkaboutDataProtocol::makeOutboundMessage(const std::string& deviceKey,
                                                                    FeedValuesMessage feedValuesMessage)
{
    LOG(TRACE) << METHOD_INFO;

    // Check that the map is not empty
    if (feedValuesMessage.getReadings().empty())
    {
        LOG(ERROR) << "Failed to serialize 'FeedValuesMessage' -> The readings map is empty.";
        return nullptr;
    }

    // Create the topic
    const auto topic = WolkaboutProtocol::DEVICE_TO_PLATFORM_DIRECTION + WolkaboutProtocol::CHANNEL_DELIMITER +
                       deviceKey + WolkaboutProtocol::CHANNEL_DELIMITER + toString(feedValuesMessage.getMessageType());

    try
    {
        // Create the content
        auto payload = json::array();
        const auto& values = feedValuesMessage.getReadings();
        for (const auto& member : values)
        {
            // Check if the map is empty
            if (member.second.empty())
            {
                LOG(ERROR) << "Failed to serialize 'FeedValuesMessage' -> One of the readings map entries is empty.";
                return nullptr;
            }

            // Create the object for this time
            auto time = json();
            if (member.first)
                time[WolkaboutProtocol::TIMESTAMP_KEY] = member.first;
            for (const auto& reading : member.second)
            {
                const auto& key = reading.getReference();
                if (reading.isBoolean())
                    time[key] = reading.getBoolValue();
                else if (reading.isUInt())
                    time[key] = reading.getUIntValue();
                else if (reading.isInt())
                    time[key] = reading.getIntValue();
                else if (reading.isDouble())
                    time[key] = reading.getDoubleValue();
                else
                    time[key] = reading.getStringValue();
            }

            // And add it into the array
            payload += time;
        }
        return std::unique_ptr<Message>(new Message{payload.dump(), topic});
    }
    catch (const std::exception& exception)
    {
        LOG(ERROR) << "Failed to serialize 'FeedValuesMessage' -> '" << exception.what() << "'.";
        return nullptr;
    }
}

std::unique_ptr<Message> WolkaboutDataProtocol::makeOutboundMessage(const std::string& deviceKey,
                                                                    PullFeedValuesMessage pullFeedValuesMessage)
{
    LOG(TRACE) << METHOD_INFO;

    // Create the topic
    const auto topic = WolkaboutProtocol::DEVICE_TO_PLATFORM_DIRECTION + WolkaboutProtocol::CHANNEL_DELIMITER +
                       deviceKey + WolkaboutProtocol::CHANNEL_DELIMITER +
                       toString(pullFeedValuesMessage.getMessageType());
    return std::unique_ptr<Message>(new Message({}, topic));
}

std::unique_ptr<Message> WolkaboutDataProtocol::makeOutboundMessage(
  const std::string& deviceKey, AttributeRegistrationMessage attributeRegistrationMessage)
{
    LOG(TRACE) << METHOD_INFO;

    // Create the topic
    const auto topic = WolkaboutProtocol::DEVICE_TO_PLATFORM_DIRECTION + WolkaboutProtocol::CHANNEL_DELIMITER +
                       deviceKey + WolkaboutProtocol::CHANNEL_DELIMITER +
                       toString(attributeRegistrationMessage.getMessageType());

    try
    {
        // Create the payload
        auto attributes = attributeRegistrationMessage.getAttributes();
        auto payload = json(attributes);
        return std::unique_ptr<Message>(new Message{payload.dump(), topic});
    }
    catch (const std::exception& exception)
    {
        LOG(ERROR) << "Failed to serialize 'AttributeRegistrationMessage' -> '" << exception.what() << "'.";
        return nullptr;
    }
}

std::unique_ptr<Message> WolkaboutDataProtocol::makeOutboundMessage(const std::string& deviceKey,
                                                                    ParametersUpdateMessage parametersUpdateMessage)
{
    LOG(TRACE) << METHOD_INFO;

    // Check that the array is not empty
    if (parametersUpdateMessage.getParameters().empty())
    {
        LOG(ERROR) << "Failed to serialize 'ParametersUpdateMessage' -> The parameters array is empty.";
        return nullptr;
    }

    // Make the topic
    const auto topic = WolkaboutProtocol::DEVICE_TO_PLATFORM_DIRECTION + WolkaboutProtocol::CHANNEL_DELIMITER +
                       deviceKey + WolkaboutProtocol::CHANNEL_DELIMITER +
                       toString(parametersUpdateMessage.getMessageType());

    try
    {
        // Make the payload
        auto payload = json();
        for (const auto& parameter : parametersUpdateMessage.getParameters())
        {
            // Check the parameter name
            auto parameterName = toString(parameter.first);
            if (parameterName.empty())
            {
                LOG(ERROR)
                  << "Failed to serialize 'ParametersUpdateMessage' -> One of parameter keys values is invalid.";
                return nullptr;
            }

            // Make the reading out of the value, and check how it can be written in
            auto reading = Reading{"Parameter", parameter.second};
            if (reading.isBoolean())
                payload[parameterName] = reading.getBoolValue();
            else if (reading.isUInt())
                payload[parameterName] = reading.getUIntValue();
            else if (reading.isInt())
                payload[parameterName] = reading.getIntValue();
            else if (reading.isDouble())
                payload[parameterName] = reading.getDoubleValue();
            else
                payload[parameterName] = reading.getStringValue();
        }
        return std::unique_ptr<Message>(new Message{payload.dump(), topic});
    }
    catch (const std::exception& exception)
    {
        LOG(ERROR) << "Failed to serialize 'ParametersUpdateMessage' -> '" << exception.what() << "'.";
        return nullptr;
    }
}

std::unique_ptr<Message> WolkaboutDataProtocol::makeOutboundMessage(const std::string& deviceKey,
                                                                    ParametersPullMessage parametersPullMessage)
{
    LOG(TRACE) << METHOD_INFO;

    // Create the topic
    const auto topic = WolkaboutProtocol::DEVICE_TO_PLATFORM_DIRECTION + WolkaboutProtocol::CHANNEL_DELIMITER +
                       deviceKey + WolkaboutProtocol::CHANNEL_DELIMITER +
                       toString(parametersPullMessage.getMessageType());
    return std::unique_ptr<Message>(new Message{"", topic});
}

std::unique_ptr<Message> WolkaboutDataProtocol::makeOutboundMessage(
  const std::string& deviceKey, SynchronizeParametersMessage synchronizeParametersMessage)
{
    LOG(TRACE) << METHOD_INFO;

    // Create the topic
    const auto topic = WolkaboutProtocol::DEVICE_TO_PLATFORM_DIRECTION + WolkaboutProtocol::CHANNEL_DELIMITER +
                       deviceKey + WolkaboutProtocol::CHANNEL_DELIMITER + toString(MessageType::SYNCHRONIZE_PARAMETERS);

    // Create the payload
    auto payload = json::array();
    for (const auto& parameter : synchronizeParametersMessage.getParameters())
    {
        // Check that the parameter is valid
        auto parameterString = toString(parameter);
        if (parameterString.empty())
        {
            LOG(ERROR)
              << "Failed to serialize 'SynchronizeParametersMessage' -> One of parameter name values is not valid.";
            return nullptr;
        }
        payload += parameterString;
    }
    return std::unique_ptr<Message>(new Message{payload.dump(), topic});
}

std::unique_ptr<Message> WolkaboutDataProtocol::makeOutboundMessage(
  const std::string& deviceKey, DetailsSynchronizationRequestMessage detailsSynchronizationRequestMessage)
{
    LOG(TRACE) << METHOD_INFO;

    // Create the topic
    const auto topic = WolkaboutProtocol::DEVICE_TO_PLATFORM_DIRECTION + WolkaboutProtocol::CHANNEL_DELIMITER +
                       deviceKey + WolkaboutProtocol::CHANNEL_DELIMITER +
                       toString(detailsSynchronizationRequestMessage.getMessageType());
    return std::unique_ptr<Message>(new Message{"", topic});
}

std::shared_ptr<FeedValuesMessage> WolkaboutDataProtocol::parseFeedValues(std::shared_ptr<Message> message)
{
    LOG(TRACE) << METHOD_INFO;

    try
    {
        auto j = json::parse(message->getContent());
        auto readings = j.get<std::vector<Reading>>();
        return std::make_shared<FeedValuesMessage>(readings);
    }
    catch (const std::exception& exception)
    {
        LOG(ERROR) << "Failed to deserialize 'FeedValues' message -> '" << exception.what() << "'.";
        return nullptr;
    }
}

std::shared_ptr<ParametersUpdateMessage> WolkaboutDataProtocol::parseParameters(std::shared_ptr<Message> message)
{
    LOG(TRACE) << METHOD_INFO;

    try
    {
        auto j = json::parse(message->getContent());
        auto parameters = j.get<std::vector<Parameter>>();
        return std::make_shared<ParametersUpdateMessage>(parameters);
    }
    catch (const std::exception& exception)
    {
        LOG(ERROR) << "Failed to deserialize 'Parameters' message -> '" << exception.what() << "'.";
        return nullptr;
    }
}

std::shared_ptr<DetailsSynchronizationResponseMessage> WolkaboutDataProtocol::parseDetails(
  std::shared_ptr<Message> message)
{
    LOG(TRACE) << METHOD_INFO;
    const auto errorPrefix = "Failed to parse 'DetailsSynchronizationResponse' message -> ";

    // Check that the message is a DetailsSynchronizationResponse message.
    auto type = getMessageType(*message);
    if (type != MessageType::DETAILS_SYNCHRONIZATION_RESPONSE)
    {
        LOG(ERROR) << errorPrefix << " -> The message is not a 'DetailsSynchronizationResponse' message.";
        return nullptr;
    }

    try
    {
        // Parse the information
        auto j = json::parse(message->getContent());
        if (!j.is_object())
        {
            LOG(ERROR) << errorPrefix << " -> The payload is not a JSON object.";
            return nullptr;
        }
        auto feeds = j["feeds"].get<std::vector<std::string>>();
        if (std::any_of(feeds.cbegin(), feeds.cend(), [](const std::string& value) { return value.empty(); }))
        {
            LOG(ERROR) << errorPrefix << " -> The feeds array contains an empty string.";
            return nullptr;
        }
        auto attributes = j["attributes"].get<std::vector<std::string>>();
        if (std::any_of(attributes.cbegin(), attributes.cend(), [](const std::string& value) { return value.empty(); }))
        {
            LOG(ERROR) << errorPrefix << " -> The attributes array contains an empty string.";
            return nullptr;
        }
        return std::make_shared<DetailsSynchronizationResponseMessage>(feeds, attributes);
    }
    catch (const std::exception& exception)
    {
        LOG(ERROR) << errorPrefix << " -> '" << exception.what() << "'.";
        return nullptr;
    }
}
}    // namespace wolkabout

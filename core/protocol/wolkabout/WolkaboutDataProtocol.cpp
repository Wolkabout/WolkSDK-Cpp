/**
 * Copyright 2021 WolkAbout Technology s.r.o.
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
#include "core/protocol/wolkabout/WolkaboutProtocol.h"

#include "core/Types.h"
#include "core/model/Feed.h"
#include "core/utilities/Logger.h"
#include "core/utilities/json.hpp"

#include <memory>
#include <string>

using nlohmann::json;

namespace wolkabout
{
static void to_json(json& j, const Feed& feed)
{
    j = json{{"name", feed.getName()},
             {"type", toString(feed.getFeedType())},
             {"unitGuid", toString(feed.getUnit())},
             {"reference", feed.getReference()}};
}

static void to_json(json& j, const Reading& reading)
{
    j = json{reading.getReference(), reading.getStringValue()};
}

static void to_json(json& j, const std::vector<Reading>& readings)
{
    for (const auto& reading : readings)
    {
        j += json{reading.getReference(), reading.getStringValue()};
    }
}

static void to_json(json& j, const Attribute& attribute)
{
    j = json{
      {"name", attribute.getName()}, {"dataType", toString(attribute.getDataType())}, {"value", attribute.getValue()}};
}

static void to_json(json& j, const Parameters& param)
{
    j[toString(param.first)] = param.second;
}

static void from_json(const json& j, std::vector<Parameters>& p)
{
    for (auto& el : j.items())
    {
        Parameters param{parameterNameFromString(el.key()), el.value().dump()};
        p.emplace_back(param);
    }
}

std::unique_ptr<Message> WolkaboutDataProtocol::makeOutboundMessage(const std::string& deviceKey,
                                                                    FeedRegistrationMessage feedRegistrationMessage)
{
    auto topic = DEVICE_TO_PLATFORM_DIRECTION + CHANNEL_DELIMITER + deviceKey + CHANNEL_DELIMITER +
                 toString(feedRegistrationMessage.getMessageType());

    auto feeds = feedRegistrationMessage.getFeeds();
    json payload = json(feeds);

    return std::unique_ptr<Message>(new Message(json(payload).dump(), topic));
}

std::unique_ptr<Message> WolkaboutDataProtocol::makeOutboundMessage(const std::string& deviceKey,
                                                                    FeedRemovalMessage feedRemovalMessage)
{
    auto topic = DEVICE_TO_PLATFORM_DIRECTION + CHANNEL_DELIMITER + deviceKey + CHANNEL_DELIMITER +
                 toString(feedRemovalMessage.getMessageType());

    auto feeds = feedRemovalMessage.getReferences();
    json payload = json(feeds);

    return std::unique_ptr<Message>(new Message(json(payload).dump(), topic));
}

std::unique_ptr<Message> WolkaboutDataProtocol::makeOutboundMessage(const std::string& deviceKey,
                                                                    FeedValuesMessage feedValuesMessage)
{
    auto topic = DEVICE_TO_PLATFORM_DIRECTION + CHANNEL_DELIMITER + deviceKey + CHANNEL_DELIMITER +
                 toString(feedValuesMessage.getMessageType());

    auto values = feedValuesMessage.getReadings();

    json payload;

    for (auto const& member : values)
    {
        json forTimestamp;
        for (auto reading : member.second)
        {
            forTimestamp[reading.getReference()] = reading.getStringValue();
        }
        if (member.first != 0)
        {
            forTimestamp["timestamp"] = member.first;
        }
        payload += json(forTimestamp);
    }
    auto jsonstring = json(payload).dump();
    return std::unique_ptr<Message>(new Message(json(payload).dump(), topic));
}

std::unique_ptr<Message> WolkaboutDataProtocol::makeOutboundMessage(const std::string& deviceKey,
                                                                    PullFeedValuesMessage pullFeedValuesMessage)
{
    auto topic = DEVICE_TO_PLATFORM_DIRECTION + CHANNEL_DELIMITER + deviceKey + CHANNEL_DELIMITER +
                 toString(pullFeedValuesMessage.getMessageType());

    return std::unique_ptr<Message>(new Message("", topic));
}

std::unique_ptr<Message> WolkaboutDataProtocol::makeOutboundMessage(
  const std::string& deviceKey, AttributeRegistrationMessage attributeRegistrationMessage)
{
    auto topic = DEVICE_TO_PLATFORM_DIRECTION + CHANNEL_DELIMITER + deviceKey + CHANNEL_DELIMITER +
                 toString(attributeRegistrationMessage.getMessageType());

    auto attributes = attributeRegistrationMessage.getAttributes();
    json payload = json(attributes);

    return std::unique_ptr<Message>(new Message(json(payload).dump(), topic));
}

std::unique_ptr<Message> WolkaboutDataProtocol::makeOutboundMessage(const std::string& deviceKey,
                                                                    ParametersUpdateMessage parametersUpdateMessage)
{
    auto topic = DEVICE_TO_PLATFORM_DIRECTION + CHANNEL_DELIMITER + deviceKey + CHANNEL_DELIMITER +
                 toString(parametersUpdateMessage.getMessageType());

    json payload;

    for (auto parameter : parametersUpdateMessage.getParameters())
    {
        payload[toString(parameter.first)] = parameter.second;
    }

    return std::unique_ptr<Message>(new Message(json(payload).dump(), topic));
}

std::unique_ptr<Message> WolkaboutDataProtocol::makeOutboundMessage(const std::string& deviceKey,
                                                                    ParametersPullMessage parametersPullMessage)
{
    auto topic = DEVICE_TO_PLATFORM_DIRECTION + CHANNEL_DELIMITER + deviceKey + CHANNEL_DELIMITER +
                 toString(parametersPullMessage.getMessageType());

    return std::unique_ptr<Message>(new Message("", topic));
}

std::vector<std::string> WolkaboutDataProtocol::getInboundChannelsForDevice(const std::string& deviceKey) const
{
    return {PLATFORM_TO_DEVICE_DIRECTION + CHANNEL_DELIMITER + deviceKey + CHANNEL_DELIMITER +
              toString(MessageType::PARAMETER_SYNC),
            PLATFORM_TO_DEVICE_DIRECTION + CHANNEL_DELIMITER + deviceKey + CHANNEL_DELIMITER +
              toString(MessageType::FEED_VALUES)};
}

std::string WolkaboutDataProtocol::extractDeviceKeyFromChannel(const std::string& topic) const
{
    auto first = topic.find(CHANNEL_DELIMITER);
    auto last = topic.rfind(CHANNEL_DELIMITER);
    return topic.substr(first + 1, last - first - 1);
}

MessageType WolkaboutDataProtocol::getMessageType(std::shared_ptr<Message> message)
{
    auto channel = message->getChannel();
    auto last = channel.rfind(CHANNEL_DELIMITER);
    return messageTypeFromString(channel.substr(last + 1));
}

std::shared_ptr<FeedValuesMessage> WolkaboutDataProtocol::parseFeedValues(std::shared_ptr<Message> message)
{
    try
    {
        // Check that the payload is a valid JSON array
        auto content = message->getContent();
        auto jsonContent = json::parse(content);
        if (!jsonContent.is_array())
        {
            LOG(ERROR) << "Failed to parse 'FeedValuesMessage' -> The received JSON payload is not a JSON array.";
            return nullptr;
        }

        // Now parse every array entry
        auto readings = std::vector<Reading>{};
        for (const auto& arrayItem : jsonContent.items())
        {
            // Take the array value
            const auto entry = arrayItem.value();
            if (!entry.is_object())
            {
                LOG(ERROR) << "Failed to parse 'FeedValuesMessage' -> The received JSON payload has array members that "
                              "are not objects.";
                return nullptr;
            }

            // Look for the timestamp
            const auto timestampIt = entry.find(TIMESTAMP_KEY);
            if (timestampIt == entry.cend())
            {
                LOG(ERROR) << "Failed to parse 'FeedValuesMessage' -> The received JSON payload contains entries that "
                              "are missing the timestamp.";
                return nullptr;
            }
            // Parse the timestamp into a usable value
            const auto timestamp = timestampIt->get<std::uint64_t>();

            // Look for any feed values and make a reading for each one of them
            for (const auto& readingItem : entry.items())
            {
                // Skip it if it is the timestamp
                const auto& reference = readingItem.key();
                if (reference == TIMESTAMP_KEY)
                    continue;

                // Check the type of the value
                const auto& readingValue = readingItem.value();
                switch (readingValue.type())
                {
                case nlohmann::detail::value_t::boolean:
                    readings.emplace_back(Reading{reference, readingValue.get<bool>() ? "true" : "false", timestamp});
                    break;
                case nlohmann::detail::value_t::string:
                    readings.emplace_back(Reading{reference, readingValue.get<std::string>(), timestamp});
                    break;
                case nlohmann::detail::value_t::number_unsigned:
                    readings.emplace_back(
                      Reading{reference, std::to_string(readingValue.get<std::uint64_t>()), timestamp});
                    break;
                case nlohmann::detail::value_t::number_integer:
                    readings.emplace_back(
                      Reading{reference, std::to_string(readingValue.get<std::int64_t>()), timestamp});
                    break;
                case nlohmann::detail::value_t::number_float:
                    readings.emplace_back(
                      Reading{reference, std::to_string(readingValue.get<std::float_t>()), timestamp});
                    break;
                default:
                    LOG(ERROR) << "Failed to parse 'FeedValuesMessage' -> The received JSON payload contains an "
                                  "invalid feed value.";
                    return nullptr;
                }
            }
        }
        return std::make_shared<FeedValuesMessage>(readings);
    }
    catch (const std::exception& exception)
    {
        LOG(ERROR) << "Failed to parse 'FeedValuesMessage' -> '" << exception.what() << "'.";
        return nullptr;
    }
}

std::shared_ptr<ParametersUpdateMessage> WolkaboutDataProtocol::parseParameters(std::shared_ptr<Message> message)
{
    json j = json::parse(message->getContent());

    std::vector<Parameters> parameters = j.get<std::vector<Parameters>>();
    return std::make_shared<ParametersUpdateMessage>(parameters);
}

std::vector<std::string> WolkaboutDataProtocol::getInboundChannels() const
{
    return {PLATFORM_TO_DEVICE_DIRECTION + CHANNEL_DELIMITER + CHANNEL_SINGLE_LEVEL_WILDCARD + CHANNEL_DELIMITER +
              toString(MessageType::PARAMETER_SYNC),
            PLATFORM_TO_DEVICE_DIRECTION + CHANNEL_DELIMITER + CHANNEL_SINGLE_LEVEL_WILDCARD + CHANNEL_DELIMITER +
              toString(MessageType::FEED_VALUES)};
}
}    // namespace wolkabout

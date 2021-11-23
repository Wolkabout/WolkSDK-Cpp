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

static void from_json(const json& j, std::vector<Reading>& r)
{
    for (const auto& time : j.items())
    {
        // Collect all the readings for this time
        auto timestamp = std::uint64_t{0};
        auto readings = std::vector<Reading>{};

        // Read the values from the readings map
        for (const auto& reading : time.value().items())
        {
            if (reading.key() == "timestamp")
                timestamp = reading.value().get<std::uint64_t>();
            else
            {
                // Inspect the type of the value
                const auto& readingValue = reading.value();
                const auto type = readingValue.type();
                switch (readingValue.type())
                {
                case nlohmann::detail::value_t::boolean:
                    readings.emplace_back(
                      Reading{reading.key(), readingValue.get<bool>() ? "true" : "false", timestamp});
                    break;
                case nlohmann::detail::value_t::string:
                    readings.emplace_back(Reading{reading.key(), readingValue.get<std::string>(), timestamp});
                    break;
                case nlohmann::detail::value_t::number_unsigned:
                    readings.emplace_back(
                      Reading{reading.key(), std::to_string(readingValue.get<std::uint64_t>()), timestamp});
                    break;
                case nlohmann::detail::value_t::number_integer:
                    readings.emplace_back(
                      Reading{reading.key(), std::to_string(readingValue.get<std::int64_t>()), timestamp});
                    break;
                case nlohmann::detail::value_t::number_float:
                    readings.emplace_back(
                      Reading{reading.key(), std::to_string(readingValue.get<std::float_t>()), timestamp});
                    break;
                default:
                    throw std::runtime_error("Failed to parse 'FeedValuesMessage' -> The received JSON payload "
                                             "contains an invalid feed value.");
                }
            }
        }

        // Set the timestamp for all readings and put them into the vector
        for (auto& reading : readings)
        {
            reading.setTimestamp(timestamp);
            r.emplace_back(reading);
        }
    }
}

static void from_json(const json& j, std::vector<Parameters>& p)
{
    for (auto& el : j.items())
    {
        Parameters param{parameterNameFromString(el.key()), el.value().dump()};
        p.emplace_back(param);
    }
}

std::vector<std::string> WolkaboutDataProtocol::getInboundChannels() const
{
    return {PLATFORM_TO_DEVICE_DIRECTION + CHANNEL_DELIMITER + CHANNEL_SINGLE_LEVEL_WILDCARD + CHANNEL_DELIMITER +
              toString(MessageType::PARAMETER_SYNC),
            PLATFORM_TO_DEVICE_DIRECTION + CHANNEL_DELIMITER + CHANNEL_SINGLE_LEVEL_WILDCARD + CHANNEL_DELIMITER +
              toString(MessageType::FEED_VALUES)};
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
    LOG(TRACE) << METHOD_INFO;
    return WolkaboutProtocol::extractDeviceKeyFromChannel(topic);
}

MessageType WolkaboutDataProtocol::getMessageType(std::shared_ptr<MqttMessage> message)
{
    LOG(TRACE) << METHOD_INFO;
    return WolkaboutProtocol::getMessageType(message);
}

std::unique_ptr<MqttMessage> WolkaboutDataProtocol::makeOutboundMessage(const std::string& deviceKey,
                                                                        FeedRegistrationMessage feedRegistrationMessage)
{
    auto topic = DEVICE_TO_PLATFORM_DIRECTION + CHANNEL_DELIMITER + deviceKey + CHANNEL_DELIMITER +
                 toString(feedRegistrationMessage.getMessageType());

    auto feeds = feedRegistrationMessage.getFeeds();
    json payload = json(feeds);

    return std::unique_ptr<MqttMessage>(new MqttMessage(json(payload).dump(), topic));
}

std::unique_ptr<MqttMessage> WolkaboutDataProtocol::makeOutboundMessage(const std::string& deviceKey,
                                                                        FeedRemovalMessage feedRemovalMessage)
{
    auto topic = DEVICE_TO_PLATFORM_DIRECTION + CHANNEL_DELIMITER + deviceKey + CHANNEL_DELIMITER +
                 toString(feedRemovalMessage.getMessageType());

    auto feeds = feedRemovalMessage.getReferences();
    json payload = json(feeds);

    return std::unique_ptr<MqttMessage>(new MqttMessage(json(payload).dump(), topic));
}

std::unique_ptr<MqttMessage> WolkaboutDataProtocol::makeOutboundMessage(const std::string& deviceKey,
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
    return std::unique_ptr<MqttMessage>(new MqttMessage(json(payload).dump(), topic));
}

std::unique_ptr<MqttMessage> WolkaboutDataProtocol::makeOutboundMessage(const std::string& deviceKey,
                                                                        PullFeedValuesMessage pullFeedValuesMessage)
{
    auto topic = DEVICE_TO_PLATFORM_DIRECTION + CHANNEL_DELIMITER + deviceKey + CHANNEL_DELIMITER +
                 toString(pullFeedValuesMessage.getMessageType());

    return std::unique_ptr<MqttMessage>(new MqttMessage("", topic));
}

std::unique_ptr<MqttMessage> WolkaboutDataProtocol::makeOutboundMessage(
  const std::string& deviceKey, AttributeRegistrationMessage attributeRegistrationMessage)
{
    auto topic = DEVICE_TO_PLATFORM_DIRECTION + CHANNEL_DELIMITER + deviceKey + CHANNEL_DELIMITER +
                 toString(attributeRegistrationMessage.getMessageType());

    auto attributes = attributeRegistrationMessage.getAttributes();
    json payload = json(attributes);

    return std::unique_ptr<MqttMessage>(new MqttMessage(json(payload).dump(), topic));
}

std::unique_ptr<MqttMessage> WolkaboutDataProtocol::makeOutboundMessage(const std::string& deviceKey,
                                                                        ParametersUpdateMessage parametersUpdateMessage)
{
    auto topic = DEVICE_TO_PLATFORM_DIRECTION + CHANNEL_DELIMITER + deviceKey + CHANNEL_DELIMITER +
                 toString(parametersUpdateMessage.getMessageType());

    json payload;

    for (auto parameter : parametersUpdateMessage.getParameters())
    {
        payload[toString(parameter.first)] = parameter.second;
    }

    return std::unique_ptr<MqttMessage>(new MqttMessage(json(payload).dump(), topic));
}

std::unique_ptr<MqttMessage> WolkaboutDataProtocol::makeOutboundMessage(const std::string& deviceKey,
                                                                        ParametersPullMessage parametersPullMessage)
{
    auto topic = DEVICE_TO_PLATFORM_DIRECTION + CHANNEL_DELIMITER + deviceKey + CHANNEL_DELIMITER +
                 toString(parametersPullMessage.getMessageType());

    return std::unique_ptr<MqttMessage>(new MqttMessage("", topic));
}

std::shared_ptr<FeedValuesMessage> WolkaboutDataProtocol::parseFeedValues(std::shared_ptr<MqttMessage> message)
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
        LOG(ERROR) << "Failed to parse 'FeedValues' message -> '" << exception.what() << "'.";
        return nullptr;
    }
}

std::shared_ptr<ParametersUpdateMessage> WolkaboutDataProtocol::parseParameters(std::shared_ptr<MqttMessage> message)
{
    LOG(TRACE) << METHOD_INFO;

    try
    {
        auto j = json::parse(message->getContent());
        auto parameters = j.get<std::vector<Parameters>>();
        return std::make_shared<ParametersUpdateMessage>(parameters);
    }
    catch (const std::exception& exception)
    {
        LOG(ERROR) << "Failed to parse 'Parameters' message -> '" << exception.what() << "'.";
        return nullptr;
    }
}
}    // namespace wolkabout

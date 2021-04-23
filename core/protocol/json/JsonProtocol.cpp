/*
 * Copyright 2019 WolkAbout Technology s.r.o.
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

#include "JsonProtocol.h"

#include "Json.h"
#include "core/model/ActuatorSetCommand.h"
#include "core/model/ActuatorStatus.h"
#include "core/model/Alarm.h"
#include "core/model/ConfigurationItem.h"
#include "core/model/ConfigurationSetCommand.h"
#include "core/model/Message.h"
#include "core/model/SensorReading.h"
#include "core/utilities/Logger.h"
#include "core/utilities/StringUtils.h"
#include "core/utilities/json.hpp"

#include <algorithm>

using nlohmann::json;

namespace wolkabout
{
const std::string JsonProtocol::SENSOR_READING_TOPIC_ROOT = "d2p/sensor_reading/";
const std::string JsonProtocol::EVENT_TOPIC_ROOT = "d2p/events/";
const std::string JsonProtocol::ACTUATION_STATUS_TOPIC_ROOT = "d2p/actuator_status/";
const std::string JsonProtocol::CONFIGURATION_STATUS_TOPIC_ROOT = "d2p/configuration_get/";

const std::string JsonProtocol::ACTUATION_SET_TOPIC_ROOT = "p2d/actuator_set/";
const std::string JsonProtocol::CONFIGURATION_SET_TOPIC_ROOT = "p2d/configuration_set/";

const std::string JsonProtocol::MULTIVALUE_READING_DELIMITER = ",";

static void to_json(json& j, const SensorReading& p)
{
    if (p.getRtc() == 0)
    {
        j = json{{"data", p.getValue()}};
    }
    else
    {
        j = json{{"utc", p.getRtc()}, {"data", p.getValue()}};
    }
}

static void to_json(json& j, const std::shared_ptr<SensorReading>& p)
{
    if (!p)
    {
        return;
    }

    to_json(j, *p);
}

static void to_json(json& j, const Alarm& p)
{
    if (p.getRtc() == 0)
    {
        j = json{{"data", p.getValue()}};
    }
    else
    {
        j = json{{"utc", p.getRtc()}, {"data", p.getValue()}};
    }
}

static void to_json(json& j, const std::shared_ptr<Alarm>& p)
{
    if (!p)
    {
        return;
    }

    to_json(j, *p);
}

static void to_json(json& j, const ActuatorStatus& p)
{
    const std::string status = [&]() -> std::string
    {
        if (p.getState() == ActuatorStatus::State::READY)
        {
            return "READY";
        }
        else if (p.getState() == ActuatorStatus::State::BUSY)
        {
            return "BUSY";
        }
        else if (p.getState() == ActuatorStatus::State::ERROR)
        {
            return "ERROR";
        }

        return "ERROR";
    }();

    j = json{{"status", status}, {"value", p.getValue()}};
}

static void to_json(json& j, const std::shared_ptr<ActuatorStatus>& p)
{
    if (!p)
    {
        return;
    }

    to_json(j, *p);
}

JsonProtocol::JsonProtocol(bool isGateway) : m_isGateway{isGateway} {}

std::vector<std::string> JsonProtocol::getInboundChannels() const
{
    if (m_isGateway)
    {
        return {ACTUATION_SET_TOPIC_ROOT + GATEWAY_PATH_PREFIX + CHANNEL_MULTI_LEVEL_WILDCARD,
                CONFIGURATION_SET_TOPIC_ROOT + GATEWAY_PATH_PREFIX + CHANNEL_MULTI_LEVEL_WILDCARD,
                ACTUATION_SET_TOPIC_ROOT + DEVICE_PATH_PREFIX + CHANNEL_SINGLE_LEVEL_WILDCARD + CHANNEL_DELIMITER +
                  REFERENCE_PATH_PREFIX + CHANNEL_MULTI_LEVEL_WILDCARD,
                CONFIGURATION_SET_TOPIC_ROOT + DEVICE_PATH_PREFIX + CHANNEL_MULTI_LEVEL_WILDCARD};
    }
    else
    {
        return {ACTUATION_SET_TOPIC_ROOT + DEVICE_PATH_PREFIX + CHANNEL_SINGLE_LEVEL_WILDCARD + CHANNEL_DELIMITER +
                  REFERENCE_PATH_PREFIX + CHANNEL_MULTI_LEVEL_WILDCARD,
                CONFIGURATION_SET_TOPIC_ROOT + DEVICE_PATH_PREFIX + CHANNEL_MULTI_LEVEL_WILDCARD};
    }
}

std::vector<std::string> JsonProtocol::getInboundChannelsForDevice(const std::string& deviceKey) const
{
    if (m_isGateway)
    {
        return {
          ACTUATION_SET_TOPIC_ROOT + GATEWAY_PATH_PREFIX + deviceKey + CHANNEL_DELIMITER + CHANNEL_MULTI_LEVEL_WILDCARD,
          CONFIGURATION_SET_TOPIC_ROOT + GATEWAY_PATH_PREFIX + deviceKey + CHANNEL_DELIMITER +
            CHANNEL_MULTI_LEVEL_WILDCARD,
          ACTUATION_SET_TOPIC_ROOT + DEVICE_PATH_PREFIX + deviceKey + CHANNEL_DELIMITER + REFERENCE_PATH_PREFIX +
            CHANNEL_MULTI_LEVEL_WILDCARD,
          CONFIGURATION_SET_TOPIC_ROOT + DEVICE_PATH_PREFIX + deviceKey};
    }
    else
    {
        return {ACTUATION_SET_TOPIC_ROOT + DEVICE_PATH_PREFIX + deviceKey + CHANNEL_DELIMITER + REFERENCE_PATH_PREFIX +
                  CHANNEL_MULTI_LEVEL_WILDCARD,
                CONFIGURATION_SET_TOPIC_ROOT + DEVICE_PATH_PREFIX + deviceKey};
    }
}

std::unique_ptr<Message> JsonProtocol::makeMessage(
  const std::string& deviceKey, const std::vector<std::shared_ptr<SensorReading>>& sensorReadings) const
{
    if (sensorReadings.empty())
    {
        return nullptr;
    }

    // Check if every reference is the same
    const auto firstRef = sensorReadings.front()->getReference();
    bool anyDifferent =
      std::any_of(sensorReadings.cbegin(), sensorReadings.cend(),
                  [&](const std::shared_ptr<SensorReading>& reading) { return firstRef != reading->getReference(); });

    // Create the place for topic and payload
    std::string topic;
    json payload;

    // If all references are the same, just make a topic for that sensor and send each reading as data at different
    // times
    if (!anyDifferent)
    {
        const auto& messageKey = sensorReadings.front()->getKey();
        if (m_isGateway && deviceKey != messageKey && !messageKey.empty())
            topic = SENSOR_READING_TOPIC_ROOT + GATEWAY_PATH_PREFIX + deviceKey + CHANNEL_DELIMITER +
                    DEVICE_PATH_PREFIX + messageKey + CHANNEL_DELIMITER + REFERENCE_PATH_PREFIX + firstRef;
        else
            topic = SENSOR_READING_TOPIC_ROOT + DEVICE_PATH_PREFIX + deviceKey + CHANNEL_DELIMITER +
                    REFERENCE_PATH_PREFIX + firstRef;

        std::vector<json> entries(sensorReadings.size());
        std::transform(sensorReadings.begin(), sensorReadings.end(), entries.begin(),
                       [&](const std::shared_ptr<SensorReading>& sensorReading) -> json
                       {
                           const std::vector<std::string> readingValues = sensorReading->getValues();

                           std::string data;

                           if (readingValues.size() > 1)
                           {
                               data = joinMultiValues(readingValues, MULTIVALUE_READING_DELIMITER);
                           }
                           else
                           {
                               data = readingValues.front();
                           }

                           if (sensorReading->getRtc() == 0)
                           {
                               return json{{"data", data}};
                           }
                           else
                           {
                               return json{{"utc", sensorReading->getRtc()}, {"data", data}};
                           }
                       });

        payload = json(entries);
    }
    // If not, form a message for the generic topic that allows multiple references at the same time
    else
    {
        const auto& messageKey = sensorReadings.front()->getKey();
        if (m_isGateway && deviceKey != messageKey && !messageKey.empty())
            topic = SENSOR_READING_TOPIC_ROOT + GATEWAY_PATH_PREFIX + deviceKey + CHANNEL_DELIMITER +
                    DEVICE_PATH_PREFIX + messageKey;
        else
            topic = SENSOR_READING_TOPIC_ROOT + DEVICE_PATH_PREFIX + deviceKey;

        // We have to sort the entries by the UTC
        std::map<uint64_t, std::vector<std::shared_ptr<SensorReading>>> sensorReadingsByUTC;
        for (const auto& sensorReading : sensorReadings)
        {
            // Check if we already have the UTC
            if (sensorReadingsByUTC.find(sensorReading->getRtc()) != sensorReadingsByUTC.cend())
                sensorReadingsByUTC[sensorReading->getRtc()].emplace_back(sensorReading);
            else
                sensorReadingsByUTC[sensorReading->getRtc()] = {sensorReading};
        }

        // Now, create a json object for each UTC
        std::vector<json> entries(sensorReadingsByUTC.size());
        std::transform(sensorReadingsByUTC.cbegin(), sensorReadingsByUTC.cend(), entries.begin(),
                       [&](const std::pair<uint64_t, std::vector<std::shared_ptr<SensorReading>>>& pair) -> json
                       {
                           // Create the object, set the utc right away
                           auto entryJson = json{{"utc", pair.first}};

                           // Append all sensor readings in this UTC
                           for (const auto& reading : pair.second)
                           {
                               const std::vector<std::string> readingValues = reading->getValues();
                               if (readingValues.size() > 1)
                                   entryJson[reading->getReference()] =
                                     joinMultiValues(readingValues, MULTIVALUE_READING_DELIMITER);
                               else
                                   entryJson[reading->getReference()] = readingValues.front();
                           }

                           return entryJson;
                       });

        payload = json(entries);
    }

    // Create the message that will be sent, parse all objects in json vector to stringify into a json array
    return std::unique_ptr<Message>(new Message(json(payload).dump(), topic));
}

std::unique_ptr<Message> JsonProtocol::makeMessage(const std::string& deviceKey,
                                                   const std::vector<std::shared_ptr<Alarm>>& alarms) const
{
    if (alarms.empty())
    {
        return nullptr;
    }

    // Check if every reference is the same
    const auto firstRef = alarms.front()->getReference();
    bool anyDifferent =
      std::any_of(alarms.cbegin(), alarms.cend(),
                  [&](const std::shared_ptr<Alarm>& reading) { return firstRef != reading->getReference(); });

    // Create the place for topic and payload
    std::string topic;
    json payload;

    // If all references are the same, just make a topic for that alarm and send each reading as data at different
    // times
    if (!anyDifferent)
    {
        topic = EVENT_TOPIC_ROOT + DEVICE_PATH_PREFIX + deviceKey + CHANNEL_DELIMITER + REFERENCE_PATH_PREFIX +
                alarms.front()->getReference();

        std::vector<json> entries(alarms.size());
        std::transform(alarms.begin(), alarms.end(), entries.begin(),
                       [&](const std::shared_ptr<Alarm>& alarm) -> json
                       {
                           const std::vector<std::string> readingValues = alarm->getValues();

                           std::string data = alarm->getValue();

                           if (alarm->getRtc() == 0)
                           {
                               return json{{"data", data}};
                           }
                           else
                           {
                               return json{{"utc", alarm->getRtc()}, {"data", data}};
                           }
                       });

        payload = json(entries);
    }
    // If not, form a message for the generic topic that allows multiple references at the same time
    else
    {
        topic = EVENT_TOPIC_ROOT + DEVICE_PATH_PREFIX + deviceKey;

        // We have to sort the entries by the UTC
        std::map<uint64_t, std::vector<std::shared_ptr<Alarm>>> alarmsByUTC;
        for (const auto& alarm : alarms)
        {
            // Check if we already have the UTC
            if (alarmsByUTC.find(alarm->getRtc()) != alarmsByUTC.cend())
                alarmsByUTC[alarm->getRtc()].emplace_back(alarm);
            else
                alarmsByUTC[alarm->getRtc()] = {alarm};
        }

        // Now, create a json object for each UTC
        std::vector<json> entries(alarmsByUTC.size());
        std::transform(alarmsByUTC.cbegin(), alarmsByUTC.cend(), entries.begin(),
                       [&](const std::pair<uint64_t, std::vector<std::shared_ptr<Alarm>>>& pair) -> json
                       {
                           // Create the object, set the utc right away
                           auto entryJson = json{{"utc", pair.first}};

                           // Append all sensor readings in this UTC
                           for (const auto& alarm : pair.second)
                           {
                               entryJson[alarm->getReference()] = alarm->getValue();
                           }

                           return entryJson;
                       });

        payload = json(entries);
    }

    return std::unique_ptr<Message>(new Message(payload.dump(), topic));
}

std::unique_ptr<Message> JsonProtocol::makeMessage(
  const std::string& deviceKey, const std::vector<std::shared_ptr<ActuatorStatus>>& actuatorStatuses) const
{
    // JSON_SINGLE allows only 1 ActuatorStatus per Message
    if (actuatorStatuses.size() != 1)
    {
        return nullptr;
    }

    const json jPayload(actuatorStatuses.front());
    const std::string payload = jPayload.dump();
    const std::string topic = ACTUATION_STATUS_TOPIC_ROOT + DEVICE_PATH_PREFIX + deviceKey + CHANNEL_DELIMITER +
                              REFERENCE_PATH_PREFIX + actuatorStatuses.front()->getReference();

    return std::unique_ptr<Message>(new Message(payload, topic));
}

std::unique_ptr<Message> JsonProtocol::makeMessage(const std::string& deviceKey,
                                                   const std::vector<ConfigurationItem>& configuration) const
{
    json data{};

    for (const auto& item : configuration)
    {
        data.emplace(item.getReference(),
                     [&]
                     {
                         if (item.getValues().size() == 1)
                             return item.getValues().at(0);

                         return joinMultiValues(item.getValues(), MULTIVALUE_READING_DELIMITER);
                     }());
    }

    const json jPayload{{"values", data}};
    const std::string payload = jPayload.dump();
    const std::string topic = CONFIGURATION_STATUS_TOPIC_ROOT + DEVICE_PATH_PREFIX + deviceKey;

    return std::unique_ptr<Message>(new Message(payload, topic));
}

std::unique_ptr<ActuatorSetCommand> JsonProtocol::makeActuatorSetCommand(const Message& message) const
{
    try
    {
        json j = json::parse(message.getContent());

        const std::string value = [&]() -> std::string
        {
            if (j.find("value") != j.end())
            {
                return j.at("value").get<std::string>();
            }

            return "";
        }();

        const auto reference = extractReferenceFromChannel(message.getChannel());
        if (reference.empty())
        {
            return nullptr;
        }

        return std::unique_ptr<ActuatorSetCommand>(new ActuatorSetCommand(reference, value));
    }
    catch (...)
    {
        LOG(DEBUG) << "Unable to parse ActuatorSetCommand: " << message.getContent();
        return nullptr;
    }
}

std::unique_ptr<ConfigurationSetCommand> JsonProtocol::makeConfigurationSetCommand(const Message& message) const
{
    try
    {
        json j = json::parse(message.getContent());

        std::vector<ConfigurationItem> items;
        if (j.is_object())
        {
            for (const auto& configurationEntry : j.get<json::object_t>())
            {
                const std::string reference = configurationEntry.first;

                const auto values =
                  parseMultiValues(configurationEntry.second.get<std::string>(), MULTIVALUE_READING_DELIMITER);

                items.push_back(ConfigurationItem{values, reference});
            }
        }

        return std::unique_ptr<ConfigurationSetCommand>(new ConfigurationSetCommand(items));
    }
    catch (...)
    {
        LOG(DEBUG) << "Unable to parse ConfigurationSetCommand: " << message.getContent();
        return nullptr;
    }
}

bool JsonProtocol::isActuatorSetMessage(const Message& message) const
{
    return StringUtils::startsWith(message.getChannel(), ACTUATION_SET_TOPIC_ROOT);
}

bool JsonProtocol::isConfigurationSetMessage(const Message& message) const
{
    return StringUtils::startsWith(message.getChannel(), CONFIGURATION_SET_TOPIC_ROOT);
}

std::string JsonProtocol::extractReferenceFromChannel(const std::string& topic) const
{
    LOG(TRACE) << METHOD_INFO;

    std::string top{topic};

    if (StringUtils::endsWith(topic, CHANNEL_DELIMITER))
    {
        top = top.substr(0, top.size() - CHANNEL_DELIMITER.size());
    }

    const std::string referencePathPrefix = CHANNEL_DELIMITER + REFERENCE_PATH_PREFIX;

    auto pos = top.rfind(referencePathPrefix);

    if (pos != std::string::npos)
    {
        return top.substr(pos + referencePathPrefix.size(), std::string::npos);
    }

    return "";
}

std::string JsonProtocol::extractDeviceKeyFromChannel(const std::string& topic) const
{
    LOG(TRACE) << METHOD_INFO;

    const std::string devicePathPrefix = CHANNEL_DELIMITER + DEVICE_PATH_PREFIX;

    const auto deviceKeyStartPosition = topic.find(devicePathPrefix);
    if (deviceKeyStartPosition != std::string::npos)
    {
        const auto keyEndPosition = topic.find(CHANNEL_DELIMITER, deviceKeyStartPosition + devicePathPrefix.size());

        const auto pos = deviceKeyStartPosition + devicePathPrefix.size();

        return topic.substr(pos, keyEndPosition - pos);
    }

    const std::string gatewayPathPrefix = CHANNEL_DELIMITER + GATEWAY_PATH_PREFIX;

    const auto gatewayKeyStartPosition = topic.find(gatewayPathPrefix);
    if (gatewayKeyStartPosition == std::string::npos)
    {
        return "";
    }

    const auto keyEndPosition = topic.find(CHANNEL_DELIMITER, gatewayKeyStartPosition + gatewayPathPrefix.size());

    const auto pos = gatewayKeyStartPosition + gatewayPathPrefix.size();

    return topic.substr(pos, keyEndPosition - pos);
}

std::string JsonProtocol::joinMultiValues(const std::vector<std::string>& values, const std::string& delimiter) const
{
    return std::accumulate(values.begin(), values.end(), std::string{},
                           [&](std::string& first, const std::string& second)
                           { return first.empty() ? second : first + delimiter + second; });
}

std::vector<std::string> JsonProtocol::parseMultiValues(const std::string& values, const std::string& delimiter) const
{
    return StringUtils::tokenize(values, delimiter);
}
}    // namespace wolkabout

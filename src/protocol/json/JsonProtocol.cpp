/*
 * Copyright 2018 WolkAbout Technology s.r.o.
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

#include "protocol/json/JsonProtocol.h"
#include "model/ActuatorGetCommand.h"
#include "model/ActuatorSetCommand.h"
#include "model/ActuatorStatus.h"
#include "model/Alarm.h"
#include "model/ConfigurationItem.h"
#include "model/ConfigurationSetCommand.h"
#include "model/Message.h"
#include "model/SensorReading.h"
#include "utilities/Logger.h"
#include "utilities/StringUtils.h"
#include "utilities/json.hpp"
#include <algorithm>

using nlohmann::json;

namespace wolkabout
{
const std::string JsonProtocol::NAME = "JsonProtocol";

const std::string JsonProtocol::CHANNEL_DELIMITER = "/";
const std::string JsonProtocol::CHANNEL_WILDCARD = "#";

const std::string JsonProtocol::DEVICE_TYPE = "d";
const std::string JsonProtocol::REFERENCE_TYPE = "r";
const std::string JsonProtocol::DEVICE_TO_PLATFORM_TYPE = "d2p";
const std::string JsonProtocol::PLATFORM_TO_DEVICE_TYPE = "p2d";

const std::string JsonProtocol::DEVICE_PATH_PREFIX = "d/";
const std::string JsonProtocol::REFERENCE_PATH_PREFIX = "r/";
const std::string JsonProtocol::DEVICE_TO_PLATFORM_DIRECTION = "d2p/";
const std::string JsonProtocol::PLATFORM_TO_DEVICE_DIRECTION = "p2d/";

const std::string JsonProtocol::SENSOR_READING_TOPIC_ROOT = "d2p/sensor_reading/";
const std::string JsonProtocol::EVENTS_TOPIC_ROOT = "d2p/events/";
const std::string JsonProtocol::ACTUATION_STATUS_TOPIC_ROOT = "d2p/actuator_status/";
const std::string JsonProtocol::CONFIGURATION_RESPONSE_TOPIC_ROOT = "d2p/configuration_get/";

const std::string JsonProtocol::ACTUATION_SET_TOPIC_ROOT = "p2d/actuator_set/";
const std::string JsonProtocol::ACTUATION_GET_TOPIC_ROOT = "p2d/actuator_get/";
const std::string JsonProtocol::CONFIGURATION_SET_REQUEST_TOPIC_ROOT = "p2d/configuration_set/";
const std::string JsonProtocol::CONFIGURATION_GET_REQUEST_TOPIC_ROOT = "p2d/configuration_get/";

const std::vector<std::string> JsonProtocol::INBOUND_CHANNELS = {
  ACTUATION_GET_TOPIC_ROOT + CHANNEL_WILDCARD, ACTUATION_SET_TOPIC_ROOT + CHANNEL_WILDCARD,
  CONFIGURATION_GET_REQUEST_TOPIC_ROOT + CHANNEL_WILDCARD, CONFIGURATION_SET_REQUEST_TOPIC_ROOT + CHANNEL_WILDCARD};

void to_json(json& j, const SensorReading& p)
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

void to_json(json& j, const std::shared_ptr<SensorReading>& p)
{
    if (!p)
    {
        return;
    }

    to_json(j, *p);
}

void to_json(json& j, const Alarm& p)
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

void to_json(json& j, const std::shared_ptr<Alarm>& p)
{
    if (!p)
    {
        return;
    }

    to_json(j, *p);
}

void to_json(json& j, const ActuatorStatus& p)
{
    const std::string status = [&]() -> std::string {
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

void to_json(json& j, const std::shared_ptr<ActuatorStatus>& p)
{
    if (!p)
    {
        return;
    }

    to_json(j, *p);
}

const std::string& JsonProtocol::getName() const
{
    return NAME;
}

const std::vector<std::string>& JsonProtocol::getInboundChannels() const
{
    return INBOUND_CHANNELS;
}

std::unique_ptr<Message> JsonProtocol::makeMessage(const std::string& deviceKey,
                                                   const std::vector<std::shared_ptr<SensorReading>>& sensorReadings,
                                                   const std::string& delimiter) const
{
    if (sensorReadings.size() == 0)
    {
        return nullptr;
    }

    const std::string topic = SENSOR_READING_TOPIC_ROOT + DEVICE_PATH_PREFIX + deviceKey + CHANNEL_DELIMITER +
                              REFERENCE_PATH_PREFIX + sensorReadings.front()->getReference();

    if (delimiter.empty())
    {
        const json jPayload(sensorReadings);
        const std::string payload = jPayload.dump();

        return std::unique_ptr<Message>(new Message(payload, topic));
    }

    std::vector<json> payload(sensorReadings.size());
    std::transform(sensorReadings.begin(), sensorReadings.end(), payload.begin(),
                   [&](const std::shared_ptr<SensorReading>& sensorReading) -> json {
                       const std::vector<std::string> readingValues = sensorReading->getValues();

                       const std::string data = joinReadings(readingValues, delimiter);

                       if (sensorReading->getRtc() == 0)
                       {
                           return json{{"data", data}};
                       }
                       else
                       {
                           return json{{"utc", sensorReading->getRtc()}, {"data", data}};
                       }
                   });

    const json jPayload(payload);
    const std::string content = jPayload.dump();

    return std::unique_ptr<Message>(new Message(content, topic));
}

std::unique_ptr<Message> JsonProtocol::makeMessage(const std::string& deviceKey,
                                                   const std::vector<std::shared_ptr<Alarm>>& alarms) const
{
    if (alarms.size() == 0)
    {
        return nullptr;
    }

    const json jPayload(alarms);
    const std::string payload = jPayload.dump();
    const std::string topic = EVENTS_TOPIC_ROOT + DEVICE_PATH_PREFIX + deviceKey + CHANNEL_DELIMITER +
                              REFERENCE_PATH_PREFIX + alarms.front()->getReference();

    return std::unique_ptr<Message>(new Message(payload, topic));
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
                                                   const std::vector<ConfigurationItem>& configuration,
                                                   const std::map<std::string, std::string>& delimiters) const
{
    json data{};

    for (const auto& item : configuration)
    {
        data.emplace(item.getReference(), [&] {
            if (item.getValues().size() == 1)
                return item.getValues().at(0);

            auto delimiterIt = delimiters.find(item.getReference());

            if (delimiterIt != delimiters.end() && item.getValues().size() > 1)
                return joinReadings(item.getValues(), delimiterIt->second);

            return std::string{""};
        }());
    }

    const json jPayload{{"values", data}};
    const std::string payload = jPayload.dump();
    const std::string topic = CONFIGURATION_RESPONSE_TOPIC_ROOT + DEVICE_PATH_PREFIX + deviceKey;

    return std::unique_ptr<Message>(new Message(payload, topic));
}

std::unique_ptr<ActuatorSetCommand> JsonProtocol::makeActuatorSetCommand(const Message& message) const
{
    try
    {
        json j = json::parse(message.getContent());

        const std::string value = [&]() -> std::string {
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

std::unique_ptr<ActuatorGetCommand> JsonProtocol::makeActuatorGetCommand(const Message& message) const
{
    try
    {
        const auto reference = extractReferenceFromChannel(message.getChannel());
        if (reference.empty())
        {
            return nullptr;
        }

        return std::unique_ptr<ActuatorGetCommand>(new ActuatorGetCommand(reference));
    }
    catch (...)
    {
        LOG(DEBUG) << "Unable to parse ActuatorGetCommand: " << message.getContent();
        return nullptr;
    }
}

std::unique_ptr<ConfigurationSetCommand> JsonProtocol::makeConfigurationSetCommand(const Message& message) const
{
    try
    {
        json j = json::parse(message.getContent());

        std::map<std::string, std::string> values;
        if (j.is_object())
        {
            for (const auto& configurationEntry : j.get<json::object_t>())
            {
                values[configurationEntry.first] = configurationEntry.second.get<std::string>();
            }
        }

        return std::unique_ptr<ConfigurationSetCommand>(new ConfigurationSetCommand(values));
    }
    catch (...)
    {
        LOG(DEBUG) << "Unable to parse ConfigurationSetCommand: " << message.getContent();
        return nullptr;
    }
}

bool JsonProtocol::isActuatorSetMessage(const std::string& channel) const
{
    return StringUtils::startsWith(channel, ACTUATION_SET_TOPIC_ROOT);
}

bool JsonProtocol::isActuatorGetMessage(const std::string& channel) const
{
    return StringUtils::startsWith(channel, ACTUATION_GET_TOPIC_ROOT);
}

bool JsonProtocol::isConfigurationSetMessage(const std::string& channel) const
{
    return StringUtils::startsWith(channel, CONFIGURATION_SET_REQUEST_TOPIC_ROOT);
}

bool JsonProtocol::isConfigurationGetMessage(const std::string& channel) const
{
    return StringUtils::startsWith(channel, CONFIGURATION_GET_REQUEST_TOPIC_ROOT);
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

    return "";
}

std::string JsonProtocol::joinReadings(const std::vector<std::string>& values, const std::string& delimiter) const
{
    return std::accumulate(values.begin(), values.end(), std::string{},
                           [&](std::string& first, const std::string& second) {
                               return first.empty() ? second : first + delimiter + second;
                           });
}
}    // namespace wolkabout

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

#include "protocol/json/JsonSingleReferenceProtocol.h"
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

using nlohmann::json;

namespace wolkabout
{
const std::string JsonSingleReferenceProtocol::NAME = "JsonSingleReferenceProtocol";

const std::string JsonSingleReferenceProtocol::CHANNEL_DELIMITER = "/";
const std::string JsonSingleReferenceProtocol::CHANNEL_WILDCARD = "#";

const std::string JsonSingleReferenceProtocol::SENSOR_READING_TOPIC_ROOT = "readings/";
const std::string JsonSingleReferenceProtocol::ALARMS_TOPIC_ROOT = "events/";
const std::string JsonSingleReferenceProtocol::ACTUATION_STATUS_TOPIC_ROOT = "actuators/status/";
const std::string JsonSingleReferenceProtocol::CONFIGURATION_RESPONSE_TOPIC_ROOT = "configurations/current/";

const std::string JsonSingleReferenceProtocol::ACTUATION_COMMAND_TOPIC_ROOT = "actuators/commands/";
const std::string JsonSingleReferenceProtocol::CONFIGURATION_COMMAND_TOPIC_ROOT = "configurations/commands/";

const std::string JsonSingleReferenceProtocol::MULTIVALUE_READING_DELIMITER = ",";

// avoid collision with other protocol to_json functions
namespace serializer_jsr
{
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
}    // namespace serializer_jsr

const std::string& JsonSingleReferenceProtocol::getName() const
{
    return NAME;
}

std::vector<std::string> JsonSingleReferenceProtocol::getInboundChannels() const
{
    return {ACTUATION_COMMAND_TOPIC_ROOT + CHANNEL_WILDCARD, CONFIGURATION_COMMAND_TOPIC_ROOT + CHANNEL_WILDCARD};
}

std::vector<std::string> JsonSingleReferenceProtocol::getInboundChannelsForDevice(const std::string& deviceKey) const
{
    return {ACTUATION_COMMAND_TOPIC_ROOT + deviceKey + CHANNEL_DELIMITER + CHANNEL_WILDCARD,
            CONFIGURATION_COMMAND_TOPIC_ROOT + deviceKey};
}

std::unique_ptr<Message> JsonSingleReferenceProtocol::makeMessage(
  const std::string& deviceKey, const std::vector<std::shared_ptr<SensorReading>>& sensorReadings) const
{
    if (sensorReadings.size() == 0)
    {
        return nullptr;
    }

    const std::string topic =
      SENSOR_READING_TOPIC_ROOT + deviceKey + CHANNEL_DELIMITER + sensorReadings.front()->getReference();

    std::vector<json> payload(sensorReadings.size());

    std::transform(sensorReadings.begin(), sensorReadings.end(), payload.begin(),
                   [&](const std::shared_ptr<SensorReading>& sensorReading) -> json {
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

    const json jPayload(payload);
    const std::string content = jPayload.dump();

    return std::unique_ptr<Message>(new Message(content, topic));
}

std::unique_ptr<Message> JsonSingleReferenceProtocol::makeMessage(
  const std::string& deviceKey, const std::vector<std::shared_ptr<Alarm>>& alarms) const
{
    if (alarms.size() == 0)
    {
        return nullptr;
    }

    std::vector<json> payload(alarms.size());

    std::transform(alarms.begin(), alarms.end(), payload.begin(), [&](const std::shared_ptr<Alarm>& alarm) -> json {
        json j;
        serializer_jsr::to_json(j, alarm);

        return j;
    });

    const json jPayload(payload);
    const std::string content = jPayload.dump();
    const std::string topic = ALARMS_TOPIC_ROOT + deviceKey + CHANNEL_DELIMITER + alarms.front()->getReference();

    return std::unique_ptr<Message>(new Message(content, topic));
}

std::unique_ptr<Message> JsonSingleReferenceProtocol::makeMessage(
  const std::string& deviceKey, const std::vector<std::shared_ptr<ActuatorStatus>>& actuatorStatuses) const
{
    // JSON_SINGLE allows only 1 ActuatorStatus per Message
    if (actuatorStatuses.size() != 1)
    {
        return nullptr;
    }

    json jPayload;
    serializer_jsr::to_json(jPayload, actuatorStatuses.front());

    const std::string payload = jPayload.dump();
    const std::string topic =
      ACTUATION_STATUS_TOPIC_ROOT + deviceKey + CHANNEL_DELIMITER + actuatorStatuses.front()->getReference();

    return std::unique_ptr<Message>(new Message(payload, topic));
}

std::unique_ptr<Message> JsonSingleReferenceProtocol::makeMessage(
  const std::string& deviceKey, const std::vector<ConfigurationItem>& configuration) const
{
    json data{};

    for (const auto& item : configuration)
    {
        data.emplace(item.getReference(), [&] {
            if (item.getValues().size() == 1)
                return item.getValues().at(0);

            return joinMultiValues(item.getValues(), MULTIVALUE_READING_DELIMITER);
        }());
    }

    const json jPayload{{"values", data}};
    const std::string payload = jPayload.dump();
    const std::string topic = CONFIGURATION_RESPONSE_TOPIC_ROOT + deviceKey;

    return std::unique_ptr<Message>(new Message(payload, topic));
}

std::unique_ptr<ActuatorSetCommand> JsonSingleReferenceProtocol::makeActuatorSetCommand(const Message& message) const
{
    if (!isActuatorSetMessage(message))
    {
        return nullptr;
    }

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
        if (!reference.empty())
        {
            return std::unique_ptr<ActuatorSetCommand>(new ActuatorSetCommand(reference, value));
        }
    }
    catch (...)
    {
        LOG(DEBUG) << "Unable to parse ActuatorSetCommand: " << message.getContent();
    }

    return nullptr;
}

std::unique_ptr<ActuatorGetCommand> JsonSingleReferenceProtocol::makeActuatorGetCommand(const Message& message) const
{
    if (!isActuatorGetMessage(message))
    {
        return nullptr;
    }

    const auto reference = extractReferenceFromChannel(message.getChannel());
    if (!reference.empty())
    {
        return std::unique_ptr<ActuatorGetCommand>(new ActuatorGetCommand(reference));
    }

    return nullptr;
}

std::unique_ptr<ConfigurationSetCommand> JsonSingleReferenceProtocol::makeConfigurationSetCommand(
  const Message& message) const
{
    if (!isConfigurationSetMessage(message))
    {
        return nullptr;
    }

    try
    {
        json j = json::parse(message.getContent());

        std::vector<ConfigurationItem> items;
        if (j.find("values") != j.end() && j.at("values").is_object())
        {
            for (const auto& configurationEntry : j["values"].get<json::object_t>())
            {
                const std::string reference = configurationEntry.first;

                const auto values =
                  parseMultiValues(configurationEntry.second.get<std::string>(), MULTIVALUE_READING_DELIMITER);

                items.push_back(ConfigurationItem{values, reference});
            }

            return std::unique_ptr<ConfigurationSetCommand>(new ConfigurationSetCommand(items));
        }
    }
    catch (...)
    {
        LOG(DEBUG) << "Unable to parse ConfigurationSetCommand: " << message.getContent();
    }

    return nullptr;
}

bool JsonSingleReferenceProtocol::isActuatorSetMessage(const Message& message) const
{
    if (!StringUtils::startsWith(message.getChannel(), ACTUATION_COMMAND_TOPIC_ROOT))
    {
        return false;
    }

    try
    {
        json j = json::parse(message.getContent());
        const std::string type = j.at("command").get<std::string>();

        return type == "SET";
    }
    catch (...)
    {
    }

    return false;
}

bool JsonSingleReferenceProtocol::isActuatorGetMessage(const Message& message) const
{
    if (!StringUtils::startsWith(message.getChannel(), ACTUATION_COMMAND_TOPIC_ROOT))
    {
        return false;
    }

    try
    {
        json j = json::parse(message.getContent());
        const std::string type = j.at("command").get<std::string>();

        return type == "GET";
    }
    catch (...)
    {
    }

    return false;
}

bool JsonSingleReferenceProtocol::isConfigurationSetMessage(const Message& message) const
{
    if (!StringUtils::startsWith(message.getChannel(), CONFIGURATION_COMMAND_TOPIC_ROOT))
    {
        return false;
    }

    try
    {
        json j = json::parse(message.getContent());
        const std::string type = j.at("command").get<std::string>();

        return type == "SET";
    }
    catch (...)
    {
    }

    return false;
}

bool JsonSingleReferenceProtocol::isConfigurationGetMessage(const Message& message) const
{
    if (!StringUtils::startsWith(message.getChannel(), CONFIGURATION_COMMAND_TOPIC_ROOT))
    {
        return false;
    }

    try
    {
        json j = json::parse(message.getContent());
        const std::string type = j.at("command").get<std::string>();

        return type == "CURRENT";
    }
    catch (...)
    {
    }

    return false;
}

std::string JsonSingleReferenceProtocol::extractDeviceKeyFromChannel(const std::string& topic) const
{
    // remove / from end of channel
    const std::string channel =
      StringUtils::endsWith(topic, CHANNEL_DELIMITER) ? StringUtils::removeSufix(topic, CHANNEL_DELIMITER) : topic;

    if (StringUtils::startsWith(channel, ACTUATION_COMMAND_TOPIC_ROOT))
    {
        const auto endPosition = channel.find_last_of(CHANNEL_DELIMITER);

        if (endPosition != std::string::npos && endPosition > 0)
        {
            const auto startPosition = channel.find_last_of(CHANNEL_DELIMITER, endPosition - 1);

            if (startPosition != std::string::npos)
            {
                return channel.substr(startPosition + CHANNEL_DELIMITER.size(),
                                      endPosition - (startPosition + CHANNEL_DELIMITER.size()));
            }
        }
    }
    else if (StringUtils::startsWith(channel, CONFIGURATION_COMMAND_TOPIC_ROOT))
    {
        const auto position = channel.find_last_of(CHANNEL_DELIMITER);

        if (position != std::string::npos)
        {
            return channel.substr(position + CHANNEL_DELIMITER.size(), std::string::npos);
        }
    }

    return "";
}

std::string JsonSingleReferenceProtocol::extractReferenceFromChannel(const std::string& topic) const
{
    // remove / from end of channel
    const std::string channel =
      StringUtils::endsWith(topic, CHANNEL_DELIMITER) ? StringUtils::removeSufix(topic, CHANNEL_DELIMITER) : topic;

    if (StringUtils::startsWith(channel, ACTUATION_COMMAND_TOPIC_ROOT))
    {
        const auto position = channel.find_last_of(CHANNEL_DELIMITER);

        if (position != std::string::npos)
        {
            return channel.substr(position + CHANNEL_DELIMITER.size(), std::string::npos);
        }
    }

    return "";
}

std::string JsonSingleReferenceProtocol::joinMultiValues(const std::vector<std::string>& values,
                                                         const std::string& delimiter) const
{
    return std::accumulate(values.begin(), values.end(), std::string{},
                           [&](std::string& first, const std::string& second) {
                               return first.empty() ? second : first + delimiter + second;
                           });
}

std::vector<std::string> JsonSingleReferenceProtocol::parseMultiValues(const std::string& values,
                                                                       const std::string& delimiter) const
{
    return StringUtils::tokenize(values, delimiter);
}
}    // namespace wolkabout

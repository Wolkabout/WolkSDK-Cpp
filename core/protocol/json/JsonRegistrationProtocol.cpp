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

#include "protocol/json/JsonRegistrationProtocol.h"
#include "model/DeviceRegistrationRequest.h"
#include "model/DeviceRegistrationResponse.h"
#include "model/DeviceReregistrationResponse.h"
#include "model/Message.h"
#include "utilities/Logger.h"
#include "utilities/StringUtils.h"
#include "utilities/json.hpp"

#include <cassert>

using nlohmann::json;

namespace wolkabout
{
const std::string JsonRegistrationProtocol::NAME = "JsonRegistrationProtocol";

const std::string JsonRegistrationProtocol::CHANNEL_DELIMITER = "/";
const std::string JsonRegistrationProtocol::CHANNEL_WILDCARD = "#";
const std::string JsonRegistrationProtocol::DEVICE_PATH_PREFIX = "d/";
const std::string JsonRegistrationProtocol::DEVICE_TO_PLATFORM_DIRECTION = "d2p/";
const std::string JsonRegistrationProtocol::PLATFORM_TO_DEVICE_DIRECTION = "p2d/";

const std::string JsonRegistrationProtocol::DEVICE_REGISTRATION_REQUEST_TOPIC_ROOT = "d2p/register_device/";
const std::string JsonRegistrationProtocol::DEVICE_REGISTRATION_RESPONSE_TOPIC_ROOT = "p2d/register_device/";
const std::string JsonRegistrationProtocol::DEVICE_REREGISTRATION_REQUEST_TOPIC_ROOT = "p2d/reregister_device/";
const std::string JsonRegistrationProtocol::DEVICE_REREGISTRATION_RESPONSE_TOPIC_ROOT = "d2p/reregister_device/";

const std::vector<std::string> JsonRegistrationProtocol::INBOUND_CHANNELS = {
  DEVICE_REGISTRATION_RESPONSE_TOPIC_ROOT + DEVICE_PATH_PREFIX,
  DEVICE_REREGISTRATION_REQUEST_TOPIC_ROOT + DEVICE_PATH_PREFIX};

const std::string JsonRegistrationProtocol::REGISTRATION_RESPONSE_OK = "OK";
const std::string JsonRegistrationProtocol::REGISTRATION_RESPONSE_ERROR_KEY_CONFLICT = "ERROR_KEY_CONFLICT";
const std::string JsonRegistrationProtocol::REGISTRATION_RESPONSE_ERROR_MANIFEST_CONFLICT = "ERROR_MANIFEST_CONFLICT";
const std::string JsonRegistrationProtocol::REGISTRATION_RESPONSE_ERROR_MAX_NUMBER_OF_DEVICES_EXCEEDED =
  "ERROR_MAXIMUM_NUMBER_OF_DEVICES_EXCEEDED";
const std::string JsonRegistrationProtocol::REGISTRATION_RESPONSE_ERROR_READING_PAYLOAD = "ERROR_READING_PAYLOAD";
const std::string JsonRegistrationProtocol::REGISTRATION_RESPONSE_ERROR_GATEWAY_NOT_FOUND = "ERROR_GATEWAY_NOT_FOUND";
const std::string JsonRegistrationProtocol::REGISTRATION_RESPONSE_ERROR_NO_GATEWAY_MANIFEST =
  "ERROR_NO_GATEWAY_MANIFEST";

/*** CONFIGURATION MANIFEST ***/
void to_json(json& j, const ConfigurationManifest& configurationManifest)
{
    auto dataType = [&]() -> std::string {
        switch (configurationManifest.getDataType())
        {
        case ConfigurationManifest::DataType::BOOLEAN:
            return "BOOLEAN";

        case ConfigurationManifest::DataType::NUMERIC:
            return "NUMERIC";

        case ConfigurationManifest::DataType::STRING:
            return "STRING";

        default:
            throw std::invalid_argument("Invalid data type");
        }
    }();

    // clang-format off
    j = {
        {"defaultValue", configurationManifest.getDefaultValue()},
        {"dataType", dataType},
        {"description", configurationManifest.getDescription()},
        {"nullValue", configurationManifest.getNullValue()},
        {"reference", configurationManifest.getReference()},
        {"unit", configurationManifest.getUnit()},
        {"size", configurationManifest.getSize()},
        {"delimiter", configurationManifest.getDelimiter()},
        {"name", configurationManifest.getName()},
        {"maximum", configurationManifest.getMaximum()},
        {"minimum", configurationManifest.getMinimum()},
        {"labels", configurationManifest.getLabels()}
    };
    // clang-format on
}

void from_json(const json& j, ConfigurationManifest& configurationManifest)
{
    auto dataType = [&]() -> ConfigurationManifest::DataType {
        std::string dataTypeStr = j.at("dataType").get<std::string>();
        if (dataTypeStr == "STRING")
        {
            return ConfigurationManifest::DataType::STRING;
        }
        else if (dataTypeStr == "NUMERIC")
        {
            return ConfigurationManifest::DataType::NUMERIC;
        }
        else if (dataTypeStr == "BOOLEAN")
        {
            return ConfigurationManifest::DataType::BOOLEAN;
        }
        else
        {
            throw std::invalid_argument("Invalid data type");
        }
    }();

    // clang-format off
    configurationManifest =
            ConfigurationManifest(
                j.at("name").get<std::string>(),
                j.at("reference").get<std::string>(),
                j.at("description").get<std::string>(),
                j.at("unit").get<std::string>(),
                dataType,
                j.at("minimum").get<double>(),
                j.at("maximum").get<double>(),
                j.at("defaultValue").get<std::string>(),
                j.at("size").get<unsigned int>(),
                j.at("delimiter").get<std::string>(),
                j.at("labels").get<std::vector<std::string>>(),
                j.at("nullValue").get<std::string>()
            );
    // clang-format on
}
/*** CONFIGURATION MANIFEST ***/

/*** ALARM MANIFEST ***/
void to_json(json& j, const AlarmManifest& alarmManfiest)
{
    auto alarmSeverity = [&]() -> std::string {
        switch (alarmManfiest.getSeverity())
        {
        case AlarmManifest::AlarmSeverity::ALERT:
            return "ALERT";

        case AlarmManifest::AlarmSeverity::CRITICAL:
            return "CRITICAL";

        case AlarmManifest::AlarmSeverity::ERROR:
            return "ERROR";

        default:
            throw std::invalid_argument("Invalid alarm severity");
        }
    }();

    // clang-format off
    j = {
        {"reference", alarmManfiest.getReference()},
        {"severity", alarmSeverity},
        {"name", alarmManfiest.getName()},
        {"description", alarmManfiest.getDescription()},
        {"message", alarmManfiest.getMessage()}
    };
    // clang-format on
}

void from_json(const json& j, AlarmManifest& alarmManifest)
{
    auto alarmSeverity = [&]() -> AlarmManifest::AlarmSeverity {
        std::string severity = j.at("severity").get<std::string>();
        if (severity == "ALERT")
        {
            return AlarmManifest::AlarmSeverity::ALERT;
        }
        else if (severity == "ERROR")
        {
            return AlarmManifest::AlarmSeverity::ERROR;
        }
        else if (severity == "CRITICAL")
        {
            return AlarmManifest::AlarmSeverity::CRITICAL;
        }
        else
        {
            throw std::invalid_argument("Invalid alarm severity");
        }
    }();

    // clang-format off
    alarmManifest =
            AlarmManifest(j.at("name").get<std::string>(),
                          alarmSeverity,
                          j.at("reference").get<std::string>(),
                          j.at("message").get<std::string>(),
                          j.at("description").get<std::string>());
    // clang-format on
}
/*** ALARM MANIFEST ***/

/*** ACTUATOR MANIFEST ***/
void to_json(json& j, const ActuatorManifest& actuatorManfiest)
{
    auto dataType = [&]() -> std::string {
        switch (actuatorManfiest.getDataType())
        {
        case ActuatorManifest::DataType::BOOLEAN:
            return "BOOLEAN";

        case ActuatorManifest::DataType::NUMERIC:
            return "NUMERIC";

        case ActuatorManifest::DataType::STRING:
            return "STRING";

        default:
            throw std::invalid_argument("Invalid data type");
        }
    }();

    auto labels = [&]() -> std::string {
        bool first = true;
        std::stringstream ss;
        for (unsigned long i = 0; i < actuatorManfiest.getLabels().size(); ++i)
        {
            if (!first)
            {
                ss << actuatorManfiest.getDelimiter();
            }

            ss << actuatorManfiest.getLabels().at(i);
            first = false;
        }

        return ss.str();
    }();

    // clang-format off
    j = {
        {"dataType", dataType},
        {"precision", actuatorManfiest.getPrecision()},
        {"description", actuatorManfiest.getDescription()},
        {"readingType", actuatorManfiest.getReadingType()},
        {"labels", labels},
        {"reference", actuatorManfiest.getReference()},
        {"unit", actuatorManfiest.getUnit()},
        {"size", actuatorManfiest.getLabels().size() == 0 ? 1 : actuatorManfiest.getLabels().size()},
        {"delimiter", actuatorManfiest.getDelimiter()},
        {"name", actuatorManfiest.getName()},
        {"minimum", actuatorManfiest.getMinimum()},
        {"maximum", actuatorManfiest.getMaximum()}
    };
    // clang-format on
}

void from_json(const json& j, ActuatorManifest& actuatorManifest)
{
    auto dataType = [&]() -> ActuatorManifest::DataType {
        std::string dataTypeStr = j.at("dataType").get<std::string>();
        if (dataTypeStr == "STRING")
        {
            return ActuatorManifest::DataType::STRING;
        }
        else if (dataTypeStr == "NUMERIC")
        {
            return ActuatorManifest::DataType::NUMERIC;
        }
        else if (dataTypeStr == "BOOLEAN")
        {
            return ActuatorManifest::DataType::BOOLEAN;
        }
        else
        {
            throw std::invalid_argument("Invalid data type");
        }
    }();

    actuatorManifest = ActuatorManifest(
      j.at("name").get<std::string>(), j.at("reference").get<std::string>(), j.at("description").get<std::string>(),
      j.at("unit").get<std::string>(), j.at("readingType").get<std::string>(), dataType,
      j.at("precision").get<unsigned int>(), j.at("minimum").get<double>(), j.at("maximum").get<double>());

    std::string delimiter = j.at("delimiter").get<std::string>();
    std::string labelsStr = j.at("labels").get<std::string>();
    std::vector<std::string> labels = StringUtils::tokenize(labelsStr, delimiter);

    if (labels.size() > 0)
    {
        actuatorManifest.setLabels(labels);
        actuatorManifest.setDelimiter(delimiter);
    }
}
/*** ACTUATOR MANIFEST ***/

/*** SENSOR MANIFEST ***/
void to_json(json& j, const SensorManifest& sensorManifest)
{
    auto dataType = [&]() -> std::string {
        switch (sensorManifest.getDataType())
        {
        case SensorManifest::DataType::BOOLEAN:
            return "BOOLEAN";

        case SensorManifest::DataType::NUMERIC:
            return "NUMERIC";

        case SensorManifest::DataType::STRING:
            return "STRING";

        default:
            throw std::invalid_argument("Invalid data type");
        }
    }();

    auto labels = [&]() -> std::string {
        bool first = true;
        std::stringstream ss;
        for (unsigned long i = 0; i < sensorManifest.getLabels().size(); ++i)
        {
            if (!first)
            {
                ss << sensorManifest.getDelimiter();
            }

            ss << sensorManifest.getLabels().at(i);
            first = false;
        }

        return ss.str();
    }();

    // clang-format off
    j = {
        {"dataType", dataType},
        {"precision", sensorManifest.getPrecision()},
        {"description", sensorManifest.getDescription()},
        {"readingType", sensorManifest.getReadingType()},
        {"labels", labels},
        {"reference", sensorManifest.getReference()},
        {"unit", sensorManifest.getUnit()},
        {"size", sensorManifest.getLabels().size() == 0 ? 1 : sensorManifest.getLabels().size()},
        {"delimiter", sensorManifest.getDelimiter()},
        {"name", sensorManifest.getName()},
        {"minimum", sensorManifest.getMinimum()},
        {"maximum", sensorManifest.getMaximum()}
    };
    // clang-format on
}

void from_json(const json& j, SensorManifest& sensorManifest)
{
    auto dataType = [&]() -> SensorManifest::DataType {
        std::string dataTypeStr = j.at("dataType").get<std::string>();
        if (dataTypeStr == "STRING")
        {
            return SensorManifest::DataType::STRING;
        }
        else if (dataTypeStr == "NUMERIC")
        {
            return SensorManifest::DataType::NUMERIC;
        }
        else if (dataTypeStr == "BOOLEAN")
        {
            return SensorManifest::DataType::BOOLEAN;
        }
        else
        {
            throw std::invalid_argument("Invalid data type");
        }
    }();

    sensorManifest = SensorManifest(
      j.at("name").get<std::string>(), j.at("reference").get<std::string>(), j.at("description").get<std::string>(),
      j.at("unit").get<std::string>(), j.at("readingType").get<std::string>(), dataType,
      j.at("precision").get<unsigned int>(), j.at("minimum").get<double>(), j.at("maximum").get<double>());

    std::string delimiter = j.at("delimiter").get<std::string>();
    std::string labelsStr = j.at("labels").get<std::string>();
    std::vector<std::string> labels = StringUtils::tokenize(labelsStr, delimiter);

    if (labels.size() > 0)
    {
        sensorManifest.setLabels(labels);
        sensorManifest.setDelimiter(delimiter);
    }
}
/*** SENSOR MANIFEST ***/

/*** DEVICE MANIFEST ***/
void to_json(json& j, const DeviceManifest& deviceManifest)
{
    // clang-format off
    j = {
        {"name", deviceManifest.getName()},
        {"description", deviceManifest.getDescription()},
        {"protocol", deviceManifest.getProtocol()},
        {"firmwareUpdateProtocol", deviceManifest.getFirmwareUpdateProtocol()},
        {"configs", deviceManifest.getConfigurations()},
        {"alarms", deviceManifest.getAlarms()},
        {"actuators", deviceManifest.getActuators()},
        {"feeds", deviceManifest.getSensors()}
    };
    // clang-format on
}

void from_json(const json& j, DeviceManifest& deviceManifest)
{
    deviceManifest = DeviceManifest(
      j.at("name").get<std::string>(), j.at("description").get<std::string>(), j.at("protocol").get<std::string>(),
      j.at("firmwareUpdateProtocol").get<std::string>(), j.at("configs").get<std::vector<ConfigurationManifest>>(),
      j.at("feeds").get<std::vector<SensorManifest>>(), j.at("alarms").get<std::vector<AlarmManifest>>(),
      j.at("actuators").get<std::vector<ActuatorManifest>>());
}
/*** DEVICE MANIFEST ***/

/*** DEVICE REGISTRATION REQUEST DTO ***/
void to_json(json& j, const DeviceRegistrationRequest& dto)
{
    // clang-format off
    j = {
        {"device",
            {{"name", dto.getDeviceName()},
            {"key", dto.getDeviceKey()}}
        },
        {"manifest", dto.getManifest()}
    };
    // clang-format on
}

void from_json(const json& j, DeviceRegistrationRequest& dto)
{
    dto = DeviceRegistrationRequest(j.at("name").get<std::string>(), j.at("key").get<std::string>(),
                                    j.at("manifest").get<DeviceManifest>());
}
/*** DEVICE REGISTRATION REQUEST DTO ***/

/*** DEVICE REREGISTRATION RESPONSE DTO ***/
void to_json(json& j, const DeviceReregistrationResponse& dto)
{
    auto resultStr = [&]() -> std::string {
        switch (dto.getResult())
        {
        case DeviceReregistrationResponse::Result::OK:
            return "OK";
            break;

        default:
            throw std::invalid_argument("Unhandled result");
        }
    }();

    // clang-format off
    j = {
        {"result", resultStr}
    };
    // clang-format on
}
/*** DEVICE REREGISTRATION RESPONSE DTO ***/

const std::string& JsonRegistrationProtocol::getName() const
{
    return NAME;
}

std::vector<std::string> JsonRegistrationProtocol::getInboundChannels() const
{
    std::vector<std::string> channels;
    std::transform(INBOUND_CHANNELS.cbegin(), INBOUND_CHANNELS.cend(), std::back_inserter(channels),
                   [](const std::string& source) { return source + CHANNEL_WILDCARD; });
    return channels;
}

std::vector<std::string> JsonRegistrationProtocol::getInboundChannelsForDevice(const std::string& deviceKey) const
{
    std::vector<std::string> channels;
    std::transform(INBOUND_CHANNELS.cbegin(), INBOUND_CHANNELS.cend(), std::back_inserter(channels),
                   [&](const std::string& source) -> std::string { return source + deviceKey; });
    return channels;
}

std::string JsonRegistrationProtocol::extractDeviceKeyFromChannel(const std::string& topic) const
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

std::shared_ptr<Message> JsonRegistrationProtocol::makeMessage(const std::string& deviceKey,
                                                               const DeviceRegistrationRequest& request) const
{
    LOG(DEBUG) << METHOD_INFO;

    try
    {
        const json jsonPayload(request);
        std::string channel;

        channel = DEVICE_REGISTRATION_REQUEST_TOPIC_ROOT + DEVICE_PATH_PREFIX + deviceKey;

        return std::make_shared<Message>(jsonPayload.dump(), channel);
    }
    catch (...)
    {
        return nullptr;
    }
}

std::shared_ptr<DeviceRegistrationResponse> JsonRegistrationProtocol::makeRegistrationResponse(
  std::shared_ptr<Message> message) const
{
    LOG(TRACE) << METHOD_INFO;

    try
    {
        const json j = json::parse(message->getContent());

        const std::string typeStr = j.at("result").get<std::string>();

        const DeviceRegistrationResponse::Result result = [&] {
            if (typeStr == REGISTRATION_RESPONSE_OK)
            {
                return DeviceRegistrationResponse::Result::OK;
            }
            else if (typeStr == REGISTRATION_RESPONSE_ERROR_KEY_CONFLICT)
            {
                return DeviceRegistrationResponse::Result::ERROR_KEY_CONFLICT;
            }
            else if (typeStr == REGISTRATION_RESPONSE_ERROR_MANIFEST_CONFLICT)
            {
                return DeviceRegistrationResponse::Result::ERROR_MANIFEST_CONFLICT;
            }
            else if (typeStr == REGISTRATION_RESPONSE_ERROR_MAX_NUMBER_OF_DEVICES_EXCEEDED)
            {
                return DeviceRegistrationResponse::Result::ERROR_MAXIMUM_NUMBER_OF_DEVICES_EXCEEDED;
            }
            else if (typeStr == REGISTRATION_RESPONSE_ERROR_READING_PAYLOAD)
            {
                return DeviceRegistrationResponse::Result::ERROR_READING_PAYLOAD;
            }
            else if (typeStr == REGISTRATION_RESPONSE_ERROR_GATEWAY_NOT_FOUND)
            {
                return DeviceRegistrationResponse::Result::ERROR_GATEWAY_NOT_FOUND;
            }
            else if (typeStr == REGISTRATION_RESPONSE_ERROR_NO_GATEWAY_MANIFEST)
            {
                return DeviceRegistrationResponse::Result::ERROR_NO_GATEWAY_MANIFEST;
            }

            assert(false);
            throw std::logic_error("");
        }();

        return std::make_shared<DeviceRegistrationResponse>(result);
    }
    catch (std::exception& e)
    {
        LOG(ERROR) << "Device registration protocol: Unable to deserialize device "
                      "registration response: "
                   << e.what();
        return nullptr;
    }
}
}    // namespace wolkabout
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

#include "protocol/json/JsonDto.h"
#include "model/ActuatorManifest.h"
#include "model/AlarmManifest.h"
#include "model/ConfigurationManifest.h"
#include "model/DeviceRegistrationRequest.h"
#include "model/DeviceRegistrationResponse.h"
#include "model/DeviceReregistrationResponse.h"
#include "model/SensorManifest.h"

using nlohmann::json;

namespace wolkabout
{
/*** CONFIGURATION MANIFEST ***/
void to_json(json& j, const ConfigurationManifest& configurationManifest)
{
    auto dataType = [&]() -> std::string {
        auto dataTypeStr = toString(configurationManifest.getDataType());

        return dataTypeStr.empty() ? throw std::invalid_argument("Invalid data type") : dataTypeStr;
    }();

    json confJ;

    confJ["name"] = configurationManifest.getName();
    confJ["reference"] = configurationManifest.getReference();
    confJ["dataType"] = dataType;
    confJ["description"] = configurationManifest.getDescription();
    confJ["defaultValue"] = configurationManifest.getDefaultValue();
    confJ["size"] = configurationManifest.getSize();

    if (configurationManifest.getDataType() == DataType::NUMERIC)
    {
        confJ["minimum"] = configurationManifest.getMinimum();
        confJ["maximum"] = configurationManifest.getMaximum();
    }
    else
    {
        confJ["minimum"] = nullptr;
        confJ["maximum"] = nullptr;
    }

    if (configurationManifest.getSize() > 1)
    {
        confJ["labels"] = configurationManifest.getLabels();
    }
    else
    {
        confJ["labels"] = nullptr;
    }

    j = confJ;
}

void from_json(const json& j, ConfigurationManifest& configurationManifest)
{
    auto dataType = [&]() -> DataType {
        std::string dataTypeStr = j.at("dataType").get<std::string>();
        if (dataTypeStr == "STRING")
        {
            return DataType::STRING;
        }
        else if (dataTypeStr == "NUMERIC")
        {
            return DataType::NUMERIC;
        }
        else if (dataTypeStr == "BOOLEAN")
        {
            return DataType::BOOLEAN;
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
                dataType,
                j.at("description").get<std::string>(),
                j.at("defaultValue").get<std::string>(),
                j.at("labels").is_null() ? std::vector<std::string>{} : j.at("labels").get<std::vector<std::string>>(),
                j.at("minimum").is_null() ? 0 : j.at("minimum").get<double>(),
                j.at("maximum").is_null() ? 0 : j.at("maximum").get<double>()
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
    json actuatorJ;

    actuatorJ["name"] = actuatorManfiest.getName();
    actuatorJ["reference"] = actuatorManfiest.getReference();
    actuatorJ["description"] = actuatorManfiest.getDescription();

    actuatorJ["readingType"]["name"] = actuatorManfiest.getReadingTypeName();
    actuatorJ["readingType"]["dataType"] = toString(actuatorManfiest.getDataType());
    actuatorJ["readingType"]["precision"] = actuatorManfiest.getPrecision();
    actuatorJ["readingType"]["labels"] = actuatorManfiest.getLabels();
    actuatorJ["readingType"]["size"] = actuatorManfiest.getSize();

    actuatorJ["unit"]["readingType"] = actuatorManfiest.getReadingTypeName();
    if (!actuatorManfiest.getUnitSymbol().empty())
    {
        actuatorJ["unit"]["symbol"] = actuatorManfiest.getUnitSymbol();
    }
    else
    {
        actuatorJ["unit"]["symbol"] = nullptr;
    }

    if (actuatorManfiest.getDataType() == DataType::NUMERIC)
    {
        actuatorJ["minimum"] = actuatorManfiest.getMinimum();
        actuatorJ["maximum"] = actuatorManfiest.getMaximum();
    }
    else
    {
        actuatorJ["minimum"] = nullptr;
        actuatorJ["maximum"] = nullptr;
    }

    j = actuatorJ;
}

void from_json(const json& j, ActuatorManifest& actuatorManifest)
{
    auto dataType = [&]() -> DataType {
        std::string dataTypeStr = j.at("readingType").at("dataType").get<std::string>();
        if (dataTypeStr == "STRING")
        {
            return DataType::STRING;
        }
        else if (dataTypeStr == "NUMERIC")
        {
            return DataType::NUMERIC;
        }
        else if (dataTypeStr == "BOOLEAN")
        {
            return DataType::BOOLEAN;
        }
        else
        {
            throw std::invalid_argument("Invalid data type");
        }
    }();

    // clang-format off
    actuatorManifest = ActuatorManifest{
                j.at("name").get<std::string>(),
                j.at("reference").get<std::string>(),
                j["readingType"].at("name").get<std::string>(),
                j["unit"].at("symbol").is_null() ? "" : j["unit"].at("symbol").get<std::string>(),
                dataType,
                j["readingType"].at("precision").get<int>(),
                j.at("description").get<std::string>(),
                j["readingType"].at("labels").is_null() ? std::vector<std::string>{} : j["readingType"].at("labels").get<std::vector<std::string>>(),
                j.at("minimum").is_null() ? 0 : j.at("minimum").get<double>(),
                j.at("maximum").is_null() ? 0 : j.at("maximum").get<double>()};
    // clang-format on
}
/*** ACTUATOR MANIFEST ***/

/*** SENSOR MANIFEST ***/
void to_json(json& j, const SensorManifest& sensorManifest)
{
    json sensorJ;

    sensorJ["name"] = sensorManifest.getName();
    sensorJ["reference"] = sensorManifest.getReference();
    sensorJ["description"] = sensorManifest.getDescription();

    sensorJ["readingType"]["name"] = sensorManifest.getReadingTypeName();
    sensorJ["readingType"]["dataType"] = toString(sensorManifest.getDataType());
    sensorJ["readingType"]["precision"] = sensorManifest.getPrecision();
    sensorJ["readingType"]["labels"] = sensorManifest.getLabels();
    sensorJ["readingType"]["size"] = sensorManifest.getSize();

    sensorJ["unit"]["readingType"] = sensorManifest.getReadingTypeName();
    if (!sensorManifest.getUnitSymbol().empty())
    {
        sensorJ["unit"]["symbol"] = sensorManifest.getUnitSymbol();
    }
    else
    {
        sensorJ["unit"]["symbol"] = nullptr;
    }

    if (sensorManifest.getDataType() == DataType::NUMERIC)
    {
        sensorJ["minimum"] = sensorManifest.getMinimum();
        sensorJ["maximum"] = sensorManifest.getMaximum();
    }
    else
    {
        sensorJ["minimum"] = nullptr;
        sensorJ["maximum"] = nullptr;
    }

    j = sensorJ;
}

void from_json(const json& j, SensorManifest& sensorManifest)
{
    auto dataType = [&]() -> DataType {
        std::string dataTypeStr = j["readingType"].at("dataType").get<std::string>();
        if (dataTypeStr == "STRING")
        {
            return DataType::STRING;
        }
        else if (dataTypeStr == "NUMERIC")
        {
            return DataType::NUMERIC;
        }
        else if (dataTypeStr == "BOOLEAN")
        {
            return DataType::BOOLEAN;
        }
        else
        {
            throw std::invalid_argument("Invalid data type");
        }
    }();

    sensorManifest = SensorManifest{j.at("name").get<std::string>(),
                                    j.at("reference").get<std::string>(),
                                    j["readingType"].at("name").get<std::string>(),
                                    j["unit"].at("symbol").is_null() ? "" : j["unit"].at("symbol").get<std::string>(),
                                    dataType,
                                    j["readingType"].at("precision").get<int>(),
                                    j.at("description").get<std::string>(),
                                    j["readingType"].at("labels").is_null() ?
                                      std::vector<std::string>{} :
                                      j["readingType"].at("labels").get<std::vector<std::string>>(),
                                    j.at("minimum").is_null() ? 0 : j.at("minimum").get<double>(),
                                    j.at("maximum").is_null() ? 0 : j.at("maximum").get<double>()};
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
    dto =
      DeviceRegistrationRequest(j.at("device").at("name").get<std::string>(),
                                j.at("device").at("key").get<std::string>(), j.at("manifest").get<DeviceManifest>());
}
/*** DEVICE REGISTRATION REQUEST DTO ***/

/*** DEVICE REGISTRATION RESPONSE DTO ***/
void to_json(json& j, const DeviceRegistrationResponse& dto)
{
    auto resultStr = [&]() -> std::string {
        switch (dto.getResult())
        {
        case DeviceRegistrationResponse::Result::OK:
            return "OK";
            break;

        case DeviceRegistrationResponse::Result::ERROR_GATEWAY_NOT_FOUND:
            return "ERROR_GATEWAY_NOT_FOUND";
            break;

        case DeviceRegistrationResponse::Result::ERROR_KEY_CONFLICT:
            return "ERROR_KEY_CONFLICT";
            break;

        case DeviceRegistrationResponse::Result::ERROR_MANIFEST_CONFLICT:
            return "ERROR_MANIFEST_CONFLICT";
            break;

        case DeviceRegistrationResponse::Result::ERROR_MAXIMUM_NUMBER_OF_DEVICES_EXCEEDED:
            return "ERROR_MAXIMUM_NUMBER_OF_DEVICES_EXCEEDED";
            break;

        case DeviceRegistrationResponse::Result::ERROR_NO_GATEWAY_MANIFEST:
            return "ERROR_NO_GATEWAY_MANIFEST";
            break;

        case DeviceRegistrationResponse::Result::ERROR_READING_PAYLOAD:
            return "ERROR_READING_PAYLOAD";
            break;

        default:
            assert(false);
            throw std::invalid_argument("Unhandled result");
        }
    }();

    // clang-format off
    j = {
        {"result", resultStr}
    };
    // clang-format on
}
/*** DEVICE REGISTRATION RESPONSE DTO ***/

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
}    // namespace wolkabout

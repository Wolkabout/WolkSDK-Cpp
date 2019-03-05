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
#include "model/ActuatorTemplate.h"
#include "model/AlarmTemplate.h"
#include "model/ConfigurationTemplate.h"
#include "model/SensorTemplate.h"
#include "model/SubdeviceRegistrationRequest.h"
#include "model/SubdeviceRegistrationResponse.h"
#include "model/GatewayUpdateRequest.h"
#include "model/GatewayUpdateResponse.h"

using nlohmann::json;

namespace wolkabout
{
static std::string createMultivalue(const std::string& value, int size)
{
    std::string multivalue = "";
    for (int i = 0; i < size; ++i)
    {
        multivalue += value;
        if (i < size - 1)
        {
            multivalue += ",";
        }
    }

    return multivalue;
}

/*** CONFIGURATION TEMPLATE ***/
void to_json(json& j, const ConfigurationTemplate& configurationTemplate)
{
    auto dataType = [&]() -> std::string {
        auto dataTypeStr = toString(configurationTemplate.getDataType());

        return dataTypeStr.empty() ? throw std::invalid_argument("Invalid data type") : dataTypeStr;
    }();

    json confJ;

    confJ["name"] = configurationTemplate.getName();
    confJ["dataType"] = configurationTemplate.getDataType();
    confJ["reference"] = configurationTemplate.getReference();
    confJ["defaultValue"] = createMultivalue(configurationTemplate.getDefaultValue(), configurationTemplate.getSize());
    confJ["size"] = configurationTemplate.getSize();
    confJ["description"] = configurationTemplate.getDescription();

    if (configurationTemplate.getDataType() == DataType::NUMERIC)
    {
        confJ["minimum"] = configurationTemplate.getMinimum();
        confJ["maximum"] = configurationTemplate.getMaximum();
    }
    else
    {
        confJ["minimum"] = nullptr;
        confJ["maximum"] = nullptr;
    }

    if (configurationTemplate.getSize() > 1)
    {
        confJ["labels"] = configurationTemplate.getLabels();
    }
    else
    {
        confJ["labels"] = nullptr;
    }

    j = confJ;
}

void from_json(const json& j, ConfigurationTemplate& configurationTemplate)
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
    configurationTemplate =
            ConfigurationTemplate(
                j.at("name").get<std::string>(),
                j.at("reference").get<std::string>(),
                dataType,
                j.at("description").get<std::string>(),
                j.at("defaultValue").is_null() ? 0 : j.at("defaultValue").get<std::string>(),
                j.at("labels").is_null() ? std::vector<std::string>{} : j.at("labels").get<std::vector<std::string>>(),
                j.at("minimum").is_null() ? 0 : j.at("minimum").get<double>(),
                j.at("maximum").is_null() ? 0 : j.at("maximum").get<double>()
            );
    // clang-format on
}
/*** CONFIGURATION TEMPLATE ***/

/*** ALARM TEMPLATE ***/
void to_json(json& j, const AlarmTemplate& alarmTemplate)
{
    auto alarmSeverity = [&]() -> std::string {
        switch (alarmTemplate.getSeverity())
        {
        case AlarmTemplate::AlarmSeverity::ALERT:
            return "ALERT";

        case AlarmTemplate::AlarmSeverity::CRITICAL:
            return "CRITICAL";

        case AlarmTemplate::AlarmSeverity::ERROR:
            return "ERROR";

        default:
            throw std::invalid_argument("Invalid alarm severity");
        }
    }();

    // clang-format off
    j = {
        {"name", alarmTemplate.getName()},
        {"reference", alarmTemplate.getReference()},
        {"message", alarmTemplate.getMessage()}
        {"severity", alarmSeverity},
        {"description", alarmTemplate.getDescription()},
    };
    // clang-format on
}

void from_json(const json& j, AlarmTemplate& alarmTemplate)
{
    auto alarmSeverity = [&]() -> AlarmTemplate::AlarmSeverity {
        std::string severity = j.at("severity").get<std::string>();
        if (severity == "ALERT")
        {
            return AlarmTemplate::AlarmSeverity::ALERT;
        }
        else if (severity == "ERROR")
        {
            return AlarmTemplate::AlarmSeverity::ERROR;
        }
        else if (severity == "CRITICAL")
        {
            return AlarmTemplate::AlarmSeverity::CRITICAL;
        }
        else
        {
            throw std::invalid_argument("Invalid alarm severity");
        }
    }();

    // clang-format off
    alarmTemplate =
            AlarmTemplate(j.at("name").get<std::string>(),
                          alarmSeverity,
                          j.at("reference").get<std::string>(),
                          j.at("message").get<std::string>(),
                          j.at("description").get<std::string>());
    // clang-format on
}
/*** ALARM TEMPLATE ***/

/*** ACTUATOR TEMPLATE ***/
void to_json(json& j, const ActuatorTemplate& actuatorTemplate)
{
    json actuatorJ;

    actuatorJ["name"] = actuatorTemplate.getName();
    actuatorJ["reference"] = actuatorTemplate.getReference();
    actuatorJ["description"] = actuatorTemplate.getDescription();
    actuatorJ["unit"]["readingTypeName"] = actuatorTemplate.getReadingTypeName();

    if (!actuatorTemplate.getUnitSymbol().empty())
    {
        actuatorJ["unit"]["symbol"] = actuatorTemplate.getUnitSymbol();
    }
    else
    {
        actuatorJ["unit"]["symbol"] = nullptr;
    }

    if (actuatorTemplate.getDataType() == DataType::NUMERIC)
    {
        actuatorJ["minimum"] = actuatorTemplate.getMinimum();
        actuatorJ["maximum"] = actuatorTemplate.getMaximum();
    }
    else
    {
        actuatorJ["minimum"] = nullptr;
        actuatorJ["maximum"] = nullptr;
    }

    j = actuatorJ;
}

void from_json(const json& j, ActuatorTemplate& actuatorTemplate)
{
    // clang-format off
    actuatorTemplate = ActuatorTemplate{
                j.at("name").get<std::string>(),
                j.at("reference").get<std::string>(),
                j["unit"].at("readingTypeName").get<std::string>(),
                j["unit"].at("symbol").is_null() ? "" : j["unit"].at("symbol").get<std::string>(),
                j.at("description").get<std::string>(),
                j.at("minimum").is_null() ? 0 : j.at("minimum").get<double>(),
                j.at("maximum").is_null() ? 0 : j.at("maximum").get<double>()};
    // clang-format on
}
/*** ACTUATOR TEMPLATE ***/

/*** SENSOR TEMPLATE ***/
void to_json(json& j, const SensorTemplate& sensorTemplate)
{
    json sensorJ;

    sensorJ["name"] = sensorTemplate.getName();
    sensorJ["reference"] = sensorTemplate.getReference();
    sensorJ["description"] = sensorTemplate.getDescription();

    sensorJ["unit"]["readingTypeName"] = sensorTemplate.getReadingTypeName();
    sensorJ["unit"]["readingType"] = sensorTemplate.getReadingTypeName();
    if (!sensorTemplate.getUnitSymbol().empty())
    {
        sensorJ["unit"]["symbol"] = sensorTemplate.getUnitSymbol();
    }
    else
    {
        sensorJ["unit"]["symbol"] = nullptr;
    }

    if (sensorTemplate.getDataType() == DataType::NUMERIC)
    {
        sensorJ["minimum"] = sensorTemplate.getMinimum();
        sensorJ["maximum"] = sensorTemplate.getMaximum();
    }
    else
    {
        sensorJ["minimum"] = nullptr;
        sensorJ["maximum"] = nullptr;
    }

    j = sensorJ;
}

void from_json(const json& j, SensorTemplate& sensorTemplate)
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

    sensorTemplate = SensorTemplate{j.at("name").get<std::string>(),
                                    j.at("reference").get<std::string>(),
                                    j["unit"].at("readingTypeName").get<std::string>(),
                                    j["unit"].at("symbol").is_null() ? "" : j["unit"].at("symbol").get<std::string>(),
                                    dataType,
                                    j.at("description").get<std::string>(),
                                    j.at("minimum").is_null() ? 0 : j.at("minimum").get<double>(),
                                    j.at("maximum").is_null() ? 0 : j.at("maximum").get<double>()};
}
/*** SENSOR TEMPLATE ***/

/*** DEVICE TEMPLATE ***/
void to_json(json& j, const DeviceTemplate& deviceTemplate)
{
    // clang-format off
    j = {
        {"configurations", deviceTemplate.getConfigurations()},
        {"sensors", deviceTemplate.getSensors()},
        {"alarms", deviceTemplate.getAlarms()},
        {"actuators", deviceTemplate.getActuators()},
        {"firmwareUpdateType", deviceTemplate.getFirmwareUpdateType()},
        {"typeParemeters", deviceTemplate.getTypeParameters()},
        {"connectivityParemeters", deviceTemplate.getConectivityParameters()},
        {"firmwareUpdateParemeters", deviceTemplate.getFirmwareUpdateParameters()}
    };
    // clang-format on
}

void from_json(const json& j, DeviceTemplate& deviceTemplate)
{
    deviceTemplate = DeviceTemplate(
      j.at("configurations").get<std::vector<ConfigurationTemplate>>(),
      j.at("sensors").get<std::vector<SensorTemplate>>(), j.at("alarms").get<std::vector<AlarmTemplate>>(),
      j.at("actuators").get<std::vector<ActuatorTemplate>>(), j.at("firmwareUpdateType").get<std::string>(),
      j.at("typeParemeters").get<std::map<std::string, std::string>>(),
      j.at("connectivityParemeters").get<std::map<std::string, std::string>>(),
      j.at("firmwareUpdateParemeters").get<std::map<std::string, bool>>());
}
/*** DEVICE TEMPLATE ***/

/*** SUBDEVICE REGISTRATION REQUEST DTO ***/
void to_json(json& j, const SubdeviceRegistrationRequest& dto)
{
    // clang-format off
    j = {
        {"name", dto.getSubdeviceName()},
        {"key", dto.getSubdeviceKey()},
        {"defaultBinding", dto.getDefaultBinding()},
        {"configurations", dto.getTemplate().getConfigurations()},
        {"sensors", dto.getTemplate().getSensors()},
        {"alarms", dto.getTemplate().getAlarms()},
        {"actuators", dto.getTemplate().getActuators()},
        {"firmwareUpdateType", dto.getTemplate().getFirmwareUpdateType()},
        {"typeParemeters", dto.getTemplate().getTypeParameters()},
        {"connectivityParemeters", dto.getTemplate().getConectivityParameters()},
        {"firmwareUpdateParemeters", dto.getTemplate().getFirmwareUpdateParameters()}
    };
    // clang-format on
}

void from_json(const json& j, SubdeviceRegistrationRequest& dto)
{
    dto =
      SubdeviceRegistrationRequest(j.at("name").get<std::string>(),
                                j.at("key").get<std::string>(),
                                j.at("defaultBinding").get<bool>(),
                                DeviceTemplate{j.at("configurations").get<std::vector<ConfigurationTemplate>>(), j.at("sensors").get<std::vector<SensorTemplate>>(), j.at("alarms").get<std::vector<AlarmTemplate>>(), j.at("actuators").get<std::vector<ActuatorTemplate>>(), j.at("firmwareUpdateType").get<std::string>(),});
}
/*** SUBDEVICE REGISTRATION REQUEST DTO ***/

/*** SUBDEVICE REGISTRATION RESPONSE DTO ***/
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

        case DeviceRegistrationResponse::Result::ERROR_TEMPLATE_CONFLICT:
            return "ERROR_TEMPLATE_CONFLICT";
            break;

        case DeviceRegistrationResponse::Result::ERROR_MAXIMUM_NUMBER_OF_DEVICES_EXCEEDED:
            return "ERROR_MAXIMUM_NUMBER_OF_DEVICES_EXCEEDED";
            break;

        case DeviceRegistrationResponse::Result::ERROR_NO_GATEWAY_TEMPLATE:
            return "ERROR_NO_GATEWAY_TEMPLATE";
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
/*** SUBDEVICE REGISTRATION RESPONSE DTO ***/

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

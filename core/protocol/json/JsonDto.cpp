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

#include "protocol/json/JsonDto.h"

#include "model/ActuatorTemplate.h"
#include "model/AlarmTemplate.h"
#include "model/ConfigurationTemplate.h"
#include "model/DeviceStatusConfirm.h"
#include "model/GatewayUpdateRequest.h"
#include "model/GatewayUpdateResponse.h"
#include "model/SensorTemplate.h"
#include "model/SubdeviceDeletionResponse.h"
#include "model/SubdeviceRegistrationRequest.h"
#include "model/SubdeviceRegistrationResponse.h"
#include "model/SubdeviceUpdateRequest.h"
#include "model/SubdeviceUpdateResponse.h"

#include <string>

using nlohmann::json;

namespace wolkabout
{
static std::string createMultivalue(const std::string& value, size_t size)
{
    std::string multivalue = "";
    for (size_t i = 0; i < size; ++i)
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
    confJ["dataType"] = dataType;
    confJ["reference"] = configurationTemplate.getReference();
    confJ["defaultValue"] = createMultivalue(configurationTemplate.getDefaultValue(), configurationTemplate.getSize());
    confJ["size"] = configurationTemplate.getSize();
    confJ["description"] = configurationTemplate.getDescription();

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

    auto it_description = j.find("description");
    std::string description = (it_description != j.end()) ? j.at("description").get<std::string>() : "";

    auto it_defaultValue = j.find("defaultValue");
    std::string defaultValue = (it_defaultValue != j.end()) ? j.at("defaultValue").get<std::string>() : "";

    // clang-format off
    configurationTemplate =
            ConfigurationTemplate(
                j.at("name").get<std::string>(),
                j.at("reference").get<std::string>(),
                dataType,
                description,
                defaultValue,
                j.at("labels").is_null() ? std::vector<std::string>{} : j.at("labels").get<std::vector<std::string>>());
    // clang-format on
}
/*** CONFIGURATION TEMPLATE ***/

/*** ALARM TEMPLATE ***/
void to_json(json& j, const AlarmTemplate& alarmTemplate)
{
    // clang-format off
    j = {
        {"name", alarmTemplate.getName()},
        {"reference", alarmTemplate.getReference()},
        {"description", alarmTemplate.getDescription()},
    };
    // clang-format on
}

void from_json(const json& j, AlarmTemplate& alarmTemplate)
{
    auto it_description = j.find("description");
    std::string description = (it_description != j.end()) ? j.at("description").get<std::string>() : "";

    // clang-format off
    alarmTemplate =
            AlarmTemplate(j.at("name").get<std::string>(),
                          j.at("reference").get<std::string>(),
                          description);
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

    j = actuatorJ;
}

void from_json(const json& j, ActuatorTemplate& actuatorTemplate)
{
    auto it_description = j.find("description");
    std::string description = (it_description != j.end()) ? j.at("description").get<std::string>() : "";

    // clang-format off
    actuatorTemplate = ActuatorTemplate{
                j.at("name").get<std::string>(),
                j.at("reference").get<std::string>(),
                j["unit"].at("readingTypeName").get<std::string>(),
                j["unit"].at("symbol").is_null() ? "" : j["unit"].at("symbol").get<std::string>(),
                description};
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
    if (!sensorTemplate.getUnitSymbol().empty())
    {
        sensorJ["unit"]["symbol"] = sensorTemplate.getUnitSymbol();
    }
    else
    {
        sensorJ["unit"]["symbol"] = nullptr;
    }

    j = sensorJ;
}

void from_json(const json& j, SensorTemplate& sensorTemplate)
{
    auto it_description = j.find("description");
    std::string description = (it_description != j.end()) ? j.at("description").get<std::string>() : "";

    sensorTemplate =
      SensorTemplate{j.at("name").get<std::string>(), j.at("reference").get<std::string>(),
                     j["unit"].at("readingTypeName").get<std::string>(),
                     j["unit"].at("symbol").is_null() ? "" : j["unit"].at("symbol").get<std::string>(), description};
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
        {"typeParameters", deviceTemplate.getTypeParameters()},
        {"connectivityParameters", deviceTemplate.getConnectivityParameters()},
        {"firmwareUpdateParameters", deviceTemplate.getFirmwareUpdateParameters()}
    };
    // clang-format on
}

void from_json(const json& j, DeviceTemplate& deviceTemplate)
{
    deviceTemplate = DeviceTemplate(
      j.at("configurations").get<std::vector<ConfigurationTemplate>>(),
      j.at("sensors").get<std::vector<SensorTemplate>>(), j.at("alarms").get<std::vector<AlarmTemplate>>(),
      j.at("actuators").get<std::vector<ActuatorTemplate>>(), j.at("firmwareUpdateType").get<std::string>(),
      j.at("typeParameters").get<std::map<std::string, std::string>>(),
      j.at("connectivityParameters").get<std::map<std::string, std::string>>(),
      j.at("firmwareUpdateParameters").get<std::map<std::string, bool>>());
}
/*** DEVICE TEMPLATE ***/

/*** GATEWAY UPDATE REQUEST DTO ***/

void to_json(nlohmann::json& j, const GatewayUpdateRequest& dto)
{
    // clang-format off
    j = {
        {"sensors", dto.getTemplate().getSensors()},
        {"actuators", dto.getTemplate().getActuators()},
        {"alarms", dto.getTemplate().getAlarms()},
        {"configurations", dto.getTemplate().getConfigurations()}
    };
    // clang-format on
}

/*** GATEWAY UPDATE REQUEST DTO ***/

/*** GATEWAY UPDATE RESPONSE DTO ***/
void to_json(json& j, const GatewayUpdateResponse& dto)
{
    // clang-format off
    j = {
        {"result", dto.getResult().getMessage()},
        {"description", dto.getResult().getDescription()}
    };
    // clang-format on
}

GatewayUpdateResponse gateway_update_response_from_json(const json& j)
{
    auto result = platform_result_from_string(j.at("result").get<std::string>());

    if (!j.at("description").is_null())
    {
        result.setDescription(j.at("description").get<std::string>());
    }

    return GatewayUpdateResponse(result);
}
/*** GATEWAY UPDATE RESPONSE DTO ***/

/*** SUBDEVICE REGISTRATION REQUEST DTO ***/
void to_json(json& j, const SubdeviceRegistrationRequest& dto)
{
    // clang-format off
    j = {
        {"name", dto.getSubdeviceName()},
        {"deviceKey", dto.getSubdeviceKey()},
        {"sensors", dto.getTemplate().getSensors()},
        {"actuators", dto.getTemplate().getActuators()},
        {"alarms", dto.getTemplate().getAlarms()},
        {"configurations", dto.getTemplate().getConfigurations()},
        {"defaultBinding", dto.getDefaultBinding()},
        {"firmwareUpdateParameters", dto.getTemplate().getFirmwareUpdateParameters()}
    };

    if (!dto.getType().empty())
    {
        j["type"] = dto.getType();
    }
    // clang-format on
}

SubdeviceRegistrationRequest subdevice_registration_request_from_json(const json& j)
{
    DeviceTemplate subdeviceTemplate = DeviceTemplate(
      j.at("configurations").get<std::vector<ConfigurationTemplate>>(),
      j.at("sensors").get<std::vector<SensorTemplate>>(), j.at("alarms").get<std::vector<AlarmTemplate>>(),
      j.at("actuators").get<std::vector<ActuatorTemplate>>(), "" /*no firmwareUpdateType in request*/,
      {} /*no typeParameters in request*/, {} /*no connectivityParameters in request*/,
      j.at("firmwareUpdateParameters").get<std::map<std::string, bool>>());

    auto it_defaultBinding = j.find("defaultBinding");
    bool defaultBinding = (it_defaultBinding != j.end()) ? j.at("defaultBinding").get<bool>() : false;

    auto it_type = j.find("type");
    std::string type = (it_type != j.end()) ? j.at("type").get<std::string>() : "";

    return SubdeviceRegistrationRequest(j.at("name").get<std::string>(), j.at("deviceKey").get<std::string>(),
                                        subdeviceTemplate, defaultBinding, type);
}
/*** SUBDEVICE REGISTRATION REQUEST DTO ***/

/*** SUBDEVICE REGISTRATION RESPONSE DTO ***/
void to_json(json& j, const SubdeviceRegistrationResponse& dto)
{
    // clang-format off
    json j_payload = {{"deviceKey", dto.getSubdeviceKey()}};
    j = {
        {"payload" , j_payload},
        {"result", dto.getResult().getMessage()},
        {"description", dto.getResult().getDescription()}
    };
    // clang-format on
}

SubdeviceRegistrationResponse subdevice_registration_response_from_json(const nlohmann::json& j)
{
    auto result = platform_result_from_string(j.at("result").get<std::string>());

    if (!j.at("description").is_null())
    {
        result.setDescription(j.at("description").get<std::string>());
    }

    const auto key = j["payload"].at("deviceKey").get<std::string>();

    return SubdeviceRegistrationResponse(key, result);
}

/*** SUBDEVICE REGISTRATION RESPONSE DTO ***/

/*** SUBDEVICE UPDATE REQUEST DTO ***/
void to_json(json& j, const SubdeviceUpdateRequest& dto)
{
    // clang-format off
    j = {
        {"updateDefaultSemantics", dto.getUpdateDefaultSemantics()},
        {"add", {
            {"sensors", dto.getSensors()},
            {"actuators", dto.getActuators()},
            {"alarms", dto.getAlarms()},
            {"configurations", dto.getConfigurations()}}
        }
    };
    // clang-format on
}

SubdeviceUpdateRequest subdevice_update_request_from_json(const nlohmann::json& j, const std::string& deviceKey)
{
    auto configurations = j["add"].at("configurations").get<std::vector<ConfigurationTemplate>>();
    auto sensors = j["add"].at("sensors").get<std::vector<SensorTemplate>>();
    auto actuators = j["add"].at("actuators").get<std::vector<ActuatorTemplate>>();
    auto alarms = j["add"].at("alarms").get<std::vector<AlarmTemplate>>();

    bool updateDefaultSemantics = j.at("updateDefaultSemantics").get<bool>();

    return SubdeviceUpdateRequest(deviceKey, updateDefaultSemantics, configurations, sensors, alarms, actuators);
}
/*** SUBDEVICE UPDATE REQUEST DTO ***/

/*** SUBDEVICE UPDATE RESPONSE DTO ***/
void to_json(nlohmann::json& j, const SubdeviceUpdateResponse& dto)
{
    // clang-format off
    j = {
        {"result", dto.getResult().getMessage()},
        {"description", dto.getResult().getDescription()}
    };
    // clang-format on
}

SubdeviceUpdateResponse subdevice_update_response_from_json(const nlohmann::json& j, const std::string& deviceKey)
{
    auto result = platform_result_from_string(j.at("result").get<std::string>());

    if (!j.at("description").is_null())
    {
        result.setDescription(j.at("description").get<std::string>());
    }

    return SubdeviceUpdateResponse(deviceKey, result);
}
/*** SUBDEVICE UPDATE RESPONSE DTO ***/

/*** SUBDEVICE DELETION RESPONSE DTO ***/
void to_json(nlohmann::json& j, const SubdeviceDeletionResponse& dto)
{
    j = {{"result", dto.getResult().getMessage()}};
}

SubdeviceDeletionResponse subdevice_deletion_response_from_json(const nlohmann::json& j, const std::string& deviceKey)
{
    const auto result = platform_result_from_string(j.at("result").get<std::string>());

    return SubdeviceDeletionResponse(deviceKey, result);
}
/*** SUBDEVICE DELETION RESPONSE DTO ***/

PlatformResult platform_result_from_json(const nlohmann::json& j)
{
    auto result = platform_result_from_string(j.at("result").get<std::string>());

    auto it_description = j.find("description");
    if (it_description != j.end() && !j.at("description").is_null())
    {
        result.setDescription(j.at("description").get<std::string>());
    }

    return result;
}

PlatformResult platform_result_from_string(const std::string& str)
{
    const auto code = [&] {
        if (str == "OK")
        {
            return PlatformResult::Code::OK;
        }
        else if (str == "ERROR_GATEWAY_NOT_FOUND")
        {
            return PlatformResult::Code::ERROR_GATEWAY_NOT_FOUND;
        }
        else if (str == "ERROR_DEVICE_NOT_FOUND")
        {
            return PlatformResult::Code::ERROR_DEVICE_NOT_FOUND;
        }
        else if (str == "ERROR_SUBDEVICE_MANAGEMENT_FORBIDDEN")
        {
            return PlatformResult::Code::ERROR_SUBDEVICE_MANAGEMENT_FORBIDDEN;
        }
        else if (str == "ERROR_MAXIMUM_NUMBER_OF_DEVICES_EXCEEDED")
        {
            return PlatformResult::Code::ERROR_MAXIMUM_NUMBER_OF_DEVICES_EXCEEDED;
        }
        else if (str == "ERROR_NOT_A_GATEWAY")
        {
            return PlatformResult::Code::ERROR_NOT_A_GATEWAY;
        }
        else if (str == "ERROR_INVALID_DEVICE")
        {
            return PlatformResult::Code::ERROR_INVALID_DEVICE;
        }
        else if (str == "ERROR_KEY_CONFLICT")
        {
            return PlatformResult::Code::ERROR_KEY_CONFLICT;
        }
        else if (str == "ERROR_READING_PAYLOAD")
        {
            return PlatformResult::Code::ERROR_READING_PAYLOAD;
        }
        else if (str == "ERROR_VALIDATION")
        {
            return PlatformResult::Code::ERROR_VALIDATION;
        }
        else if (str == "VALIDATION_ERROR")
        {
            return PlatformResult::Code::VALIDATION_ERROR;
        }
        else if (str == "INVALID_SUBDEVICE_DTO")
        {
            return PlatformResult::Code::INVALID_SUBDEVICE_DTO;
        }
        else if (str == "PARSE_ERROR")
        {
            return PlatformResult::Code::PARSE_ERROR;
        }
        else if (str == "MISSING_UNIT")
        {
            return PlatformResult::Code::MISSING_UNIT;
        }
        else if (str == "ERROR_UNKNOWN")
        {
            return PlatformResult::Code::ERROR_UNKNOWN;
        }
        else
        {
            return PlatformResult::Code::NOT_SUPPORTED;
        }
    }();

    return {code, str};
}

}    // namespace wolkabout

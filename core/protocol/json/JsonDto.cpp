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
void to_json(json& destinationJ, const ConfigurationTemplate& configurationTemplate)
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

    destinationJ = confJ;
}

void from_json(const json& sourceJ, ConfigurationTemplate& configurationTemplate)
{
    const auto dataType = [&]() -> DataType {
        std::string dataTypeStr = sourceJ.at("dataType").get<std::string>();
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

    auto it_description = sourceJ.find("description");
    std::string description = (it_description != sourceJ.end()) ? sourceJ.at("description").get<std::string>() : "";

    auto it_defaultValue = sourceJ.find("defaultValue");
    std::string defaultValue = (it_defaultValue != sourceJ.end()) ? sourceJ.at("defaultValue").get<std::string>() : "";

    // clang-format off
    configurationTemplate =
            ConfigurationTemplate(
                sourceJ.at("name").get<std::string>(),
                sourceJ.at("reference").get<std::string>(),
                dataType,
                description,
                defaultValue,
                sourceJ.at("labels").is_null() ? std::vector<std::string>{} : sourceJ.at("labels").get<std::vector<std::string>>());
    // clang-format on
}
/*** CONFIGURATION TEMPLATE ***/

/*** ALARM TEMPLATE ***/
void to_json(json& destinationJ, const AlarmTemplate& alarmTemplate)
{
    // clang-format off
    destinationJ = {
        {"name", alarmTemplate.getName()},
        {"reference", alarmTemplate.getReference()},
        {"description", alarmTemplate.getDescription()},
    };
    // clang-format on
}

void from_json(const json& sourceJ, AlarmTemplate& alarmTemplate)
{
    auto it_description = sourceJ.find("description");
    std::string description = (it_description != sourceJ.end()) ? sourceJ.at("description").get<std::string>() : "";

    // clang-format off
    alarmTemplate =
            AlarmTemplate(sourceJ.at("name").get<std::string>(),
                          sourceJ.at("reference").get<std::string>(),
                          description);
    // clang-format on
}
/*** ALARM TEMPLATE ***/

/*** ACTUATOR TEMPLATE ***/
void to_json(json& destinationJ, const ActuatorTemplate& actuatorTemplate)
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

    destinationJ = actuatorJ;
}

void from_json(const json& sourceJ, ActuatorTemplate& actuatorTemplate)
{
    auto it_description = sourceJ.find("description");
    std::string description = (it_description != sourceJ.end()) ? sourceJ.at("description").get<std::string>() : "";

    // clang-format off
    actuatorTemplate = ActuatorTemplate{
                sourceJ.at("name").get<std::string>(),
                sourceJ.at("reference").get<std::string>(),
                sourceJ["unit"].at("readingTypeName").get<std::string>(),
                sourceJ["unit"].at("symbol").is_null() ? "" : sourceJ["unit"].at("symbol").get<std::string>(),
                description};
    // clang-format on
}
/*** ACTUATOR TEMPLATE ***/

/*** SENSOR TEMPLATE ***/
void to_json(json& destinationJ, const SensorTemplate& sensorTemplate)
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

    destinationJ = sensorJ;
}

void from_json(const json& sourceJ, SensorTemplate& sensorTemplate)
{
    auto it_description = sourceJ.find("description");
    std::string description = (it_description != sourceJ.end()) ? sourceJ.at("description").get<std::string>() : "";

    sensorTemplate = SensorTemplate{
      sourceJ.at("name").get<std::string>(), sourceJ.at("reference").get<std::string>(),
      sourceJ["unit"].at("readingTypeName").get<std::string>(),
      sourceJ["unit"].at("symbol").is_null() ? "" : sourceJ["unit"].at("symbol").get<std::string>(), description};
}
/*** SENSOR TEMPLATE ***/

/*** DEVICE TEMPLATE ***/
void to_json(json& destinationJ, const DeviceTemplate& deviceTemplate)
{
    // clang-format off
    destinationJ = {
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

void from_json(const json& sourceJ, DeviceTemplate& deviceTemplate)
{
    deviceTemplate = DeviceTemplate(
      sourceJ.at("configurations").get<std::vector<ConfigurationTemplate>>(),
      sourceJ.at("sensors").get<std::vector<SensorTemplate>>(), sourceJ.at("alarms").get<std::vector<AlarmTemplate>>(),
      sourceJ.at("actuators").get<std::vector<ActuatorTemplate>>(), sourceJ.at("firmwareUpdateType").get<std::string>(),
      sourceJ.at("typeParameters").get<std::map<std::string, std::string>>(),
      sourceJ.at("connectivityParameters").get<std::map<std::string, std::string>>(),
      sourceJ.at("firmwareUpdateParameters").get<std::map<std::string, bool>>());
}
/*** DEVICE TEMPLATE ***/

/*** GATEWAY UPDATE REQUEST DTO ***/

void to_json(json& destinationJ, const GatewayUpdateRequest& dto)
{
    // clang-format off
    destinationJ = {
        {"sensors", dto.getTemplate().getSensors()},
        {"actuators", dto.getTemplate().getActuators()},
        {"alarms", dto.getTemplate().getAlarms()},
        {"configurations", dto.getTemplate().getConfigurations()}
    };
    // clang-format on
}

/*** GATEWAY UPDATE REQUEST DTO ***/

/*** GATEWAY UPDATE RESPONSE DTO ***/
void to_json(json& destinationJ, const GatewayUpdateResponse& dto)
{
    // clang-format off
    destinationJ = {
        {"result", dto.getResult().getMessage()},
        {"description", dto.getResult().getDescription()}
    };
    // clang-format on
}

GatewayUpdateResponse gateway_update_response_from_json(const json& sourceJ)
{
    auto result = platform_result_from_string(sourceJ.at("result").get<std::string>());

    if (!sourceJ.at("description").is_null())
    {
        result.setDescription(sourceJ.at("description").get<std::string>());
    }

    return GatewayUpdateResponse(result);
}
/*** GATEWAY UPDATE RESPONSE DTO ***/

/*** SUBDEVICE REGISTRATION REQUEST DTO ***/
void to_json(json& destinationJ, const SubdeviceRegistrationRequest& dto)
{
    // clang-format off
    destinationJ = {
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
        destinationJ["type"] = dto.getType();
    }
    // clang-format on
}

SubdeviceRegistrationRequest subdevice_registration_request_from_json(const json& sourceJ)
{
    DeviceTemplate subdeviceTemplate = DeviceTemplate(
      sourceJ.at("configurations").get<std::vector<ConfigurationTemplate>>(),
      sourceJ.at("sensors").get<std::vector<SensorTemplate>>(), sourceJ.at("alarms").get<std::vector<AlarmTemplate>>(),
      sourceJ.at("actuators").get<std::vector<ActuatorTemplate>>(), "" /*no firmwareUpdateType in request*/,
      {} /*no typeParameters in request*/, {} /*no connectivityParameters in request*/,
      sourceJ.at("firmwareUpdateParameters").get<std::map<std::string, bool>>());

    auto it_defaultBinding = sourceJ.find("defaultBinding");
    bool defaultBinding = (it_defaultBinding != sourceJ.end()) ? sourceJ.at("defaultBinding").get<bool>() : false;

    auto it_type = sourceJ.find("type");
    std::string type = (it_type != sourceJ.end()) ? sourceJ.at("type").get<std::string>() : "";

    return SubdeviceRegistrationRequest(sourceJ.at("name").get<std::string>(),
                                        sourceJ.at("deviceKey").get<std::string>(), subdeviceTemplate, defaultBinding,
                                        type);
}
/*** SUBDEVICE REGISTRATION REQUEST DTO ***/

/*** SUBDEVICE REGISTRATION RESPONSE DTO ***/
void to_json(json& destinationJ, const SubdeviceRegistrationResponse& dto)
{
    // clang-format off
    json j_payload = {{"deviceKey", dto.getSubdeviceKey()}};
    destinationJ = {
        {"payload" , j_payload},
        {"result", dto.getResult().getMessage()},
        {"description", dto.getResult().getDescription()}
    };
    // clang-format on
}

SubdeviceRegistrationResponse subdevice_registration_response_from_json(const json& sourceJ)
{
    auto result = platform_result_from_string(sourceJ.at("result").get<std::string>());

    if (!sourceJ.at("description").is_null())
    {
        result.setDescription(sourceJ.at("description").get<std::string>());
    }

    const auto key = sourceJ["payload"].at("deviceKey").get<std::string>();

    return SubdeviceRegistrationResponse(key, result);
}

/*** SUBDEVICE REGISTRATION RESPONSE DTO ***/

/*** SUBDEVICE UPDATE REQUEST DTO ***/
void to_json(json& destinationJ, const SubdeviceUpdateRequest& dto)
{
    // clang-format off
    destinationJ = {
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

SubdeviceUpdateRequest subdevice_update_request_from_json(const json& sourceJ, const std::string& deviceKey)
{
    auto configurations = sourceJ["add"].at("configurations").get<std::vector<ConfigurationTemplate>>();
    auto sensors = sourceJ["add"].at("sensors").get<std::vector<SensorTemplate>>();
    auto actuators = sourceJ["add"].at("actuators").get<std::vector<ActuatorTemplate>>();
    auto alarms = sourceJ["add"].at("alarms").get<std::vector<AlarmTemplate>>();

    bool updateDefaultSemantics = sourceJ.at("updateDefaultSemantics").get<bool>();

    return SubdeviceUpdateRequest(deviceKey, updateDefaultSemantics, configurations, sensors, alarms, actuators);
}
/*** SUBDEVICE UPDATE REQUEST DTO ***/

/*** SUBDEVICE UPDATE RESPONSE DTO ***/
void to_json(json& destinationJ, const SubdeviceUpdateResponse& dto)
{
    // clang-format off
    destinationJ = {
        {"result", dto.getResult().getMessage()},
        {"description", dto.getResult().getDescription()}
    };
    // clang-format on
}

SubdeviceUpdateResponse subdevice_update_response_from_json(const json& sourceJ, const std::string& deviceKey)
{
    auto result = platform_result_from_string(sourceJ.at("result").get<std::string>());

    if (!sourceJ.at("description").is_null())
    {
        result.setDescription(sourceJ.at("description").get<std::string>());
    }

    return SubdeviceUpdateResponse(deviceKey, result);
}
/*** SUBDEVICE UPDATE RESPONSE DTO ***/

/*** SUBDEVICE DELETION RESPONSE DTO ***/
void to_json(json& destinationJ, const SubdeviceDeletionResponse& dto)
{
    destinationJ = {{"result", dto.getResult().getMessage()}};
}

SubdeviceDeletionResponse subdevice_deletion_response_from_json(const json& sourceJ, const std::string& deviceKey)
{
    const auto result = platform_result_from_string(sourceJ.at("result").get<std::string>());

    return SubdeviceDeletionResponse(deviceKey, result);
}
/*** SUBDEVICE DELETION RESPONSE DTO ***/

PlatformResult platform_result_from_json(const json& sourceJ)
{
    auto result = platform_result_from_string(sourceJ.at("result").get<std::string>());

    auto it_description = sourceJ.find("description");
    if (it_description != sourceJ.end() && !sourceJ.at("description").is_null())
    {
        result.setDescription(sourceJ.at("description").get<std::string>());
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

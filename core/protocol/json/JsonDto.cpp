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
#include "model/GatewayUpdateRequest.h"
#include "model/GatewayUpdateResponse.h"
#include "model/SensorTemplate.h"
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

    if (configurationTemplate.getMinimum())
    {
        confJ["minimum"] = configurationTemplate.getMinimum().value();
    }

    if (configurationTemplate.getMaximum())
    {
        confJ["maximum"] = configurationTemplate.getMaximum().value();
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

    auto it_minimum = j.find("minimum");
    WolkOptional<double> minimum;
    if (it_minimum != j.end() && !j["minimum"].is_null())
    {
        minimum = j["minimum"].get<double>();
    }

    auto it_maximum = j.find("maximum");
    WolkOptional<double> maximum;
    if (it_maximum != j.end() && !j["maximum"].is_null())
    {
        maximum = j["maximum"].get<double>();
    }

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
                j.at("labels").is_null() ? std::vector<std::string>{} : j.at("labels").get<std::vector<std::string>>(),
                minimum,
                maximum);
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

    if (actuatorTemplate.getMinimum())
    {
        actuatorJ["minimum"] = actuatorTemplate.getMinimum().value();
    }
    else
    {
        actuatorJ["minimum"] = nullptr;
    }

    if (actuatorTemplate.getMaximum())
    {
        actuatorJ["maximum"] = actuatorTemplate.getMaximum().value();
    }
    else
    {
        actuatorJ["maximum"] = nullptr;
    }

    j = actuatorJ;
}

void from_json(const json& j, ActuatorTemplate& actuatorTemplate)
{
    auto it_minimum = j.find("minimum");
    WolkOptional<double> minimum;
    if (it_minimum != j.end() && !j["minimum"].is_null())
    {
        minimum = j["minimum"].get<double>();
    }

    auto it_maximum = j.find("maximum");
    WolkOptional<double> maximum;
    if (it_maximum != j.end() && !j["maximum"].is_null())
    {
        maximum = j["maximum"].get<double>();
    }

    auto it_description = j.find("description");
    std::string description = (it_description != j.end()) ? j.at("description").get<std::string>() : "";

    // clang-format off
    actuatorTemplate = ActuatorTemplate{
                j.at("name").get<std::string>(),
                j.at("reference").get<std::string>(),
                j["unit"].at("readingTypeName").get<std::string>(),
                j["unit"].at("symbol").is_null() ? "" : j["unit"].at("symbol").get<std::string>(),
                description,
                minimum,
                maximum};
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

    if (sensorTemplate.getMinimum())
    {
        sensorJ["minimum"] = sensorTemplate.getMinimum().value();
    }
    else
    {
        sensorJ["minimum"] = nullptr;
    }

    if (sensorTemplate.getMaximum())
    {
        sensorJ["maximum"] = sensorTemplate.getMaximum().value();
    }
    else
    {
        sensorJ["maximum"] = nullptr;
    }

    j = sensorJ;
}

void from_json(const json& j, SensorTemplate& sensorTemplate)
{
    auto it_minimum = j.find("minimum");
    WolkOptional<double> minimum;
    if (it_minimum != j.end() && !j["minimum"].is_null())
    {
        minimum = j["minimum"].get<double>();
    }

    auto it_maximum = j.find("maximum");
    WolkOptional<double> maximum;
    if (it_maximum != j.end() && !j["maximum"].is_null())
    {
        maximum = j["maximum"].get<double>();
    }

    auto it_description = j.find("description");
    std::string description = (it_description != j.end()) ? j.at("description").get<std::string>() : "";

    sensorTemplate = SensorTemplate{j.at("name").get<std::string>(),
                                    j.at("reference").get<std::string>(),
                                    j["unit"].at("readingTypeName").get<std::string>(),
                                    j["unit"].at("symbol").is_null() ? "" : j["unit"].at("symbol").get<std::string>(),
                                    description,
                                    minimum,
                                    maximum};
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

/*** SUBDEVICE REGISTRATION REQUEST DTO ***/
void to_json(json& j, const SubdeviceRegistrationRequest& dto)
{
    // clang-format off
    j = {
        {"name", dto.getSubdeviceName()},
        {"deviceKey", dto.getSubdeviceKey()},
        {"defaultBinding", dto.getDefaultBinding()},
        {"sensors", dto.getTemplate().getSensors()},
        {"actuators", dto.getTemplate().getActuators()},
        {"alarms", dto.getTemplate().getAlarms()},
        {"configurations", dto.getTemplate().getConfigurations()},
        {"typeParameters", dto.getTemplate().getTypeParameters()},
        {"connectivityParameters", dto.getTemplate().getConnectivityParameters()},
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
      j.at("typeParameters").get<std::map<std::string, std::string>>(),
      j.at("connectivityParameters").get<std::map<std::string, std::string>>(),
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
    auto resultStr = [&]() -> std::string {
        switch (dto.getResult())
        {
        case SubdeviceRegistrationResponse::Result::OK:
            return "OK";
        case SubdeviceRegistrationResponse::Result::GATEWAY_NOT_FOUND:
            return "ERROR_GATEWAY_NOT_FOUND";
        case SubdeviceRegistrationResponse::Result::VALIDATION_ERROR:
            return "VALIDATION_ERROR";
        case SubdeviceRegistrationResponse::Result::INVALID_SUBDEVICE_DTO:
            return "ERROR_INVALID_SUBDEVICE_DTO";
        case SubdeviceRegistrationResponse::Result::SUBDEVICE_MANAGEMENT_FORBIDDEN:
            return "ERROR_SUBDEVICE_MANAGEMENT_FORBIDDEN";
        case SubdeviceRegistrationResponse::Result::MAXIMUM_NUMBER_OF_DEVICES_EXCEEDED:
            return "ERROR_MAXIMUM_NUMBER_OF_DEVICES_EXCEEDED";
        case SubdeviceRegistrationResponse::Result::NOT_A_GATEWAY:
            return "ERROR_NOT_A_GATEWAY";
        case SubdeviceRegistrationResponse::Result::KEY_CONFLICT:
            return "ERROR_KEY_CONFLICT";
        case SubdeviceRegistrationResponse::Result::MISSING_UNIT:
            return "MISSING_UNIT";
        case SubdeviceRegistrationResponse::Result::ERROR_UNKNOWN:
            return "ERROR_UNKNOWN";
        default:
        {
            assert(false);
            throw std::invalid_argument("Unhandled result");
        }
        }
    }();

    // clang-format off
    json j_payload = {{"deviceKey", dto.getSubdeviceKey()}};
    j = {
        {"payload" , j_payload},
        {"result", resultStr},
        {"description", dto.getDescription()}
    };
    // clang-format on
}

SubdeviceRegistrationResponse subdevice_registration_response_from_json(const nlohmann::json& j)
{
    auto result = [&]() -> SubdeviceRegistrationResponse::Result {
        std::string resultStr = j.at("result").get<std::string>();
        if (resultStr == "OK")
        {
            return SubdeviceRegistrationResponse::Result::OK;
        }
        else if (resultStr == "ERROR_GATEWAY_NOT_FOUND")
        {
            return SubdeviceRegistrationResponse::Result::GATEWAY_NOT_FOUND;
        }
        else if (resultStr == "VALIDATION_ERROR")
        {
            return SubdeviceRegistrationResponse::Result::VALIDATION_ERROR;
        }
        else if (resultStr == "ERROR_INVALID_SUBDEVICE_DTO")
        {
            return SubdeviceRegistrationResponse::Result::INVALID_SUBDEVICE_DTO;
        }
        else if (resultStr == "ERROR_SUBDEVICE_MANAGEMENT_FORBIDDEN")
        {
            return SubdeviceRegistrationResponse::Result::SUBDEVICE_MANAGEMENT_FORBIDDEN;
        }
        else if (resultStr == "ERROR_MAXIMUM_NUMBER_OF_DEVICES_EXCEEDED")
        {
            return SubdeviceRegistrationResponse::Result::MAXIMUM_NUMBER_OF_DEVICES_EXCEEDED;
        }
        else if (resultStr == "ERROR_NOT_A_GATEWAY")
        {
            return SubdeviceRegistrationResponse::Result::NOT_A_GATEWAY;
        }
        else if (resultStr == "ERROR_KEY_CONFLICT")
        {
            return SubdeviceRegistrationResponse::Result::KEY_CONFLICT;
        }
        else if (resultStr == "MISSING_UNIT")
        {
            return SubdeviceRegistrationResponse::Result::MISSING_UNIT;
        }
        else
        {
            return SubdeviceRegistrationResponse::Result::ERROR_UNKNOWN;
        }
    }();

    std::string description = (j.at("description").is_null()) ? "" : j.at("description").get<std::string>();

    return SubdeviceRegistrationResponse(j["payload"].at("deviceKey").get<std::string>(), result, description);
}

/*** SUBDEVICE REGISTRATION RESPONSE DTO ***/

/*** GATEWAY UPDATE REQUEST DTO ***/

void to_json(nlohmann::json& j, const GatewayUpdateRequest& dto)
{
    // clang-format off
    j = {
        {"firmwareUpdateType", dto.getTemplate().getFirmwareUpdateType()},
        {"sensors", dto.getTemplate().getSensors()},
        {"actuators", dto.getTemplate().getActuators()},
        {"alarms", dto.getTemplate().getAlarms()},
        {"configurations", dto.getTemplate().getConfigurations()},
        {"typeParameters", dto.getTemplate().getTypeParameters()},
        {"connectivityParameters", dto.getTemplate().getConnectivityParameters()},
        {"firmwareUpdateParameters", dto.getTemplate().getFirmwareUpdateParameters()}
    };
    // clang-format on
}

/*** GATEWAY UPDATE REQUEST DTO ***/

/*** GATEWAY UPDATE RESPONSE DTO ***/
GatewayUpdateResponse gateway_update_response_from_json(const json& j)
{
    auto result = [&]() -> GatewayUpdateResponse::Result {
        std::string resultStr = j.at("result").get<std::string>();
        if (resultStr == "OK")
        {
            return GatewayUpdateResponse::Result::OK;
        }
        else if (resultStr == "ERROR_GATEWAY_NOT_FOUND")
        {
            return GatewayUpdateResponse::Result::GATEWAY_NOT_FOUND;
        }
        else if (resultStr == "VALIDATION_ERROR")
        {
            return GatewayUpdateResponse::Result::VALIDATION_ERROR;
        }
        else if (resultStr == "ERROR_GATEWAY_UPDATE_FORBIDDEN")
        {
            return GatewayUpdateResponse::Result::GATEWAY_UPDATE_FORBIDDEN;
        }
        else if (resultStr == "ERROR_SUBDEVICE_MANAGEMENT_FORBIDDEN")
        {
            return GatewayUpdateResponse::Result::SUBDEVICE_MANAGEMENT_FORBIDDEN;
        }
        else if (resultStr == "MISSING_UNIT")
        {
            return GatewayUpdateResponse::Result::MISSING_UNIT;
        }
        else
        {
            return GatewayUpdateResponse::Result::ERROR_UNKNOWN;
        }
    }();

    std::string description = (j.at("description").is_null() ? "" : j.at("description").get<std::string>());

    return GatewayUpdateResponse(result, description);
}
/*** GATEWAY UPDATE RESPONSE DTO ***/

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
    DeviceTemplate subdeviceTemplate = DeviceTemplate(
      j.at("configurations").get<std::vector<ConfigurationTemplate>>(),
      j.at("sensors").get<std::vector<SensorTemplate>>(), j.at("alarms").get<std::vector<AlarmTemplate>>(),
      j.at("actuators").get<std::vector<ActuatorTemplate>>(), "" /*no firmwareUpdateType in request*/,
      j.at("typeParameters").get<std::map<std::string, std::string>>(),
      j.at("connectivityParameters").get<std::map<std::string, std::string>>(),
      j.at("firmwareUpdateParameters").get<std::map<std::string, bool>>());

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
    auto resultStr = [&]() -> std::string {
        switch (dto.getResult())
        {
        case SubdeviceUpdateResponse::Result::OK:
            return "OK";
        case SubdeviceUpdateResponse::Result::GATEWAY_NOT_FOUND:
            return "ERROR_GATEWAY_NOT_FOUND";
        case SubdeviceUpdateResponse::Result::VALIDATION_ERROR:
            return "VALIDATION_ERROR";
        case SubdeviceUpdateResponse::Result::SUBDEVICE_MANAGEMENT_FORBIDDEN:
            return "ERROR_SUBDEVICE_MANAGEMENT_FORBIDDEN";
        case SubdeviceUpdateResponse::Result::NOT_A_GATEWAY:
            return "ERROR_NOT_A_GATEWAY";
        case SubdeviceUpdateResponse::Result::MISSING_UNIT:
            return "MISSING_UNIT";
        case SubdeviceUpdateResponse::Result::ERROR_UNKNOWN:
            return "ERROR_UNKNOWN";
        default:
        {
            assert(false);
            throw std::invalid_argument("Unhandled result");
        }
        }
    }();

    // clang-format off
    j = {
        {"result", resultStr},
        {"description", dto.getDescription()}
    };
    // clang-format on
}

SubdeviceUpdateResponse subdevice_update_response_from_json(const nlohmann::json& j, const std::string& deviceKey)
{
    auto result = [&]() -> SubdeviceUpdateResponse::Result {
        std::string resultStr = j.at("result").get<std::string>();
        if (resultStr == "OK")
        {
            return SubdeviceUpdateResponse::Result::OK;
        }
        else if (resultStr == "ERROR_GATEWAY_NOT_FOUND")
        {
            return SubdeviceUpdateResponse::Result::GATEWAY_NOT_FOUND;
        }
        else if (resultStr == "VALIDATION_ERROR")
        {
            return SubdeviceUpdateResponse::Result::VALIDATION_ERROR;
        }
        else if (resultStr == "ERROR_SUBDEVICE_MANAGEMENT_FORBIDDEN")
        {
            return SubdeviceUpdateResponse::Result::SUBDEVICE_MANAGEMENT_FORBIDDEN;
        }
        else if (resultStr == "ERROR_NOT_A_GATEWAY")
        {
            return SubdeviceUpdateResponse::Result::NOT_A_GATEWAY;
        }
        else if (resultStr == "MISSING_UNIT")
        {
            return SubdeviceUpdateResponse::Result::MISSING_UNIT;
        }
        else
        {
            return SubdeviceUpdateResponse::Result::ERROR_UNKNOWN;
        }
    }();

    std::string description = (j.at("description").is_null()) ? "" : j.at("description").get<std::string>();

    return SubdeviceUpdateResponse(deviceKey, result, description);
}

/*** SUBDEVICE UPDATE RESPONSE DTO ***/

PlatformResult platform_result_from_json(const nlohmann::json& j)
{
    auto resultStr = j.at("result").get<std::string>();

    if (resultStr == "OK")
        return PlatformResult::OK;
    else if (resultStr == "ERROR_GATEWAY_NOT_FOUND")
        return PlatformResult::ERROR_GATEWAY_NOT_FOUND;
    else if (resultStr == "ERROR_KEY_MISSING")
        return PlatformResult::ERROR_KEY_MISSING;
    else if (resultStr == "ERROR_NOT_A_GATEWAY")
        return PlatformResult::ERROR_NOT_A_GATEWAY;
    else if (resultStr == "ERROR_DEVICE_NOT_FOUND")
        return PlatformResult::ERROR_DEVICE_NOT_FOUND;
    else if (resultStr == "ERROR_INVALID_DEVICE")
        return PlatformResult::ERROR_INVALID_DEVICE;

    auto errorMsg = "Invalid value for platform result: " + resultStr;
    throw std::logic_error(errorMsg);
}

}    // namespace wolkabout

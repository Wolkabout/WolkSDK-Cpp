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
#include "model/GatewayUpdateRequest.h"
#include "model/GatewayUpdateResponse.h"
#include "model/SensorTemplate.h"
#include "model/SubdeviceRegistrationRequest.h"
#include "model/SubdeviceRegistrationResponse.h"

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
    auto it_minimum = j.find("minimum");
    double minimum = (it_minimum != j.end()) ? ((j["minimum"].is_null()) ? NULL : j["minimum"].get<double>()) : NULL;

    auto it_maximum = j.find("maximum");
    double maximum = (it_maximum != j.end()) ? ((j["maximum"].is_null()) ? NULL : j["maximum"].get<double>()) : NULL;

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

    auto it_minimum = j.find("minimum");
    double minimum = (it_minimum != j.end()) ? ((j["minimum"].is_null()) ? NULL : j["minimum"].get<double>()) : NULL;

    auto it_maximum = j.find("maximum");
    double maximum = (it_maximum != j.end()) ? ((j["maximum"].is_null()) ? NULL : j["maximum"].get<double>()) : NULL;

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
        {"configurations", dto.getTemplate().getConfigurations()},
        {"sensors", dto.getTemplate().getSensors()},
        {"alarms", dto.getTemplate().getAlarms()},
        {"actuators", dto.getTemplate().getActuators()},
        {"firmwareUpdateType", dto.getTemplate().getFirmwareUpdateType()},
        {"typeParameters", dto.getTemplate().getTypeParameters()},
        {"connectivityParameters", dto.getTemplate().getConnectivityParameters()},
        {"firmwareUpdateParameters", dto.getTemplate().getFirmwareUpdateParameters()}
    };
    // clang-format on
}

SubdeviceRegistrationRequest subdevice_registration_request_from_json(const json& j)
{
    DeviceTemplate subdeviceTemplate = DeviceTemplate(
      j.at("configurations").get<std::vector<ConfigurationTemplate>>(),
      j.at("sensors").get<std::vector<SensorTemplate>>(), j.at("alarms").get<std::vector<AlarmTemplate>>(),
      j.at("actuators").get<std::vector<ActuatorTemplate>>(), j.at("firmwareUpdateType").get<std::string>(),
      j.at("typeParameters").get<std::map<std::string, std::string>>(),
      j.at("connectivityParameters").get<std::map<std::string, std::string>>(),
      j.at("firmwareUpdateParameters").get<std::map<std::string, bool>>());

    auto it_defaultBinding = j.find("defaultBinding");

    bool defaultBinding = (it_defaultBinding != j.end()) ? j.at("defaultBinding").get<bool>() : false;
    return SubdeviceRegistrationRequest(j.at("name").get<std::string>(), j.at("deviceKey").get<std::string>(),
                                        subdeviceTemplate, defaultBinding);
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
            break;

        case SubdeviceRegistrationResponse::Result::ERROR_GATEWAY_NOT_FOUND:
            return "ERROR_GATEWAY_NOT_FOUND";
            break;

        case SubdeviceRegistrationResponse::Result::ERROR_KEY_CONFLICT:
            return "ERROR_KEY_CONFLICT";
            break;

        case SubdeviceRegistrationResponse::Result::ERROR_NOT_A_GATEWAY:
            return "ERROR_NOT_A_GATEWAY";
            break;

        case SubdeviceRegistrationResponse::Result::ERROR_MAXIMUM_NUMBER_OF_DEVICES_EXCEEDED:
            return "ERROR_MAXIMUM_NUMBER_OF_DEVICES_EXCEEDED";
            break;

        case SubdeviceRegistrationResponse::Result::ERROR_VALIDATION_ERROR:
            return "ERROR_VALIDATION_ERROR";
            break;

        case SubdeviceRegistrationResponse::Result::ERROR_INVALID_DTO:
            return "ERROR_INVALID_DTO";
            break;

        case SubdeviceRegistrationResponse::Result::ERROR_KEY_MISSING:
            return "ERROR_KEY_MISSING";
            break;

        case SubdeviceRegistrationResponse::Result::ERROR_SUBDEVICE_MANAGEMENT_FORBIDDEN:
            return "ERROR_SUBDEVICE_MANAGEMENT_FORBIDDEN";
            break;

        case SubdeviceRegistrationResponse::Result::ERROR_UNKNOWN:
            return "ERROR_UNKNOWN";
            break;

        default:
            assert(false);
            throw std::invalid_argument("Unhandled result");
        }
    }();

    // clang-format off
    j = {
        {"payload", {"deviceKey", dto.getSubdeviceKey()}},
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
            return SubdeviceRegistrationResponse::Result::ERROR_GATEWAY_NOT_FOUND;
        }
        else if (resultStr == "ERROR_NOT_A_GATEWAY")
        {
            return SubdeviceRegistrationResponse::Result::ERROR_NOT_A_GATEWAY;
        }
        else if (resultStr == "ERROR_KEY_CONFLICT")
        {
            return SubdeviceRegistrationResponse::Result::ERROR_KEY_CONFLICT;
        }
        else if (resultStr == "ERROR_MAXIMUM_NUMBER_OF_DEVICES_EXCEEDED")
        {
            return SubdeviceRegistrationResponse::Result::ERROR_MAXIMUM_NUMBER_OF_DEVICES_EXCEEDED;
        }
        else if (resultStr == "ERROR_VALIDATION_ERROR")
        {
            return SubdeviceRegistrationResponse::Result::ERROR_VALIDATION_ERROR;
        }
        else if (resultStr == "ERROR_INVALID_DTO")
        {
            return SubdeviceRegistrationResponse::Result::ERROR_INVALID_DTO;
        }
        else if (resultStr == "ERROR_KEY_MISSING")
        {
            return SubdeviceRegistrationResponse::Result::ERROR_KEY_MISSING;
        }
        else if (resultStr == "ERROR_SUBDEVICE_MANAGEMENT_FORBIDDEN")
        {
            return SubdeviceRegistrationResponse::Result::ERROR_SUBDEVICE_MANAGEMENT_FORBIDDEN;
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

void to_json(nlohmann::json& j, const GatewayUpdateResponse& dto)
{
    auto resultStr = [&]() -> std::string {
        switch (dto.getResult())
        {
        case GatewayUpdateResponse::Result::OK:
            return "OK";
            break;

        case GatewayUpdateResponse::Result::ERROR_GATEWAY_NOT_FOUND:
            return "ERROR_GATEWAY_NOT_FOUND";
            break;

        case GatewayUpdateResponse::Result::ERROR_KEY_CONFLICT:
            return "ERROR_KEY_CONFLICT";
            break;

        case GatewayUpdateResponse::Result::ERROR_NOT_A_GATEWAY:
            return "ERROR_NOT_A_GATEWAY";
            break;

        case GatewayUpdateResponse::Result::ERROR_VALIDATION_ERROR:
            return "ERROR_VALIDATION_ERROR";
            break;

        case GatewayUpdateResponse::Result::ERROR_INVALID_DTO:
            return "ERROR_INVALID_DTO";
            break;

        case GatewayUpdateResponse::Result::ERROR_KEY_MISSING:
            return "ERROR_KEY_MISSING";
            break;

        case GatewayUpdateResponse::Result::ERROR_UNKNOWN:
            return "ERROR_UNKNOWN";
            break;

        default:
            assert(false);
            throw std::invalid_argument("Unhandled result");
        }
    }();

    // clang-format off
    j = {
        {"result", resultStr},
        {"description", dto.getDescription()}
    };
}

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
                return GatewayUpdateResponse::Result::ERROR_GATEWAY_NOT_FOUND;
            }
            else if (resultStr == "ERROR_NOT_A_GATEWAY")
            {
                return GatewayUpdateResponse::Result::ERROR_NOT_A_GATEWAY;
            }
            else if (resultStr == "ERROR_KEY_CONFLICT")
            {
                return GatewayUpdateResponse::Result::ERROR_KEY_CONFLICT;
            }
            else if (resultStr == "ERROR_VALIDATION_ERROR")
            {
                return GatewayUpdateResponse::Result::ERROR_VALIDATION_ERROR;
            }
            else if (resultStr == "ERROR_INVALID_DTO")
            {
                return GatewayUpdateResponse::Result::ERROR_INVALID_DTO;
            }
            else if (resultStr == "ERROR_KEY_MISSING")
            {
                return GatewayUpdateResponse::Result::ERROR_KEY_MISSING;
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
}    // namespace wolkabout

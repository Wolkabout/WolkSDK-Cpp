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

#ifndef JSONDTO_H
#define JSONDTO_H

#include "model/PlatformResult.h"
#include "utilities/json.hpp"

namespace wolkabout
{
class ActuatorTemplate;
class AlarmTemplate;
class ConfigurationTemplate;
class SensorTemplate;
class DeviceTemplate;
class GatewayUpdateRequest;
class GatewayUpdateResponse;
class SubdeviceRegistrationRequest;
class SubdeviceRegistrationResponse;
class SubdeviceUpdateRequest;
class SubdeviceUpdateResponse;

void to_json(nlohmann::json& j, const ConfigurationTemplate& configurationTemplate);
void from_json(const nlohmann::json& j, ConfigurationTemplate& configurationTemplate);

void to_json(nlohmann::json& j, const AlarmTemplate& alarmTemplate);
void from_json(const nlohmann::json& j, AlarmTemplate& alarmTemplate);

void to_json(nlohmann::json& j, const ActuatorTemplate& actuatorTemplate);
void from_json(const nlohmann::json& j, ActuatorTemplate& actuatorTemplate);

void to_json(nlohmann::json& j, const SensorTemplate& sensorTemplate);
void from_json(const nlohmann::json& j, SensorTemplate& sensorTemplate);

void to_json(nlohmann::json& j, const DeviceTemplate& deviceTemplate);
void from_json(const nlohmann::json& j, DeviceTemplate& deviceTemplate);

void to_json(nlohmann::json& j, const GatewayUpdateRequest& dto);

void to_json(nlohmann::json& j, const SubdeviceRegistrationRequest& dto);
SubdeviceRegistrationRequest subdevice_registration_request_from_json(const nlohmann::json& j);

void to_json(nlohmann::json& j, const GatewayUpdateResponse& dto);
GatewayUpdateResponse gateway_update_response_from_json(const nlohmann::json& j);

void to_json(nlohmann::json& j, const SubdeviceRegistrationResponse& dto);
SubdeviceRegistrationResponse subdevice_registration_response_from_json(const nlohmann::json& j);

void to_json(nlohmann::json& j, const SubdeviceUpdateRequest& dto);

SubdeviceUpdateResponse subdevice_update_response_from_json(const nlohmann::json& j, const std::string& deviceKey);

PlatformResult platform_result_from_json(const nlohmann::json& j);
}    // namespace wolkabout

#endif    // JSONDTO_H

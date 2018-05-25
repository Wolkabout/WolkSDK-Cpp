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

#ifndef JSONDTO_H
#define JSONDTO_H

#include "utilities/json.hpp"

namespace wolkabout
{
class ActuatorManifest;
class AlarmManifest;
class ConfigurationManifest;
class DeviceManifest;
class DeviceRegistrationRequest;
class DeviceRegistrationResponse;
class DeviceReregistrationResponse;
class SensorManifest;

void to_json(nlohmann::json& j, const ConfigurationManifest& configurationManifest);
void from_json(const nlohmann::json& j, ConfigurationManifest& configurationManifest);

void to_json(nlohmann::json& j, const AlarmManifest& alarmManfiest);
void from_json(const nlohmann::json& j, AlarmManifest& alarmManifest);

void to_json(nlohmann::json& j, const ActuatorManifest& actuatorManfiest);
void from_json(const nlohmann::json& j, ActuatorManifest& actuatorManifest);

void to_json(nlohmann::json& j, const SensorManifest& sensorManifest);
void from_json(const nlohmann::json& j, SensorManifest& sensorManifest);

void to_json(nlohmann::json& j, const DeviceManifest& deviceManifest);
void from_json(const nlohmann::json& j, DeviceManifest& deviceManifest);

void to_json(nlohmann::json& j, const DeviceRegistrationRequest& dto);
void from_json(const nlohmann::json& j, DeviceRegistrationRequest& dto);

void to_json(nlohmann::json& j, const DeviceRegistrationResponse& dto);

void to_json(nlohmann::json& j, const DeviceReregistrationResponse& dto);
}

#endif    // JSONDTO_H

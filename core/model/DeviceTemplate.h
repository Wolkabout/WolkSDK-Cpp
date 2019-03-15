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

#ifndef DEVICETEMPLATE_H
#define DEVICETEMPLATE_H

#include "model/ActuatorTemplate.h"
#include "model/AlarmTemplate.h"
#include "model/ConfigurationTemplate.h"
#include "model/SensorTemplate.h"

#include <functional>
#include <map>
#include <memory>
#include <string>
#include <vector>

namespace wolkabout
{
class DeviceTemplate
{
public:
    explicit DeviceTemplate(std::vector<ConfigurationTemplate> configurations = {},
                            std::vector<SensorTemplate> sensors = {}, std::vector<AlarmTemplate> alarms = {},
                            std::vector<ActuatorTemplate> actuators = {}, std::string firmwareUpdateType = "",
                            std::map<std::string, std::string> typeParameters = {},
                            std::map<std::string, std::string> connectivityParameters = {},
                            std::map<std::string, bool> firmwareUpdateParameters = {});

    DeviceTemplate& addConfiguration(const ConfigurationTemplate& configurationTemplate);
    DeviceTemplate& addSensor(const SensorTemplate& sensorTemplate);
    DeviceTemplate& addAlarm(const AlarmTemplate& alarmTemplate);
    DeviceTemplate& addActuator(const ActuatorTemplate& actuatorTemplate);
    DeviceTemplate& addTypeParameter(const std::pair<std::string, std::string>& typeParameter);
    DeviceTemplate& addConnectivityParameter(const std::pair<std::string, std::string>& connectivityParameter);
    DeviceTemplate& addFirmwareUpdateParameter(const std::pair<std::string, bool>& firmwareUpdateParameter);

    const std::vector<ConfigurationTemplate>& getConfigurations() const;
    const std::vector<SensorTemplate>& getSensors() const;
    const std::vector<AlarmTemplate>& getAlarms() const;
    const std::vector<ActuatorTemplate>& getActuators() const;

    const std::string& getFirmwareUpdateType() const;

    const std::map<std::string, std::string>& getTypeParameters() const;
    const std::map<std::string, std::string>& getConnectivityParameters() const;
    const std::map<std::string, bool>& getFirmwareUpdateParameters() const;

    std::unique_ptr<ConfigurationTemplate> getConfigurationTemplate(
      std::function<bool(const ConfigurationTemplate&)> filter) const;
    std::unique_ptr<SensorTemplate> getSensorTemplate(std::function<bool(const SensorTemplate&)> filter) const;
    std::unique_ptr<AlarmTemplate> getAlarmTemplate(std::function<bool(const AlarmTemplate&)> filter) const;
    std::unique_ptr<ActuatorTemplate> getActuatorTemplate(std::function<bool(const ActuatorTemplate&)> filter) const;

    bool hasConfigurationTemplateWithReference(const std::string& reference) const;
    bool hasSensorTemplateWithReference(const std::string& reference) const;
    bool hasAlarmTemplateWithReference(const std::string& reference) const;
    bool hasActuatorTemplateWithReference(const std::string& reference) const;

    bool operator==(DeviceTemplate& rhs) const;
    bool operator!=(DeviceTemplate& rhs) const;

private:
    std::vector<ConfigurationTemplate> m_configurations;
    std::vector<SensorTemplate> m_sensors;
    std::vector<AlarmTemplate> m_alarms;
    std::vector<ActuatorTemplate> m_actuators;

    std::string m_firmwareUpdateType;
    std::map<std::string, std::string> m_typeParameters;
    std::map<std::string, std::string> m_connectivityParameters;
    std::map<std::string, bool> m_firmwareUpdateParameters;
};
}    // namespace wolkabout

#endif    // DEVICETEMPLATE_H

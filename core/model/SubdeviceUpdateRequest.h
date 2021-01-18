/*
 * Copyright 2020 WolkAbout Technology s.r.o.
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

#ifndef SUBDEVICEUPDATEREQUEST_H
#define SUBDEVICEUPDATEREQUEST_H

#include "model/DetailedDevice.h"
#include "model/DeviceTemplate.h"

#include <string>

namespace wolkabout
{
class SubdeviceUpdateRequest
{
public:
    SubdeviceUpdateRequest(std::string subdeviceKey, bool updateDefaultSemantics,
                           std::vector<ConfigurationTemplate> configurations = {},
                           std::vector<SensorTemplate> sensors = {}, std::vector<AlarmTemplate> alarms = {},
                           std::vector<ActuatorTemplate> actuators = {});

    const std::string& getSubdeviceKey() const;
    bool getUpdateDefaultSemantics() const;
    const std::vector<ConfigurationTemplate>& getConfigurations() const;
    const std::vector<SensorTemplate>& getSensors() const;
    const std::vector<AlarmTemplate>& getAlarms() const;
    const std::vector<ActuatorTemplate>& getActuators() const;

private:
    std::string m_subdeviceKey;
    bool m_updateDefaultSemantics;
    std::vector<ConfigurationTemplate> m_configurations;
    std::vector<SensorTemplate> m_sensors;
    std::vector<AlarmTemplate> m_alarms;
    std::vector<ActuatorTemplate> m_actuators;
};
}    // namespace wolkabout

#endif    // SUBDEVICEUPDATEREQUEST_H

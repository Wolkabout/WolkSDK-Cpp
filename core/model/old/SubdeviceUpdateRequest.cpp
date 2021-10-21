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

#include "SubdeviceUpdateRequest.h"

#include <utility>

namespace wolkabout
{
SubdeviceUpdateRequest::SubdeviceUpdateRequest(std::string subdeviceKey, bool updateDefaultSemantics,
                                               std::vector<ConfigurationTemplate> configurations,
                                               std::vector<SensorTemplate> sensors, std::vector<AlarmTemplate> alarms,
                                               std::vector<ActuatorTemplate> actuators)
: m_subdeviceKey{std::move(subdeviceKey)}
, m_updateDefaultSemantics{updateDefaultSemantics}
, m_configurations{std::move(configurations)}
, m_sensors{std::move(sensors)}
, m_alarms{std::move(alarms)}
, m_actuators{std::move(actuators)}
{
}

const std::string& SubdeviceUpdateRequest::getSubdeviceKey() const
{
    return m_subdeviceKey;
}

bool SubdeviceUpdateRequest::getUpdateDefaultSemantics() const
{
    return m_updateDefaultSemantics;
}

const std::vector<ConfigurationTemplate>& SubdeviceUpdateRequest::getConfigurations() const
{
    return m_configurations;
}

const std::vector<SensorTemplate>& SubdeviceUpdateRequest::getSensors() const
{
    return m_sensors;
}

const std::vector<AlarmTemplate>& SubdeviceUpdateRequest::getAlarms() const
{
    return m_alarms;
}

const std::vector<ActuatorTemplate>& SubdeviceUpdateRequest::getActuators() const
{
    return m_actuators;
}

}    // namespace wolkabout

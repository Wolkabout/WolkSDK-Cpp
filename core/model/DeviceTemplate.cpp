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

#include "model/DeviceTemplate.h"

#include <utility>

wolkabout::DeviceTemplate::DeviceTemplate(
  std::vector<wolkabout::ConfigurationTemplate> configurations, std::vector<wolkabout::SensorTemplate> sensors,
  std::vector<wolkabout::AlarmTemplate> alarms, std::vector<wolkabout::ActuatorTemplate> actuators,
  std::string firmwareUpdateType, std::map<std::string, std::string> typeParameters,
  std::map<std::string, std::string> connectivityParameters, std::map<std::string, bool> firmwareUpdateParameters)
: m_configurations(std::move(configurations))
, m_sensors(std::move(sensors))
, m_alarms(std::move(alarms))
, m_actuators(std::move(actuators))
, m_firmwareUpdateType(std::move(firmwareUpdateType))
, m_typeParameters(std::move(typeParameters))
, m_connectivityParameters(std::move(connectivityParameters))
, m_firmwareUpdateParameters(std::move(firmwareUpdateParameters))
{
}

wolkabout::DeviceTemplate& wolkabout::DeviceTemplate::addConfiguration(
  const wolkabout::ConfigurationTemplate& configurationTemplate)
{
    m_configurations.push_back(configurationTemplate);
    return *this;
}

wolkabout::DeviceTemplate& wolkabout::DeviceTemplate::addSensor(const wolkabout::SensorTemplate& sensorTemplate)
{
    m_sensors.push_back(sensorTemplate);
    return *this;
}

wolkabout::DeviceTemplate& wolkabout::DeviceTemplate::addAlarm(const wolkabout::AlarmTemplate& alarmTemplate)
{
    m_alarms.push_back(alarmTemplate);
    return *this;
}

wolkabout::DeviceTemplate& wolkabout::DeviceTemplate::addActuator(const wolkabout::ActuatorTemplate& actuatorTemplate)
{
    m_actuators.push_back(actuatorTemplate);
    return *this;
}

wolkabout::DeviceTemplate& wolkabout::DeviceTemplate::addTypeParameter(
  const std::pair<std::string, std::string>& typeParameter)
{
    m_typeParameters.insert(typeParameter);
    return *this;
}

wolkabout::DeviceTemplate& wolkabout::DeviceTemplate::addConnectivityParameter(
  const std::pair<std::string, std::string>& connectivityParameter)
{
    m_connectivityParameters.insert(connectivityParameter);
    return *this;
}

wolkabout::DeviceTemplate& wolkabout::DeviceTemplate::addFirmwareUpdateParameter(
  const std::pair<std::string, bool>& firmwareUpdateParameter)
{
    m_firmwareUpdateParameters.insert(firmwareUpdateParameter);
    return *this;
}

const std::vector<wolkabout::ConfigurationTemplate>& wolkabout::DeviceTemplate::getConfigurations() const
{
    return m_configurations;
}

const std::vector<wolkabout::SensorTemplate>& wolkabout::DeviceTemplate::getSensors() const
{
    return m_sensors;
}

const std::vector<wolkabout::AlarmTemplate>& wolkabout::DeviceTemplate::getAlarms() const
{
    return m_alarms;
}

const std::vector<wolkabout::ActuatorTemplate>& wolkabout::DeviceTemplate::getActuators() const
{
    return m_actuators;
}

std::unique_ptr<wolkabout::ConfigurationTemplate> wolkabout::DeviceTemplate::getConfigurationTemplate(
  std::function<bool(const ConfigurationTemplate&)> filter) const
{
    for (const ConfigurationTemplate& configurationTemplate : m_configurations)
    {
        if (filter(configurationTemplate))
        {
            return std::unique_ptr<ConfigurationTemplate>(new ConfigurationTemplate(configurationTemplate));
        }
    }

    return nullptr;
}

const std::string& wolkabout::DeviceTemplate::getFirmwareUpdateType() const
{
    return m_firmwareUpdateType;
}

const std::map<std::string, std::string>& wolkabout::DeviceTemplate::getTypeParameters() const
{
    return m_typeParameters;
}

const std::map<std::string, std::string>& wolkabout::DeviceTemplate::getConnectivityParameters() const
{
    return m_connectivityParameters;
}

const std::map<std::string, bool>& wolkabout::DeviceTemplate::getFirmwareUpdateParameters() const
{
    return m_firmwareUpdateParameters;
}

std::unique_ptr<wolkabout::ConfigurationTemplate> wolkabout::DeviceTemplate::getConfigurationTemplate(
  const std::string& reference) const
{
    return getConfigurationTemplate(
      [&](const ConfigurationTemplate& conf) { return conf.getReference() == reference; });
}

std::unique_ptr<wolkabout::SensorTemplate> wolkabout::DeviceTemplate::getSensorTemplate(
  const std::string& reference) const
{
    return getSensorTemplate([&](const SensorTemplate& sensor) { return sensor.getReference() == reference; });
}

std::unique_ptr<wolkabout::AlarmTemplate> wolkabout::DeviceTemplate::getAlarmTemplate(
  const std::string& reference) const
{
    return getAlarmTemplate([&](const AlarmTemplate& alarm) { return alarm.getReference() == reference; });
}

std::unique_ptr<wolkabout::ActuatorTemplate> wolkabout::DeviceTemplate::getActuatorTemplate(
  const std::string& reference) const
{
    return getActuatorTemplate([&](const ActuatorTemplate& actuator) { return actuator.getReference() == reference; });
}

std::unique_ptr<wolkabout::SensorTemplate> wolkabout::DeviceTemplate::getSensorTemplate(
  std::function<bool(const SensorTemplate&)> filter) const
{
    for (const SensorTemplate& sensorTemplate : m_sensors)
    {
        if (filter(sensorTemplate))
        {
            return std::unique_ptr<SensorTemplate>(new SensorTemplate(sensorTemplate));
        }
    }

    return nullptr;
}

std::unique_ptr<wolkabout::AlarmTemplate> wolkabout::DeviceTemplate::getAlarmTemplate(
  std::function<bool(const AlarmTemplate&)> filter) const
{
    for (const AlarmTemplate& alarmTemplate : m_alarms)
    {
        if (filter(alarmTemplate))
        {
            return std::unique_ptr<AlarmTemplate>(new AlarmTemplate(alarmTemplate));
        }
    }

    return nullptr;
}

std::unique_ptr<wolkabout::ActuatorTemplate> wolkabout::DeviceTemplate::getActuatorTemplate(
  std::function<bool(const ActuatorTemplate&)> filter) const
{
    for (const ActuatorTemplate& actuatorTemplate : m_actuators)
    {
        if (filter(actuatorTemplate))
        {
            return std::unique_ptr<ActuatorTemplate>(new ActuatorTemplate(actuatorTemplate));
        }
    }

    return nullptr;
}

bool wolkabout::DeviceTemplate::hasConfigurationTemplateWithReference(const std::string& reference) const
{
    for (const ConfigurationTemplate& configurationTemplate : m_configurations)
    {
        if (configurationTemplate.getReference() == reference)
        {
            return true;
        }
    }

    return false;
}

bool wolkabout::DeviceTemplate::hasSensorTemplateWithReference(const std::string& reference) const
{
    for (const SensorTemplate& sensorTemplate : m_sensors)
    {
        if (sensorTemplate.getReference() == reference)
        {
            return true;
        }
    }

    return false;
}

bool wolkabout::DeviceTemplate::hasAlarmTemplateWithReference(const std::string& reference) const
{
    for (const AlarmTemplate& alarmTemplate : m_alarms)
    {
        if (alarmTemplate.getReference() == reference)
        {
            return true;
        }
    }

    return false;
}

bool wolkabout::DeviceTemplate::hasActuatorTemplateWithReference(const std::string& reference) const
{
    for (const ActuatorTemplate& actuatorTemplate : m_actuators)
    {
        if (actuatorTemplate.getReference() == reference)
        {
            return true;
        }
    }

    return false;
}

bool wolkabout::DeviceTemplate::operator==(DeviceTemplate& rhs) const
{
    if (m_configurations.size() != rhs.m_configurations.size())
    {
        return false;
    }

    if (m_sensors.size() != rhs.m_sensors.size())
    {
        return false;
    }

    if (m_actuators.size() != rhs.m_actuators.size())
    {
        return false;
    }

    if (m_alarms.size() != rhs.m_alarms.size())
    {
        return false;
    }

    if (m_typeParameters != rhs.m_typeParameters)
    {
        return false;
    }

    if (m_connectivityParameters != rhs.m_connectivityParameters)
    {
        return false;
    }

    if (m_firmwareUpdateParameters != rhs.m_firmwareUpdateParameters)
    {
        return false;
    }

    for (unsigned long long int i = 0; i < m_configurations.size(); ++i)
    {
        if (m_configurations.at(i) != rhs.m_configurations.at(i))
        {
            return false;
        }
    }

    for (unsigned long long int i = 0; i < m_sensors.size(); ++i)
    {
        if (m_sensors.at(i) != rhs.m_sensors.at(i))
        {
            return false;
        }
    }

    for (unsigned long long int i = 0; i < m_actuators.size(); ++i)
    {
        if (m_actuators.at(i) != rhs.m_actuators.at(i))
        {
            return false;
        }
    }

    for (unsigned long long int i = 0; i < m_alarms.size(); ++i)
    {
        if (m_alarms.at(i) != rhs.m_alarms.at(i))
        {
            return false;
        }
    }

    return true;
}

bool wolkabout::DeviceTemplate::operator!=(DeviceTemplate& rhs) const
{
    return !(*this == rhs);
}

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

#include "model/DetailedDevice.h"

#include <string>
#include <utility>

namespace wolkabout
{
DetailedDevice::DetailedDevice(std::string name, std::string key, DeviceManifest deviceManifest)
: DetailedDevice(std::move(name), std::move(key), "", std::move(deviceManifest));
{
}

DetailedDevice::DetailedDevice(std::string name, std::string key, std::string password, DeviceManifest deviceManifest)
: DetailedDevice(std::move(name), std::move(key), std:move(password), std::move(deviceManifest), /* TODO: flags*/)
{
}

DetailedDevice::DetailedDevice(std::string name, std::string key, std::string password, DeviceManifest deviceManifest, bool manageSubdevices)
: m_name(std::move(name))
, m_key(std::move(key))
, m_password(std::move(password))
, m_deviceManifest(std::move(deviceManifest))
{
    //TODO: handle manageSubdevices
    //TODO: Firmware update and file transfer flags
}

const std::string& DetailedDevice::getName() const
{
    return m_name;
}

const std::string& DetailedDevice::getKey() const
{
    return m_key;
}

const std::string& DetailedDevice::getPassword() const
{
    return m_password;
}

const DeviceManifest& DetailedDevice::getManifest() const
{
    return m_deviceManifest;
}

std::vector<std::string> DetailedDevice::getActuatorReferences() const
{
    std::vector<std::string> actuatorReferences;
    for (const ActuatorManifest& actuatorManifest : m_deviceManifest.getActuators())
    {
        actuatorReferences.push_back(actuatorManifest.getReference());
    }

    return actuatorReferences;
}

std::map<std::string, std::string> DetailedDevice::getSensorDelimiters() const
{
    std::map<std::string, std::string> delimiters;

    const auto sensors = m_deviceManifest.getSensors();
    for (const auto& sensor : sensors)
    {
        if (!sensor.getDelimiter().empty())
        {
            delimiters[sensor.getReference()] = sensor.getDelimiter();
        }
    }

    return delimiters;
}

std::map<std::string, std::string> DetailedDevice::getConfigurationDelimiters() const
{
    std::map<std::string, std::string> delimiters;

    const auto configurationItems = m_deviceManifest.getConfigurations();
    for (const auto& item : configurationItems)
    {
        if (!item.getDelimiter().empty())
        {
            delimiters[item.getReference()] = item.getDelimiter();
        }
    }

    return delimiters;
}

bool DetailedDevice::operator==(DetailedDevice& rhs) const
{
    if (m_key != rhs.m_key || m_name != rhs.m_name || m_password != rhs.m_password ||
        m_deviceManifest != rhs.m_deviceManifest)
    {
        return false;
    }

    return true;
}

bool DetailedDevice::operator!=(DetailedDevice& rhs) const
{
    return !(*this == rhs);
}
}    // namespace wolkabout

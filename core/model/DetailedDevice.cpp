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

#include "model/DetailedDevice.h"

#include <utility>

namespace wolkabout
{
DetailedDevice::DetailedDevice(std::string name, std::string key, DeviceTemplate deviceTemplate)
: DetailedDevice(std::move(name), std::move(key), "", std::move(deviceTemplate))
{
}

DetailedDevice::DetailedDevice(std::string name, std::string key, std::string password, DeviceTemplate deviceTemplate)
: m_name(std::move(name))
, m_key(std::move(key))
, m_password(std::move(password))
, m_deviceTemplate(std::move(deviceTemplate))
{
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

const DeviceTemplate& DetailedDevice::getTemplate() const
{
    return m_deviceTemplate;
}

std::vector<std::string> DetailedDevice::getActuatorReferences() const
{
    std::vector<std::string> actuatorReferences;
    for (const ActuatorTemplate& actuatorTemplate : m_deviceTemplate.getActuators())
    {
        actuatorReferences.push_back(actuatorTemplate.getReference());
    }

    return actuatorReferences;
}

bool DetailedDevice::operator==(DetailedDevice& rhs) const
{
    if (m_key != rhs.m_key || m_name != rhs.m_name || m_password != rhs.m_password ||
        m_deviceTemplate != rhs.m_deviceTemplate)
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

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

#include "SubdeviceRegistrationRequest.h"

#include <utility>

namespace wolkabout
{
SubdeviceRegistrationRequest::SubdeviceRegistrationRequest(std::string subdeviceName, std::string subdeviceKey,
                                                           DeviceTemplate subdeviceTemplate, bool defaultBinding,
                                                           std::string type)
: m_subdeviceName(std::move(subdeviceName))
, m_subdeviceKey(std::move(subdeviceKey))
, m_subdeviceTemplate(std::move(subdeviceTemplate))
, m_defaultBinding(defaultBinding)
, m_type(std::move(type))
{
}

SubdeviceRegistrationRequest::SubdeviceRegistrationRequest(DetailedDevice device, bool defaultBinding, std::string type)
: m_subdeviceName(device.getName())
, m_subdeviceKey(device.getKey())
, m_subdeviceTemplate(device.getTemplate())
, m_defaultBinding(defaultBinding)
, m_type(std::move(type))
{
}

const std::string& SubdeviceRegistrationRequest::getSubdeviceName() const
{
    return m_subdeviceName;
}

const std::string& SubdeviceRegistrationRequest::getSubdeviceKey() const
{
    return m_subdeviceKey;
}

bool SubdeviceRegistrationRequest::getDefaultBinding() const
{
    return m_defaultBinding;
}

std::string SubdeviceRegistrationRequest::getType() const
{
    return m_type;
}

const DeviceTemplate& SubdeviceRegistrationRequest::getTemplate() const
{
    return m_subdeviceTemplate;
}
}    // namespace wolkabout

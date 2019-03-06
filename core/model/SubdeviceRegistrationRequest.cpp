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

#include "model/SubdeviceRegistrationRequest.h"

#include <string>
#include <utility>

namespace wolkabout
{
SubdeviceRegistrationRequest::SubdeviceRegistrationRequest(std::string subdeviceName, std::string subdeviceKey,
                                                           DeviceTemplate subdeviceTemplate, bool defaultBinding)
: m_subdeviceName(std::move(subdeviceName))
, m_subdeviceKey(std::move(subdeviceKey))
, m_defaultBinding(std::move(defaultBinding))
, m_subdeviceTemplate(std::move(subdeviceTemplate))
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

const bool& SubdeviceRegistrationRequest::getDefaultBinding() const
{
    return m_defaultBinding;
}

const DeviceTemplate& SubdeviceRegistrationRequest::getTemplate() const
{
    return m_subdeviceTemplate;
}
}    // namespace wolkabout

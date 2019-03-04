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
#include "model/DetailedDevice.h"
#include "model/DeviceManifest.h"

#include <utility>

namespace wolkabout
{
SubdeviceRegistrationRequest::SubdeviceRegistrationRequest(std::string subdeviceName, std::string subdeviceKey,
                                                     DeviceManifest subdeviceManifest)
: m_subdevice(std::move(subdeviceName), std::move(subdeviceKey), std::move(subdeviceManifest))
{
}

SubdeviceRegistrationRequest::SubdeviceRegistrationRequest(DetailedDevice subdevice) : m_subdevice(std::move(subdevice)) {}

const std::string& SubdeviceRegistrationRequest::getDeviceName() const
{
    return m_subdevice.getName();
}

const std::string& SubdeviceRegistrationRequest::getDeviceKey() const
{
    return m_subdevice.getKey();
}

const DeviceManifest& SubdeviceRegistrationRequest::getManifest() const
{
    return m_subdevice.getManifest();
}
}    // namespace wolkabout

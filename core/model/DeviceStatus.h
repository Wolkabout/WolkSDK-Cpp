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

#ifndef DEVICESTATUS_H
#define DEVICESTATUS_H

#include <string>

namespace wolkabout
{
class DeviceStatus
{
public:
    enum class Status
    {
        CONNECTED,
        OFFLINE,
        SLEEP,
        SERVICE
    };

    DeviceStatus(std::string deviceKey, DeviceStatus::Status status);

    const std::string& getDeviceKey() const;
    DeviceStatus::Status getStatus() const;

private:
    std::string m_deviceKey;
    DeviceStatus::Status m_status;
};
}    // namespace wolkabout

#endif    // DEVICESTATUS_H

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

#include "FirmwareUpdateInstall.h"

namespace wolkabout
{
FirmwareUpdateInstall::FirmwareUpdateInstall(std::vector<std::string> deviceKeys, std::string name, std::string hash)
: m_deviceKeys{std::move(deviceKeys)}, m_name{std::move(name)}, m_hash{std::move(hash)}
{
}

FirmwareUpdateInstall::FirmwareUpdateInstall(std::string name, std::string hash)
: FirmwareUpdateInstall({}, std::move(name), std::move(hash))
{
}

const std::vector<std::string>& FirmwareUpdateInstall::getDeviceKeys() const
{
    return m_deviceKeys;
}

const std::string& FirmwareUpdateInstall::getName() const
{
    return m_name;
}

const std::string& FirmwareUpdateInstall::getHash() const
{
    return m_hash;
}
}    // namespace wolkabout

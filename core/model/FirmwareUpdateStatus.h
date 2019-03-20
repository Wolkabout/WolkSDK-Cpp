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

#ifndef FIRMWAREUPDATESTATUS_H
#define FIRMWAREUPDATESTATUS_H

#include "model/WolkOptional.h"

#include <string>
#include <vector>

namespace wolkabout
{
class FirmwareUpdateStatus
{
public:
    enum class Status
    {
        INSTALLATION = 0,
        COMPLETED,
        ERROR,
        ABORTED
    };

    enum class Error
    {
        UNSPECIFIED_ERROR = 0,
        FILE_NOT_PRESENT = 1,
        FILE_HASH_MISMATCH = 2,
        FILE_SYSTEM_ERROR = 3,
        INSTALLATION_FAILED = 4,
        SUBDEVICE_NOT_PRESENT = 5
    };

    FirmwareUpdateStatus(std::vector<std::string> deviceKeys, FirmwareUpdateStatus::Status status);
    FirmwareUpdateStatus(std::vector<std::string> deviceKeys, FirmwareUpdateStatus::Error errorCode);

    const std::vector<std::string>& getDeviceKeys() const;
    FirmwareUpdateStatus::Status getStatus() const;
    const WolkOptional<FirmwareUpdateStatus::Error>& getErrorCode() const;

private:
    std::vector<std::string> m_deviceKeys;
    FirmwareUpdateStatus::Status m_status;
    WolkOptional<FirmwareUpdateStatus::Error> m_errorCode;
};
}    // namespace wolkabout

#endif    // FIRMWAREUPDATESTATUS_H

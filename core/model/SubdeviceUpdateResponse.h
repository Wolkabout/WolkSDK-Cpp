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

#ifndef SUBDEVICEUPDATERESPONSE_H
#define SUBDEVICEUPDATERESPONSE_H

#include <string>

namespace wolkabout
{
class SubdeviceUpdateResponse
{
public:
    enum class Result
    {
        OK,
        GATEWAY_NOT_FOUND,
        VALIDATION_ERROR,
        SUBDEVICE_MANAGEMENT_FORBIDDEN,
        NOT_A_GATEWAY,
        MISSING_UNIT,
        ERROR_UNKNOWN
    };

    SubdeviceUpdateResponse(std::string subdeviceKey, Result result, std::string description = "");

    Result getResult() const;

    const std::string& getSubdeviceKey() const;

    const std::string& getDescription() const;

private:
    std::string m_subdeviceKey;
    Result m_result;
    std::string m_description;
};
}    // namespace wolkabout

#endif    // SUBDEVICEUPDATERESPONSE_H

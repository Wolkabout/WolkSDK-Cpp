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

#ifndef SUBDEVICEREREGISTRATIONRESPONSE_H
#define SUBDEVICEREREGISTRATIONRESPONSE_H

#include <string>

namespace wolkabout
{
class SubdeviceReregistrationResponse
{
public:
    enum class Result
    {
        OK = 0,
        ERROR_GATEWAY_NOT_FOUND,
        ERROR_NOT_A_GATEWAY,
        ERROR_VALIDATION_ERROR,
        ERROR_INVALID_DTO,
        ERROR_KEY_MISSING,
        ERROR_SUBDEVICE_MANAGEMENT_FORBIDDEN,
        ERROR_UNKNOWN

    };

    SubdeviceReregistrationResponse() = default;
    SubdeviceReregistrationResponse(SubdeviceReregistrationResponse::Result result, std::string description = "");

    virtual ~SubdeviceReregistrationResponse() = default;

    SubdeviceReregistrationResponse::Result getResult() const;

    const std::string& getDescription() const;

private:
    SubdeviceReregistrationResponse::Result m_result;
    std::string m_description;
};
}    // namespace wolkabout

#endif    // SUBDEVICEREREGISTRATIONRESPONSE_H

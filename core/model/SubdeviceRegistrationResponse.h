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

#ifndef SUBDEVICEREGISTRATIONRESPONSE_H
#define SUBDEVICEREGISTRATIONRESPONSE_H

#include <string>

namespace wolkabout
{
class SubdeviceRegistrationResponse
{
public:
    enum class Result
    {
        OK,
        ERROR_GATEWAY_NOT_FOUND,
        ERROR_NOT_A_GATEWAY,
        ERROR_KEY_CONFLICT,
        ERROR_MAXIMUM_NUMBER_OF_DEVICES_EXCEEDED,
        ERROR_VALIDATION_ERROR,
        ERROR_INVALID_DTO,
        ERROR_KEY_MISSING,
        ERROR_UNKNOWN
    };

    SubdeviceRegistrationResponse() = default;
    SubdeviceRegistrationResponse(SubdeviceRegistrationResponse::Result result, std::string description = "");

    virtual ~SubdeviceRegistrationResponse() = default;

    const SubdeviceRegistrationResponse::Result& getResult() const;

    const std::string& getDescription() const;

private:
    SubdeviceRegistrationResponse::Result m_result;
    std::string m_description;
};
}    // namespace wolkabout

#endif    // SUBDEVICEREGISTRATIONRESPONSE_H

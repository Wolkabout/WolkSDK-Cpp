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

#ifndef GATEWAYUPDATERESPONSE_H
#define GATEWAYUPDATERESPONSE_H

#include <string>

namespace wolkabout
{
class GatewayUpdateResponse
{
public:
    enum class Result
    {
        OK,
        ERROR_GATEWAY_NOT_FOUND,
        ERROR_NOT_A_GATEWAY,
        ERROR_KEY_CONFLICT,
        ERROR_VALIDATION_ERROR,
        ERROR_INVALID_DTO,
        ERROR_KEY_MISSING,
        ERROR_UNKNOWN
    };

    GatewayUpdateResponse() = default;
    explicit GatewayUpdateResponse(GatewayUpdateResponse::Result result, std::string description = "");

    const GatewayUpdateResponse::Result& getResult() const;

    const std::string& getDescription() const;

private:
    GatewayUpdateResponse::Result m_result;
    std::string m_description;
};
}    // namespace wolkabout

#endif    // GATEWAYUPDATERESPONSE_H

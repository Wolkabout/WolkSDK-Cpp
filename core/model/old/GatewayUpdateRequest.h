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

#ifndef GATEWAYUPDATEREQUEST_H
#define GATEWAYUPDATEREQUEST_H

#include "core/model/DetailedDevice.h"
#include "core/model/DeviceTemplate.h"

#include <string>

namespace wolkabout
{
class GatewayUpdateRequest
{
public:
    GatewayUpdateRequest() = default;
    GatewayUpdateRequest(std::string gatewayName, std::string gatewayKey, DeviceTemplate gatewayTemplate);
    explicit GatewayUpdateRequest(DetailedDevice gateway);

    virtual ~GatewayUpdateRequest() = default;

    const std::string& getGatewayName() const;
    const std::string& getGatewayKey() const;

    const DeviceTemplate& getTemplate() const;

private:
    DetailedDevice m_gateway;
};
}    // namespace wolkabout

#endif    // GATEWAYUPDATEREQUEST_H

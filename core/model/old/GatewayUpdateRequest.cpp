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

#include "GatewayUpdateRequest.h"

#include <utility>

namespace wolkabout
{
GatewayUpdateRequest::GatewayUpdateRequest(std::string gatewayName, std::string gatewayKey,
                                           DeviceTemplate gatewayTemplate)
: m_gateway(std::move(gatewayName), std::move(gatewayKey), std::move(gatewayTemplate))
{
}

GatewayUpdateRequest::GatewayUpdateRequest(DetailedDevice gateway) : m_gateway(std::move(gateway)) {}

const std::string& GatewayUpdateRequest::getGatewayName() const
{
    return m_gateway.getName();
}

const std::string& GatewayUpdateRequest::getGatewayKey() const
{
    return m_gateway.getKey();
}

const DeviceTemplate& GatewayUpdateRequest::getTemplate() const
{
    return m_gateway.getTemplate();
}
}    // namespace wolkabout

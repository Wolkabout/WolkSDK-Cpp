/**
 * Copyright 2022 Wolkabout Technology s.r.o.
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

#ifndef WOLKABOUTCORE_WOLKABOUTGATEWAYPLATFORMSTATUSPROTOCOL_H
#define WOLKABOUTCORE_WOLKABOUTGATEWAYPLATFORMSTATUSPROTOCOL_H

#include "core/protocol/GatewayPlatformStatusProtocol.h"

namespace wolkabout
{
class WolkaboutGatewayPlatformStatusProtocol : public GatewayPlatformStatusProtocol
{
public:
    std::vector<std::string> getInboundChannels() const override;

    std::vector<std::string> getInboundChannelsForDevice(const std::string& deviceKey) const override;

    MessageType getMessageType(const Message& message) override;

    DeviceType getDeviceType(const Message& message) override;

    std::string getDeviceKey(const Message& message) const override;

    std::string getResponseChannelForMessage(MessageType type, const std::string& deviceKey) const override;

    std::unique_ptr<Message> makeOutboundMessage(const std::string& deviceKey,
                                                 const PlatformStatusMessage& message) override;
};
}    // namespace wolkabout

#endif    // WOLKABOUTCORE_WOLKABOUTGATEWAYPLATFORMSTATUSPROTOCOL_H

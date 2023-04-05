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

#ifndef WOLKGATEWAY_GATEWAYREGISTRATIONPROTOCOLMOCK_H
#define WOLKGATEWAY_GATEWAYREGISTRATIONPROTOCOLMOCK_H

#include "core/protocol/GatewayRegistrationProtocol.h"

#include <gmock/gmock.h>

using namespace wolkabout;
using namespace wolkabout::legacy;

class GatewayRegistrationProtocolMock : public GatewayRegistrationProtocol
{
public:
    // Protocol methods
    MOCK_METHOD(std::vector<std::string>, getInboundChannels, (), (const));
    MOCK_METHOD(std::vector<std::string>, getInboundChannelsForDevice, (const std::string&), (const));
    MOCK_METHOD(MessageType, getMessageType, (const Message&));
    MOCK_METHOD(DeviceType, getDeviceType, (const Message&));
    MOCK_METHOD(std::string, getDeviceKey, (const Message&), (const));
    MOCK_METHOD(std::string, getResponseChannelForMessage, (MessageType, const std::string&), (const));
    // GatewayRegistrationProtocol methods
    MOCK_METHOD(std::unique_ptr<Message>, makeOutboundMessage,
                (const std::string&, const DeviceRegistrationResponseMessage&));
    MOCK_METHOD(std::unique_ptr<Message>, makeOutboundMessage,
                (const std::string&, const RegisteredDevicesResponseMessage&));
    MOCK_METHOD(std::unique_ptr<DeviceRegistrationMessage>, parseDeviceRegistrationMessage,
                (const std::shared_ptr<Message>&));
    MOCK_METHOD(std::unique_ptr<DeviceRemovalMessage>, parseDeviceRemovalMessage, (const std::shared_ptr<Message>&));
    MOCK_METHOD(std::unique_ptr<RegisteredDevicesRequestMessage>, parseRegisteredDevicesRequestMessage,
                (const std::shared_ptr<Message>&));
};

#endif    // WOLKGATEWAY_GATEWAYREGISTRATIONPROTOCOLMOCK_H

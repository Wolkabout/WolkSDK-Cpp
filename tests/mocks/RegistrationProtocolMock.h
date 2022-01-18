/**
 * Copyright 2021 Wolkabout s.r.o.
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

#ifndef WOLKABOUTCONNECTOR_REGISTRATIONPROTOCOLMOCK_H
#define WOLKABOUTCONNECTOR_REGISTRATIONPROTOCOLMOCK_H

#include "core/protocol/RegistrationProtocol.h"

#include <gmock/gmock.h>

using namespace wolkabout;

class RegistrationProtocolMock : public RegistrationProtocol
{
public:
    // Protocol methods
    MOCK_METHOD(std::vector<std::string>, getInboundChannels, (), (const));
    MOCK_METHOD(std::vector<std::string>, getInboundChannelsForDevice, (const std::string&), (const));
    MOCK_METHOD(MessageType, getMessageType, (const Message&));
    MOCK_METHOD(DeviceType, getDeviceType, (const Message&));
    MOCK_METHOD(std::string, getDeviceKey, (const Message&), (const));
    // RegistrationProtocol methods
    MOCK_METHOD(std::unique_ptr<Message>, makeOutboundMessage, (const std::string&, const DeviceRegistrationMessage&));
    MOCK_METHOD(std::unique_ptr<Message>, makeOutboundMessage, (const std::string&, const DeviceRemovalMessage&));
    MOCK_METHOD(std::unique_ptr<Message>, makeOutboundMessage,
                (const std::string&, const RegisteredDevicesRequestMessage&));
    MOCK_METHOD(std::unique_ptr<RegisteredDevicesResponseMessage>, parseRegisteredDevicesResponse,
                (const std::shared_ptr<Message>&));
};

#endif    // WOLKABOUTCONNECTOR_REGISTRATIONPROTOCOLMOCK_H

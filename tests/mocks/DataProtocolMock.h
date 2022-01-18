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

#ifndef WOLKABOUTCONNECTOR_DATAPROTOCOLMOCK_H
#define WOLKABOUTCONNECTOR_DATAPROTOCOLMOCK_H

#include "core/protocol/DataProtocol.h"

#include <gmock/gmock.h>

using namespace wolkabout;

class DataProtocolMock : public DataProtocol
{
public:
    // Protocol methods
    MOCK_METHOD(std::vector<std::string>, getInboundChannels, (), (const));
    MOCK_METHOD(std::vector<std::string>, getInboundChannelsForDevice, (const std::string&), (const));
    MOCK_METHOD(MessageType, getMessageType, (const Message&));
    MOCK_METHOD(DeviceType, getDeviceType, (const Message&));
    MOCK_METHOD(std::string, getDeviceKey, (const Message&), (const));
    // DataProtocol methods
    MOCK_METHOD(std::unique_ptr<Message>, makeOutboundMessage, (const std::string&, FeedRegistrationMessage));
    MOCK_METHOD(std::unique_ptr<Message>, makeOutboundMessage, (const std::string&, FeedRemovalMessage));
    MOCK_METHOD(std::unique_ptr<Message>, makeOutboundMessage, (const std::string&, FeedValuesMessage));
    MOCK_METHOD(std::unique_ptr<Message>, makeOutboundMessage, (const std::string&, PullFeedValuesMessage));
    MOCK_METHOD(std::unique_ptr<Message>, makeOutboundMessage, (const std::string&, AttributeRegistrationMessage));
    MOCK_METHOD(std::unique_ptr<Message>, makeOutboundMessage, (const std::string&, ParametersUpdateMessage));
    MOCK_METHOD(std::unique_ptr<Message>, makeOutboundMessage, (const std::string&, ParametersPullMessage));
    MOCK_METHOD(std::unique_ptr<Message>, makeOutboundMessage, (const std::string&, SynchronizeParametersMessage));
    MOCK_METHOD(std::shared_ptr<FeedValuesMessage>, parseFeedValues, (std::shared_ptr<Message>));
    MOCK_METHOD(std::shared_ptr<ParametersUpdateMessage>, parseParameters, (std::shared_ptr<Message>));
};

#endif    // WOLKABOUTCONNECTOR_DATAPROTOCOLMOCK_H

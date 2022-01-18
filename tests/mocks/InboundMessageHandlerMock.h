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

#ifndef WOLKABOUTCONNECTOR_INBOUNDMESSAGEHANDLERMOCK_H
#define WOLKABOUTCONNECTOR_INBOUNDMESSAGEHANDLERMOCK_H

#include "core/connectivity/InboundMessageHandler.h"

#include <gmock/gmock.h>

using namespace wolkabout;

class InboundMessageHandlerMock : public InboundMessageHandler
{
public:
    MOCK_METHOD(void, messageReceived, (const std::string&, const std::string&));
    MOCK_METHOD(std::vector<std::string>, getChannels, (), (const));
    MOCK_METHOD(void, addListener, (std::weak_ptr<MessageListener>));
};

#endif    // WOLKABOUTCONNECTOR_INBOUNDMESSAGEHANDLERMOCK_H

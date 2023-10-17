/**
 * Copyright 2023 Wolkabout Technology s.r.o.
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

#ifndef WOLKGATEWAY_OUTBOUNDRETRYMESSAGEHANDLERMOCK_H
#define WOLKGATEWAY_OUTBOUNDRETRYMESSAGEHANDLERMOCK_H

#include "core/connectivity/OutboundRetryMessageHandler.h"

#include <gmock/gmock.h>

using namespace wolkabout;
using namespace wolkabout::legacy;

class OutboundRetryMessageHandlerMock : public OutboundRetryMessageHandler
{
public:
    explicit OutboundRetryMessageHandlerMock(OutboundMessageHandler& messageHandler)
    : OutboundRetryMessageHandler(messageHandler)
    {
    }
    MOCK_METHOD(void, addMessage, (RetryMessageStruct));
    MOCK_METHOD(void, messageReceived, (std::shared_ptr<Message>));
};

#endif    // WOLKGATEWAY_OUTBOUNDRETRYMESSAGEHANDLERMOCK_H

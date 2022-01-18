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

#ifndef WOLKABOUTCONNECTOR_CONNECTIVITYSERVICEMOCK_H
#define WOLKABOUTCONNECTOR_CONNECTIVITYSERVICEMOCK_H

#include "core/connectivity/ConnectivityService.h"

#include <gmock/gmock.h>

using namespace wolkabout;

class ConnectivityServiceMock : public ConnectivityService
{
public:
    MOCK_METHOD(bool, connect, ());
    MOCK_METHOD(void, disconnect, ());
    MOCK_METHOD(bool, reconnect, ());
    MOCK_METHOD(bool, isConnected, ());
    MOCK_METHOD(bool, publish, (std::shared_ptr<Message>));
};

#endif    // WOLKABOUTCONNECTOR_CONNECTIVITYSERVICEMOCK_H

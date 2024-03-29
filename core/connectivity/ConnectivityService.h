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

#ifndef CONNECTIVITYSERVICE_H
#define CONNECTIVITYSERVICE_H

#include "core/connectivity/InboundMessageHandler.h"

#include <functional>
#include <memory>
#include <string>
#include <vector>

namespace wolkabout
{
class Message;

class ConnectivityService
{
public:
    virtual ~ConnectivityService() = default;

    virtual bool connect() = 0;
    virtual void disconnect() = 0;
    virtual bool reconnect() = 0;

    virtual bool isConnected() = 0;

    virtual bool publish(std::shared_ptr<Message> outboundMessage) = 0;

    virtual void onConnectionLost(std::function<void()> onConnectionLost);

    virtual void setListner(std::weak_ptr<InboundMessageHandler> inboundMessageHandler);

protected:
    std::function<void()> m_onConnectionLost;
    std::weak_ptr<InboundMessageHandler> m_inboundMessageHandler;
};
}    // namespace wolkabout

#endif

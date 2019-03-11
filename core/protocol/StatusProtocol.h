/*
 * Copyright 2018 WolkAbout Technology s.r.o.
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

#ifndef STATUSPROTOCOL_H
#define STATUSPROTOCOL_H

#include "protocol/Protocol.h"
#include <memory>
#include <string>

namespace wolkabout
{
class DeviceStatusRequest;
class DeviceStatusResponse;
class DeviceStatusUpdate;
class Message;

class StatusProtocol : public Protocol
{
public:
    virtual bool isStatusRequestMessage(const Message& message) const = 0;
    virtual bool isStatusConfirmMessage(const Message& message) const = 0;
    virtual bool isPongMessage(const Message& message) const = 0;

    virtual std::unique_ptr<Message> makeMessage(const std::string& deviceKey,
                                                 const DeviceStatusResponse& response) const = 0;

    virtual std::unique_ptr<Message> makeMessage(const std::string& deviceKey,
                                                 const DeviceStatusUpdate& response) const = 0;

    virtual std::unique_ptr<Message> makeLastWillMessage(const std::string& deviceKey) const = 0;

    virtual std::unique_ptr<Message> makeLastWillMessage(const std::vector<std::string>& deviceKeys) const = 0;

    virtual std::unique_ptr<Message> makeFromPingRequest(const std::string& deviceKey) const = 0;

    inline Type getType() const override final { return Protocol::Type::STATUS; }
};
}    // namespace wolkabout

#endif    // STATUSPROTOCOL_H

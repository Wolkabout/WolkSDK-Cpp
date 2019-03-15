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

#ifndef REGISTRATIONPROTOCOL_H
#define REGISTRATIONPROTOCOL_H

#include "protocol/Protocol.h"

#include <memory>
#include <string>

namespace wolkabout
{
class Message;
class GatewayUpdateRequest;
class GatewayUpdateResponse;
class SubdeviceRegistrationRequest;
class SubdeviceRegistrationResponse;

class RegistrationProtocol : public Protocol
{
public:
    virtual bool isSubdeviceRegistrationResponseMessage(const Message& message) const = 0;
    virtual bool isGatewayUpdateResponseMessage(const Message& message) const = 0;

    virtual std::unique_ptr<Message> makeMessage(const SubdeviceRegistrationRequest& request) const = 0;

    virtual std::unique_ptr<Message> makeMessage(const GatewayUpdateRequest& request) const = 0;

    virtual std::unique_ptr<SubdeviceRegistrationResponse> makeSubdeviceRegistrationResponse(
      const Message& message) const = 0;

    virtual std::unique_ptr<GatewayUpdateResponse> makeGatewayUpdateResponse(const Message& message) const = 0;

    inline Type getType() const override { return Protocol::Type::REGISTRATION; }
};
}    // namespace wolkabout

#endif

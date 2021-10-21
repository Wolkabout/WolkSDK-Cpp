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

#ifndef JSONREGISTRATIONPROTOCOL_H
#define JSONREGISTRATIONPROTOCOL_H

#include "core/protocol/RegistrationProtocol.h"

#include <memory>
#include <string>
#include <vector>

namespace wolkabout
{
class JsonRegistrationProtocol : public RegistrationProtocol
{
public:
    JsonRegistrationProtocol(bool isGateway = true);

    std::vector<std::string> getInboundChannels() const override;
    std::vector<std::string> getInboundChannelsForDevice(const std::string& deviceKey) const override;
    std::string extractDeviceKeyFromChannel(const std::string& topic) const override;

    bool isSubdeviceRegistrationResponse(const Message& message) const override;
    bool isGatewayUpdateResponse(const Message& message) const override;
    bool isSubdeviceDeletionResponse(const Message& message) const override;
    bool isSubdeviceUpdateResponse(const Message& message) const override;

    std::unique_ptr<Message> makeMessage(const std::string& deviceKey,
                                         const SubdeviceRegistrationRequest& request) const override;

    std::unique_ptr<Message> makeMessage(const std::string& deviceKey,
                                         const GatewayUpdateRequest& request) const override;

    std::unique_ptr<Message> makeMessage(const std::string& deviceKey,
                                         const SubdeviceDeletionRequest& request) const override;

    std::unique_ptr<Message> makeMessage(const std::string& deviceKey,
                                         const SubdeviceUpdateRequest& request) const override;

    std::unique_ptr<SubdeviceRegistrationResponse> makeSubdeviceRegistrationResponse(
      const Message& message) const override;

    std::unique_ptr<GatewayUpdateResponse> makeGatewayUpdateResponse(const Message& message) const override;

    std::unique_ptr<SubdeviceUpdateResponse> makeSubdeviceUpdateResponse(const Message& message) const override;

    std::string getResponseChannel(const std::string& deviceKey, const Message& message) const override;

private:
    bool m_isGateway;
    std::string m_devicePrefix;

    static const std::string SUBDEVICE_REGISTRATION_REQUEST_TOPIC_ROOT;
    static const std::string SUBDEVICE_REGISTRATION_RESPONSE_TOPIC_ROOT;
    static const std::string GATEWAY_UPDATE_REQUEST_TOPIC_ROOT;
    static const std::string GATEWAY_UPDATE_RESPONSE_TOPIC_ROOT;
    static const std::string SUBDEVICE_DELETION_REQUEST_TOPIC_ROOT;
    static const std::string SUBDEVICE_DELETION_RESPONSE_TOPIC_ROOT;
    static const std::string SUBDEVICE_UPDATE_REQUEST_TOPIC_ROOT;
    static const std::string SUBDEVICE_UPDATE_RESPONSE_TOPIC_ROOT;
};
}    // namespace wolkabout

#endif    // JSONREGISTRATIONPROTOCOL_H

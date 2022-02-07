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

#ifndef WOLKABOUTCORE_WOLKABOUTREGISTRATIONPROTOCOL_H
#define WOLKABOUTCORE_WOLKABOUTREGISTRATIONPROTOCOL_H

#include "core/protocol/RegistrationProtocol.h"

namespace wolkabout
{
class WolkaboutRegistrationProtocol : public RegistrationProtocol
{
public:
    explicit WolkaboutRegistrationProtocol(bool isGateway = true);

    std::vector<std::string> getInboundChannels() const override;

    std::vector<std::string> getInboundChannelsForDevice(const std::string& deviceKey) const override;

    MessageType getMessageType(const Message& message) override;

    DeviceType getDeviceType(const Message& message) override;

    std::string getDeviceKey(const Message& message) const override;

    std::string getResponseChannelForMessage(MessageType type, const std::string& deviceKey) const override;

    std::unique_ptr<Message> makeOutboundMessage(const std::string& deviceKey,
                                                 const DeviceRegistrationMessage& request) override;

    std::unique_ptr<Message> makeOutboundMessage(const std::string& deviceKey,
                                                 const DeviceRemovalMessage& request) override;

    std::unique_ptr<Message> makeOutboundMessage(const std::string& deviceKey,
                                                 const ChildrenSynchronizationRequestMessage& request) override;

    std::unique_ptr<Message> makeOutboundMessage(const std::string& deviceKey,
                                                 const RegisteredDevicesRequestMessage& request) override;

    std::unique_ptr<ChildrenSynchronizationResponseMessage> parseChildrenSynchronizationResponse(
      const std::shared_ptr<Message>& message) override;

    std::unique_ptr<DeviceRegistrationResponseMessage> parseDeviceRegistrationResponse(
      const std::shared_ptr<Message>& message) override;

    std::unique_ptr<RegisteredDevicesResponseMessage> parseRegisteredDevicesResponse(
      const std::shared_ptr<Message>& message) override;

private:
    std::string getChildrenSynchronizationTopic(const std::string& deviceKey) const;

    std::string getDeviceRegistrationResponseTopic(const std::string& deviceKey) const;

    std::string getRegisteredDevicesTopic(const std::string& deviceKey) const;

    std::string m_outgoingDirection;
    std::string m_incomingDirection;
};
}    // namespace wolkabout

#endif    // WOLKABOUTCORE_WOLKABOUTREGISTRATIONPROTOCOL_H

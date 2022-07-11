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

#ifndef WOLKABOUTCORE_GATEWAYREGISTRATIONPROTOCOL_H
#define WOLKABOUTCORE_GATEWAYREGISTRATIONPROTOCOL_H

#include "core/model/messages/DeviceRegistrationMessage.h"
#include "core/model/messages/DeviceRegistrationResponseMessage.h"
#include "core/model/messages/DeviceRemovalMessage.h"
#include "core/model/messages/RegisteredDevicesRequestMessage.h"
#include "core/model/messages/RegisteredDevicesResponseMessage.h"
#include "core/protocol/Protocol.h"

namespace wolkabout
{
/**
 * This interface is a definition of a protocol that is used to receive registration messages sent out by a subdevice.
 */
class GatewayRegistrationProtocol : public Protocol
{
public:
    /**
     * This method is a serialization method to create a send-able MQTT message from a
     * DeviceRegistrationResponseMessage.
     *
     * @param deviceKey The key of the device sending the response message.
     * @param message The message containing the response data.
     * @return A newly generated MQTT message. A `nullptr` if an error has occurred.
     */
    virtual std::unique_ptr<Message> makeOutboundMessage(const std::string& deviceKey,
                                                         const DeviceRegistrationResponseMessage& message) = 0;

    /**
     * This method is a serialization method to create a send-able MQTT message from a RegisteredDevicesResponseMessage.
     *
     * @param deviceKey The key of the device sending the response message.
     * @param message The message containing the response data.
     * @return A newly generated MQTT message. A `nullptr` if an error has occurred.
     */
    virtual std::unique_ptr<Message> makeOutboundMessage(const std::string& deviceKey,
                                                         const RegisteredDevicesResponseMessage& message) = 0;

    /**
     * This method is a deserialization method used to parse a MQTT message into a DeviceRegistrationMessage.
     *
     * @param message The received MQTT message that is potentially a valid DeviceRegistrationMessage.
     * @return A parsed DeviceRegistrationMessage. A `nullptr` if an error has occurred.
     */
    virtual std::unique_ptr<DeviceRegistrationMessage> parseDeviceRegistrationMessage(
      const std::shared_ptr<Message>& message) = 0;

    /**
     * This method is a deserialization method used to parse a MQTT message into a DeviceRemovalMessage.
     *
     * @param message The received MQTT message that is potentially a valid DeviceRemovalMessage.
     * @return A parsed DeviceRemovalMessage. A `nullptr` if an error has occurred.
     */
    virtual std::unique_ptr<DeviceRemovalMessage> parseDeviceRemovalMessage(
      const std::shared_ptr<Message>& message) = 0;

    /**
     * This method is a deserialization method used to parse a MQTT message into a RegisteredDevicesRequestMessage.
     *
     * @param message The received MQTT message that is potentially a valid RegisteredDevicesRequestMessage.
     * @return A parsed RegisteredDevicesRequestMessage. A `nullptr` if an error has occurred.
     */
    virtual std::unique_ptr<RegisteredDevicesRequestMessage> parseRegisteredDevicesRequestMessage(
      const std::shared_ptr<Message>& message) = 0;
};
}    // namespace wolkabout

#endif    // WOLKABOUTCORE_GATEWAYREGISTRATIONPROTOCOL_H

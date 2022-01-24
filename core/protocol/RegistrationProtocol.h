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

#ifndef WOLKABOUTCORE_REGISTRATIONPROTOCOL_H
#define WOLKABOUTCORE_REGISTRATIONPROTOCOL_H

#include "core/model/messages/DeviceRegistrationMessage.h"
#include "core/model/messages/DeviceRemovalMessage.h"
#include "core/model/messages/RegisteredDevicesRequestMessage.h"
#include "core/model/messages/RegisteredDevicesResponseMessage.h"
#include "core/protocol/Protocol.h"

namespace wolkabout
{
/**
 * This is the interface that defines the protocol for gateway specific messages.
 * This includes messages regarding registering and deleting devices, and obtaining information about devices.
 */
class RegistrationProtocol : public Protocol
{
public:
    /**
     * This method is a serialization method to create a send-able MQTT message from a DeviceRegistrationMessage.
     *
     * @param deviceKey The key of the device sending the request message.
     * @param request The message containing the request information.
     * @return A newly generated MQTT message. A `nullptr` if an error has occurred.
     */
    virtual std::unique_ptr<Message> makeOutboundMessage(const std::string& deviceKey,
                                                         const DeviceRegistrationMessage& request) = 0;

    /**
     * This method is a serialization method to create a send-able MQTT message from a DeviceRemovalMessage.
     *
     * @param deviceKey The key of the device sending the request message.
     * @param request The message containing the request information.
     * @return A newly generated MQTT message. A `nullptr` if an error has occurred.
     */
    virtual std::unique_ptr<Message> makeOutboundMessage(const std::string& deviceKey,
                                                         const DeviceRemovalMessage& request) = 0;

    /**
     * This method is a serialization method to create a send-able MQTT message from a RegisteredDevicesRequestMessage.
     *
     * @param deviceKey The key of the device sending the request message.
     * @param request The message containing the request information.
     * @return A newly generated MQTT message. A `nullptr` if an error has occurred.
     */
    virtual std::unique_ptr<Message> makeOutboundMessage(const std::string& deviceKey,
                                                         const RegisteredDevicesRequestMessage& request) = 0;

    /**
     * This method provides additional information regarding the request message that this protocol can send.
     * This method will return the channel on which the response message will be returned.
     *
     * @param deviceKey The key of the device that will listen to the response channel.
     * @return The response channel on which the message should be listened to.
     */
    virtual std::string getResponseChannelForRegisteredDeviceRequest(const std::string& deviceKey) = 0;

    /**
     * This method is a deserialization method used to parse a MQTT message into a RegisteredDevicesResponse.
     *
     * @param message The received MQTT message that is potentially a valid RegisteredDevicesResponse message.
     * @return A parsed RegisteredDevicesResponse message. A `nullptr` if an error has occurred.
     */
    virtual std::unique_ptr<RegisteredDevicesResponseMessage> parseRegisteredDevicesResponse(
      const std::shared_ptr<Message>& message) = 0;
};
}    // namespace wolkabout

#endif    // WOLKABOUTCORE_REGISTRATIONPROTOCOL_H

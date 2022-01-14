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

#ifndef WOLKABOUTCORE_GATEWAYSUBDEVICEPROTOCOL_H
#define WOLKABOUTCORE_GATEWAYSUBDEVICEPROTOCOL_H

#include "core/model/messages/GatewaySubdeviceMessage.h"
#include "core/protocol/Protocol.h"

namespace wolkabout
{
/**
 * This interface is a definition of a protocol that is used to talk as a gateway on behalf of the devices to the
 * platform.
 */
class GatewaySubdeviceProtocol : public Protocol
{
public:
    /**
     * This method is a serialization method to create a send-able MQTT message from a GatewaySubdeviceMessage.
     *
     * @param deviceKey The device key of the gateway routing this message.
     * @param message The message itself, containing the message received from the subdevice.
     * @return A newly generated MqttMessage. `nullptr` if an error has occurred.
     */
    virtual std::unique_ptr<Message> makeOutboundMessage(const std::string& deviceKey,
                                                         const GatewaySubdeviceMessage& message) = 0;

    /**
     * This method is a deserialization method used to parse a MQTT message into a GatewaySubdeviceMessage.
     *
     * @param message The received MQTT message that is potentially a valid GatewaySubdeviceMessage.
     * @return All parsed GatewaySubdeviceMessages.
     */
    virtual std::vector<GatewaySubdeviceMessage> parseIncomingSubdeviceMessage(std::shared_ptr<Message> message) = 0;
};
}    // namespace wolkabout

#endif    // WOLKABOUTCORE_GATEWAYSUBDEVICEPROTOCOL_H

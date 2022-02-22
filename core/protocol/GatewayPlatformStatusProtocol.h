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

#ifndef WOLKABOUTCORE_GATEWAYPLATFORMSTATUSPROTOCOL_H
#define WOLKABOUTCORE_GATEWAYPLATFORMSTATUSPROTOCOL_H

#include "core/model/messages/PlatformStatusMessage.h"
#include "core/protocol/Protocol.h"

namespace wolkabout
{
/**
 * This interface is a definition of a protocol that should be used by the gateway to publish platform connection status
 * to the subdevices.
 */
class GatewayPlatformStatusProtocol : public Protocol
{
public:
    /**
     * This method is a serialization method to create a send-able MQTT message from a PlatformStatusMessage.
     *
     * @param deviceKey The device key of the gateway publishing this status.
     * @param message The message containing the connection status.
     * @return A newly generated MqttMessage. `nullptr` if an error has occurred.
     */
    virtual std::unique_ptr<Message> makeOutboundMessage(const std::string& deviceKey,
                                                         const PlatformStatusMessage& message) = 0;
};
}    // namespace wolkabout

#endif    // WOLKABOUTCORE_GATEWAYPLATFORMSTATUSPROTOCOL_H

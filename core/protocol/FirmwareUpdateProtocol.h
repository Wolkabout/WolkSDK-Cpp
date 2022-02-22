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

#ifndef WOLKABOUTCORE_FIRMWAREUPDATEPROTOCOL_H
#define WOLKABOUTCORE_FIRMWAREUPDATEPROTOCOL_H

#include "core/model/messages/FirmwareUpdateAbortMessage.h"
#include "core/model/messages/FirmwareUpdateInstallMessage.h"
#include "core/model/messages/FirmwareUpdateStatusMessage.h"
#include "core/protocol/Protocol.h"

namespace wolkabout
{
/**
 * This is an interface defining a FirmwareUpdateProtocol, used to accomplish the FirmwareUpdate functionality of the
 * platform.
 * This protocol defines the serializers/deserializers for a bunch of messages that are necessary to accomplish that
 * functionality.
 */
class FirmwareUpdateProtocol : public Protocol
{
public:
    /**
     * This method is a serialization method to create a send-able MQTT message from a FirmwareUpdateStatusMessage.
     *
     * @param deviceKey The device key for which the FirmwareUpdateStatusMessage is regarding.
     * @param message The message containing information about the FirmwareUpdate process.
     * @return A newly generated MqttMessage. `nullptr` if an error has occurred.
     */
    virtual std::unique_ptr<Message> makeOutboundMessage(const std::string& deviceKey,
                                                         const FirmwareUpdateStatusMessage& message) = 0;

    /**
     * This method is a deserialization method used to parse a MQTT message into a FirmwareUpdateInstallMessage.
     *
     * @param message The received MQTT message that is potentially a valid FirmwareUpdateInstallMessage.
     * @return A parsed FirmwareUpdateInstallMessage. `nullptr` if an error has occurred.
     */
    virtual std::unique_ptr<FirmwareUpdateInstallMessage> parseFirmwareUpdateInstall(
      const std::shared_ptr<Message>& message) = 0;

    /**
     * This method is a deserialization method used to parse a MQTT message into a FirmwareUpdateAbortMessage.
     *
     * @param message The received MQTT message that is potentially a valid FirmwareUpdateAbortMessage.
     * @return A parsed FirmwareUpdateAbortMessage. `nullptr` if an error has occurred.
     */
    virtual std::unique_ptr<FirmwareUpdateAbortMessage> parseFirmwareUpdateAbort(
      const std::shared_ptr<Message>& message) = 0;
};
}    // namespace wolkabout

#endif    // WOLKABOUTCORE_FIRMWAREUPDATEPROTOCOL_H

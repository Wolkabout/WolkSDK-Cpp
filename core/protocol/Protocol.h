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

#ifndef PROTOCOL_H
#define PROTOCOL_H

#include "core/Types.h"
#include "core/model/Message.h"

#include <memory>
#include <vector>

namespace wolkabout
{
/**
 * This is an interface for all protocol interfaces.
 * This defines some basic functionality that is required to use an MQTT connection,
 * such as to define which MQTT topics need to be listened to,
 * and how can a received message be forwarded to an appropriate handler.
 */
class Protocol
{
public:
    /**
     * Default virtual destructor.
     */
    virtual ~Protocol() = default;

    /**
     * This method can be overridden to describe the full list of MQTT topics that need to be subscribed to.
     * This list should not contain any topics that have a specific device key in them (for that, use
     * `getInboundChannelsForDevice`).
     *
     * @return The list of MQTT topics the protocol implementation requires subscription to.
     */
    virtual std::vector<std::string> getInboundChannels() const = 0;

    /**
     * This method can be overridden to describe the full list of MQTT topics that need to be subscribed to.
     * This list should only contain topics that have a specific device key in them, and not any independent of device
     * keys (for that, use `getInboundChannels`).
     *
     * @param deviceKey The device key of a newly initiated device.
     * @return The list of MQTT topics the protocol implementation requires subscription to.
     */
    virtual std::vector<std::string> getInboundChannelsForDevice(const std::string& deviceKey) const = 0;

    /**
     * This method can be overridden to provide information on a response channel that should be listened to for a
     * certain MessageType - if the MessageType has a response channel.
     *
     * @param type The type of the message for which a response channel is requested.
     * @param deviceKey The device key that should be included in the response channel, if the response channel needs
     * it.
     * @return The response channel for the message type. Can be empty if the message type does not have a response
     * channel.
     */
    virtual std::string getResponseChannelForMessage(MessageType type, const std::string& deviceKey) const = 0;

    /**
     * This method can be overridden to define the routine used to figure out the type of the received message.
     * The entire message gets passed through, so both the MQTT topic and the content of the message can be used to
     * determine that.
     *
     * @param message The message whose contents need to be analyzed.
     * @return The type of the message determined by the protocol.
     */
    virtual MessageType getMessageType(const Message& message) = 0;

    /**
     * This method can be overridden to define the routine used to figure out what kind of device communicates with the
     * platform using this message. This is used to disambiguate messages meant for standalone devices, and ones for
     * gateways, or was sent out by a standalone/gateway device.
     *
     * @param message The message whose contents need to be analyzed.
     * @return The type of the device meant to receive/send this message.
     */
    virtual DeviceType getDeviceType(const Message& message) = 0;

    /**
     * This method can be overridden to define the routine used to figure out the target device for the message.
     * Only the topic of the message is passed through as an argument, so the device key must be extracted from the
     * topic.
     *
     * @param message The message whose contents need to be analyzed.
     * @return The device key extracted from the message topic.
     */
    virtual std::string getDeviceKey(const Message& message) const = 0;
};
}    // namespace wolkabout

#endif

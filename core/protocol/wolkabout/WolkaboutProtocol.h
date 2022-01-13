/**
 * Copyright 2021 WolkAbout Technology s.r.o.
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

#ifndef WOLKABOUTCORE_WOLKABOUTPROTOCOL_H
#define WOLKABOUTCORE_WOLKABOUTPROTOCOL_H

#include "core/Types.h"
#include "core/model/Message.h"

#include <memory>

namespace wolkabout
{
/**
 * This static class contains some generic methods that should be shared in between all Wolkabout protocol
 * implementations.
 */
class WolkaboutProtocol
{
public:
    /**
     * This is a helper method that will remove any escaped quotes from a string.
     *
     * @param value Original value.
     * @return Original value but without escaped quotes.
     */
    static std::string removeQuotes(std::string value);

    /**
     * This is a generic Wolkabout implementation for the `getMessageType` interface method of the Protocol interface.
     * This is done by extracting the last part of the topic, which must correspond to a unique MessageType - this is
     * currently set by the Wolkabout protocol.
     *
     * @param message The message whose contents need to be analyzed.
     * @return The determined message type for the received message.
     */
    static MessageType getMessageType(const Message& message);

    /**
     * This is a generic Wolkabout implementation for the `getDeviceType` interface method of the Protocol interface.
     * This is done by extracting the first part of the topic, and checking for characters 'g' or 'd'. If 'g' appears
     * (such as in 'g2p' or 'p2g'), it would mean the Message is meant for a gateway, otherwise it's meant for a
     * standalone device.
     *
     * @param message The message whose contents need to be analyzed.
     * @return The determined DeviceType for the received message.
     */
    static DeviceType getDeviceType(const Message& message);

    /**
     * This is a generic Wolkabout implementation for the `getDeviceKey` interface method of the Protocol
     * interface. This is done by extracting the second part of the topic, which comes after the message direction
     * (usually `p2d`), and is always a device key - this is currently set by the Wolkabout protocol.
     *
     * @param message The message whose contents need to be analyzed.
     * @return The determined device key for the message topic.
     */
    static std::string getDeviceKey(const Message& message);

    // Some constants that are used throughout the code.
    static std::string CHANNEL_DELIMITER;
    static std::string DEVICE_TO_PLATFORM_DIRECTION;
    static std::string PLATFORM_TO_DEVICE_DIRECTION;
    static std::string GATEWAY_TO_PLATFORM_DIRECTION;
    static std::string PLATFORM_TO_GATEWAY_DIRECTION;
    static std::string TIMESTAMP_KEY;
    static std::string ESCAPED_QUOTES;

private:
    WolkaboutProtocol() = default;
    virtual ~WolkaboutProtocol() = 0;
};
}    // namespace wolkabout

#endif    // WOLKABOUTCORE_WOLKABOUTPROTOCOL_H

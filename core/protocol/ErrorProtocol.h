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

#ifndef WOLKABOUTCORE_ERRORPROTOCOL_H
#define WOLKABOUTCORE_ERRORPROTOCOL_H

#include "core/model/messages/ErrorMessage.h"
#include "core/protocol/Protocol.h"

namespace wolkabout
{
/**
 * This is an interface defining an ErrorProtocol, used to receive error messages.
 * This protocol only receives messages.
 */
class ErrorProtocol : public Protocol
{
public:
    /**
     * This method is a deserialization method used to parse a MQTT message into a ErrorMessage.
     *
     * @param message The received MQTT message that is potentially a valid Error message.
     * @return A parsed Error message. A `nullptr` if an error has occurred.
     */
    virtual std::unique_ptr<ErrorMessage> parseError(const std::shared_ptr<Message>& message) = 0;
};
}    // namespace wolkabout

#endif    // WOLKABOUTCORE_ERRORPROTOCOL_H

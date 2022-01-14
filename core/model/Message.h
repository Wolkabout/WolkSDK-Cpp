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

#ifndef MESSAGE_H
#define MESSAGE_H

#include <string>

namespace wolkabout
{
/**
 * This class represents a raw message that is inbound/outbound over MQTT.
 * The message is composed of the topic that the message was sent over or received over,
 * and the message content that was received/sent out.
 */
class Message
{
public:
    /**
     * Default constructor for this message class.
     *
     * @param content The content that was received/sent in this message.
     * @param channel The MQTT topic used to receive/send the message.
     */
    Message(std::string content, std::string channel);

    /**
     * Default virtual destructor.
     */
    virtual ~Message() = default;

    /**
     * Default getter for the content of the message.
     *
     * @return The content of the message as a string.
     */
    const std::string& getContent() const;

    /**
     * Default getter for the MQTT topic of the message.
     *
     * @return The MQTT topic of the message as a string.
     */
    const std::string& getChannel() const;

private:
    std::string m_content;
    std::string m_channel;
};
}    // namespace wolkabout

#endif

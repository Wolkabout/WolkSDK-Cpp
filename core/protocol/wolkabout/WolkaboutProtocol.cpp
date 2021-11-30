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

#include "core/protocol/wolkabout/WolkaboutProtocol.h"

namespace wolkabout
{
const std::string ESCAPED_QUOTES = "\"";

std::string WolkaboutProtocol::removeQuotes(std::string value)
{
    // Make place for the iterator in the string
    auto index = value.find(ESCAPED_QUOTES);
    while (index != std::string::npos)
    {
        value.replace(index, ESCAPED_QUOTES.size(), "");
        index = value.find(ESCAPED_QUOTES);
    }
    return value;
}

MessageType WolkaboutProtocol::getMessageType(const std::shared_ptr<Message>& message)
{
    // Take the topic, and extract its last part
    const auto& topic = message->getChannel();
    const auto section = topic.substr(topic.rfind(CHANNEL_DELIMITER) + 1);
    return messageTypeFromString(section);
}

std::string WolkaboutProtocol::extractDeviceKeyFromChannel(const std::string& topic)
{
    // Substring between the two channel delimiters
    const auto firstDivider = topic.find(CHANNEL_DELIMITER);
    const auto lastDivider = topic.rfind(CHANNEL_DELIMITER);
    return topic.substr(firstDivider + 1, lastDivider - firstDivider - 1);
}
}    // namespace wolkabout

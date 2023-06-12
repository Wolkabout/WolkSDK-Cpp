/**
 * Copyright 2023 Wolkabout Technology s.r.o.
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

#include "core/model/messages/DetailsSynchronizationResponseMessage.h"

#include <utility>

namespace wolkabout
{
DetailsSynchronizationResponseMessage::DetailsSynchronizationResponseMessage(std::vector<std::string> feeds,
                                                                             std::vector<std::string> attributes)
: m_feeds(std::move(feeds)), m_attributes(std::move(attributes))
{
}

const std::vector<std::string>& DetailsSynchronizationResponseMessage::getFeeds() const
{
    return m_feeds;
}

const std::vector<std::string>& DetailsSynchronizationResponseMessage::getAttributes() const
{
    return m_attributes;
}

MessageType DetailsSynchronizationResponseMessage::getMessageType() const
{
    return MessageType::DETAILS_SYNCHRONIZATION_RESPONSE;
}
}    // namespace wolkabout

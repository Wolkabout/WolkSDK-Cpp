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

#include "core/model/messages/FeedRegistrationMessage.h"

namespace wolkabout
{
FeedRegistrationMessage::FeedRegistrationMessage(std::vector<Feed> feeds) : m_feeds(std::move(feeds)) {}

const std::vector<Feed>& FeedRegistrationMessage::getFeeds() const
{
    return m_feeds;
}

MessageType FeedRegistrationMessage::getMessageType() const
{
    return MessageType::FEED_REGISTRATION;
}
}    // namespace wolkabout

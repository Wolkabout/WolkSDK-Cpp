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

#include "core/model/messages/FeedValuesMessage.h"

namespace wolkabout
{
FeedValuesMessage::FeedValuesMessage(const std::vector<Reading>& readings)
{
    for (const auto& reading : readings)
    {
        m_readings[reading.getTimestamp()].emplace_back(reading);
    }
}

MessageType FeedValuesMessage::getMessageType() const
{
    return MessageType::FEED_VALUES;
}

const std::map<std::uint64_t, std::vector<Reading>>& FeedValuesMessage::getReadings() const
{
    return m_readings;
}
}    // namespace wolkabout

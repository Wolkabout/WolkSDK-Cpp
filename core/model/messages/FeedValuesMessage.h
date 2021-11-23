/**
 * Copyright 2021 Wolkabout s.r.o.
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

#ifndef WOLKABOUTCORE_FEEDVALUESMESSAGE_H
#define WOLKABOUTCORE_FEEDVALUESMESSAGE_H

#include "core/model/Reading.h"
#include "core/model/messages/MessageObject.h"

#include <map>
#include <memory>
#include <vector>

namespace wolkabout
{
/**
 * This class represents a both d2p and p2d message by which feed data is transmitted.
 */
class FeedValuesMessage : public MessageObject
{
public:
    /**
     * Default constructor for this MessageObject class that intakes all readings and sorts them out by timestamp into
     * the internal map.
     *
     * @param readings All the readings that are part of this message.
     */
    explicit FeedValuesMessage(const std::vector<Reading>& readings);

    /**
     * Default constructor for this MessageObject class that intakes all reading pointers and sorts them out by
     * timestamp into the internal map.
     *
     * @param readings All the reading pointers that are part of this message.
     */
    explicit FeedValuesMessage(const std::vector<std::shared_ptr<Reading>>& readings);

    /**
     * Overridden method from the `MessageObject` interface.
     * This method indicates the message type through the virtual method.
     *
     * @return The message type for this class - always `FEED_VALUES`.
     */
    MessageType getMessageType() override;

    /**
     * Default getter for the map of readings this message contains.
     * The key of this map are timestamp, by which the readings are grouped up.
     *
     * @return The map containing readings, with timestamps as keys.
     */
    const std::map<std::uint64_t, std::vector<Reading>>& getReadings() const;

private:
    std::map<std::uint64_t, std::vector<Reading>> m_readings;
};
}    // namespace wolkabout

#endif    // WOLKABOUTCORE_FEEDVALUESMESSAGE_H

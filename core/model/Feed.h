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

#ifndef WOLKABOUTCORE_FEED_H
#define WOLKABOUTCORE_FEED_H

#include "core/Types.h"

namespace wolkabout
{
/**
 * This class represents a feed model that should be used to register feeds for devices.
 * This intakes all the necessary information, such as the name under which it will be presented, reference by which it
 * will be linked with data, the FeedType defining who can send the data, and unit which gives the feed a reading type.
 */
class Feed
{
public:
    /**
     * Default constructor for this model class.
     *
     * @param name The name of the feed on the platform.
     * @param reference The reference for data.
     * @param feedType The type defining the ways data can be directed.
     * @param unit The unit defining the reading type.
     */
    Feed(std::string name, std::string reference, FeedType feedType, std::string unit);

    /**
     * Default getter for the name of the feed.
     *
     * @return The name of the feed as a string.
     */
    const std::string& getName() const;

    /**
     * Default getter for the reference of the feed.
     *
     * @return The reference of the feed as a string.
     */
    const std::string& getReference() const;

    /**
     * Default getter for the type of the feed.
     *
     * @return The feed type of the feed as an enum value.
     */
    FeedType getFeedType() const;

    /**
     * Default getter for the unit of the feed.
     *
     * @return The unit type of the feed as a string.
     */
    std::string getUnit() const;

private:
    std::string m_name;
    std::string m_reference;
    FeedType m_feedType;
    std::string m_unit;
};
}    // namespace wolkabout

#endif    // WOLKABOUTCORE_FEED_H

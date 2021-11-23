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

#ifndef WOLKABOUTCORE_FEED_H
#define WOLKABOUTCORE_FEED_H

#include "core/Types.h"

namespace wolkabout
{
class Feed
{
public:
    Feed(std::string name, std::string reference, FeedType feedType, Unit unit);

    const std::string& getName() const;

    const std::string& getReference() const;

    FeedType getFeedType() const;

    Unit getUnit() const;

private:
    std::string m_name;
    std::string m_reference;
    FeedType m_feedType;
    Unit m_unit;
};
}    // namespace wolkabout

#endif    // WOLKABOUTCORE_FEED_H

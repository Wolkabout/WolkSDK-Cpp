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

#include "core/model/Feed.h"

#include <stdexcept>
#include <utility>

namespace wolkabout
{
Feed::Feed(std::string name, std::string reference, FeedType feedType, std::string unit)
: m_name(std::move(name)), m_reference(std::move(reference)), m_feedType(feedType), m_unit(std::move(unit))
{
    if (m_feedType < FeedType::IN || m_feedType > FeedType::IN_OUT)
        throw std::runtime_error("Failed to created Feed with an invalid FeedType value.");
}

const std::string& Feed::getName() const
{
    return m_name;
}

const std::string& Feed::getReference() const
{
    return m_reference;
}

FeedType Feed::getFeedType() const
{
    return m_feedType;
}

std::string Feed::getUnit() const
{
    return m_unit;
}
}    // namespace wolkabout

/*
 * Copyright 2018 WolkAbout Technology s.r.o.
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

#include "Reading.h"

#include "core/utilities/StringUtils.h"

namespace wolkabout
{
Reading::Reading(std::vector<std::string> values, std::string reference, unsigned long long int rtc,
                 std::string deviceKey)
: m_values(std::move(values)), m_reference(std::move(reference)), m_rtc(rtc), m_key{std::move(deviceKey)}
{
}

const std::vector<std::string>& Reading::getValues() const
{
    return m_values;
}

const std::string& Reading::getValue() const
{
    if (m_values.empty())
    {
        return StringUtils::EMPTY_STRING;
    }

    return m_values.at(0);
}

const std::string& Reading::getReference() const
{
    return m_reference;
}

unsigned long long Reading::getRtc() const
{
    return m_rtc;
}

const std::string& Reading::getKey() const
{
    return m_key;
}
}    // namespace wolkabout

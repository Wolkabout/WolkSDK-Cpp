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

#include "core/persistence/inmemory/InMemoryPersistence.h"

#include <algorithm>
#include <string>
#include <utility>
#include <vector>

namespace wolkabout
{
bool InMemoryPersistence::putReading(const std::string& key, const Reading& reading)
{
    getOrCreateReadingsByKey(key).push_back(std::make_shared<Reading>(reading));
    return true;
}

std::vector<std::shared_ptr<Reading>> InMemoryPersistence::getReadings(const std::string& key, std::uint_fast64_t count)
{
    if (m_readings.find(key) == m_readings.end())
    {
        return {};
    }

    std::vector<std::shared_ptr<Reading>>& readings = m_readings.at(key);
    auto size = static_cast<uint_fast64_t>(readings.size());
    return {readings.begin(), readings.begin() + static_cast<std::int64_t>((count < size ? count : size))};
}

void InMemoryPersistence::removeReadings(const std::string& key, uint_fast64_t count)
{
    if (m_readings.find(key) == m_readings.end())
    {
        return;
    }

    std::vector<std::shared_ptr<Reading>>& readings = getOrCreateReadingsByKey(key);
    auto size = static_cast<uint_fast64_t>(readings.size());
    readings.erase(readings.begin(), readings.begin() + static_cast<std::int64_t>((count < size ? count : size)));
    readings.shrink_to_fit();
}

std::vector<std::string> InMemoryPersistence::getReadingsKeys()
{
    std::vector<std::string> keys;
    for (const auto& pair : m_readings)
    {
        if (!pair.second.empty())
        {
            keys.push_back(pair.first);
        }
    }

    return keys;
}

bool InMemoryPersistence::putAttribute(const std::string& key, std::shared_ptr<Attribute> attribute)
{
    m_attributes.emplace(key, std::move(attribute));
    return true;
}

std::map<std::string, std::shared_ptr<Attribute>> InMemoryPersistence::getAttributes()
{
    return m_attributes;
}

std::shared_ptr<Attribute> InMemoryPersistence::getAttributeUnderKey(const std::string& key)
{
    const auto it = m_attributes.find(key);
    if (it == m_attributes.cend())
        return {};
    return it->second;
}

void InMemoryPersistence::removeAttributes()
{
    m_attributes.clear();
}

void InMemoryPersistence::removeAttributes(const std::string& key)
{
    const auto it = m_attributes.find(key);
    if (it != m_attributes.cend())
        m_attributes.erase(it);
}

std::vector<std::string> InMemoryPersistence::getAttributeKeys()
{
    auto keys = std::vector<std::string>{};
    std::transform(m_attributes.cbegin(), m_attributes.cend(), keys.end(),
                   [&](const std::pair<std::string, std::shared_ptr<Attribute>>& pair) { return pair.first; });
    return keys;
}

bool InMemoryPersistence::putParameter(const std::string& key, Parameter parameter)
{
    m_parameters.emplace(key, std::move(parameter));
    return true;
}

std::map<std::string, Parameter> InMemoryPersistence::getParameters()
{
    return m_parameters;
}

Parameter InMemoryPersistence::getParameterForKey(const std::string& key)
{
    const auto it = m_parameters.find(key);
    if (it == m_parameters.cend())
        return {};
    return it->second;
}

void InMemoryPersistence::removeParameters()
{
    m_parameters.clear();
}

void InMemoryPersistence::removeParameters(const std::string& key)
{
    const auto it = m_parameters.find(key);
    if (it != m_parameters.cend())
        m_parameters.erase(it);
}

std::vector<std::string> InMemoryPersistence::getParameterKeys()
{
    auto keys = std::vector<std::string>{};
    std::transform(m_parameters.cbegin(), m_parameters.cend(), keys.end(),
                   [&](const std::pair<std::string, Parameter>& pair) { return pair.first; });
    return keys;
}

std::vector<std::shared_ptr<Reading>>& InMemoryPersistence::getOrCreateReadingsByKey(const std::string& key)
{
    if (m_readings.find(key) == m_readings.end())
    {
        m_readings.emplace(std::pair<std::string, std::vector<std::shared_ptr<Reading>>>(key, {}));
    }

    return m_readings.at(key);
}

bool InMemoryPersistence::isEmpty()
{
    return m_readings.empty() && m_attributes.empty() && m_parameters.empty();
}
}    // namespace wolkabout

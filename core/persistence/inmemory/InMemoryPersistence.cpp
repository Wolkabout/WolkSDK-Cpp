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

#include "InMemoryPersistence.h"

#include <cstddef>
#include <mutex>
#include <string>
#include <utility>
#include <vector>

namespace wolkabout
{
bool InMemoryPersistence::putReading(const std::string& key, std::shared_ptr<Reading> reading)
{
   getOrCreateReadingsByKey(key).push_back(reading);
   return true;
}

std::vector<std::shared_ptr<Reading>> InMemoryPersistence::getReadings(const std::string& key,
                                                                                  uint_fast64_t count)
{
   if (m_readings.find(key) == m_readings.end())
   {
       return {};
   }

   std::vector<std::shared_ptr<Reading>>& readings = m_readings.at(key);
   auto size = static_cast<uint_fast64_t>(readings.size());
   return std::vector<std::shared_ptr<Reading>>(readings.begin(),
                                                      readings.begin() + (count < size ? count : size));
}

void InMemoryPersistence::removeReadings(const std::string& key, uint_fast64_t count)
{
   if (m_readings.find(key) == m_readings.end())
   {
       return;
   }

   std::vector<std::shared_ptr<Reading>>& readings = getOrCreateReadingsByKey(key);
   auto size = static_cast<uint_fast64_t>(readings.size());
   readings.erase(readings.begin(), readings.begin() + (count < size ? count : size));
   readings.shrink_to_fit();
}

std::vector<std::string> InMemoryPersistence::getReadingsKeys()
{
   std::vector<std::string> keys;
   for (const std::pair<std::string, std::vector<std::shared_ptr<Reading>>>& pair : m_readings)
   {
       if (!pair.second.empty())
       {
           keys.push_back(pair.first);
       }
   }

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

bool InMemoryPersistence::putAttribute(std::shared_ptr<Attribute> attribute)
{
    m_attributes.push_back(attribute);

    return true;
}

std::vector<std::shared_ptr<Attribute>> InMemoryPersistence::getAttributes()
{
    return m_attributes;
}

void InMemoryPersistence::removeAttributes()
{
    m_attributes.clear();
}

bool InMemoryPersistence::putParameter(Parameters parameter)
{
    m_parameters.insert(parameter);
    return true;
}

std::map<ParameterName, std::string> InMemoryPersistence::getParameters()
{
    return m_parameters;
}

void InMemoryPersistence::removeParameters(const ParameterName parameterName)
{
    m_parameters.clear();
}

bool InMemoryPersistence::isEmpty()
{
    return getReadingsKeys().empty() && m_attributes.empty() && m_parameters.empty();
}

}    // namespace wolkabout
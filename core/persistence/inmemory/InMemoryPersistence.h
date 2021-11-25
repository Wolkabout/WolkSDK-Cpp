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

#ifndef INMEMORYPERSISTENCE_H
#define INMEMORYPERSISTENCE_H

#include "core/persistence/Persistence.h"

#include <cstdint>
#include <map>
#include <mutex>
#include <string>
#include <vector>

namespace wolkabout
{
class Reading;

class InMemoryPersistence : public Persistence
{
public:
    ~InMemoryPersistence() override = default;

    bool putReading(const std::string& key, std::shared_ptr<Reading> reading) override;

    std::vector<std::shared_ptr<Reading>> getReadings(const std::string& key, std::uint_fast64_t count) override;

    void removeReadings(const std::string& key, std::uint_fast64_t count) override;

    std::vector<std::string> getReadingsKeys() override;

    bool isEmpty() override;

    bool putAttribute(std::shared_ptr<Attribute> attribute) override;

    std::vector<std::shared_ptr<Attribute>> getAttributes() override;

    void removeAttributes() override;

    bool putParameter(Parameter parameter) override;

    std::map<ParameterName, std::string> getParameters() override;

    void removeParameters(ParameterName parameterName) override;

private:
    std::vector<std::shared_ptr<Reading>>& getOrCreateReadingsByKey(const std::string& key);

    std::map<std::string, std::vector<std::shared_ptr<Reading>>> m_readings;

    std::vector<std::shared_ptr<Attribute>> m_attributes;

    std::map<ParameterName, std::string> m_parameters;
};
}    // namespace wolkabout

#endif

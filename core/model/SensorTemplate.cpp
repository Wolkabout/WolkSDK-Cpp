/*
 * Copyright 2019 WolkAbout Technology s.r.o.
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

#include "model/SensorTemplate.h"

#include <utility>

namespace wolkabout
{
SensorTemplate::SensorTemplate(std::string name, std::string reference, DataType dataType, std::string description)
: m_name{std::move(name)}
, m_reference{std::move(reference)}
, m_readingType{dataType}
, m_description{std::move(description)}
{
}

SensorTemplate::SensorTemplate(std::string name, std::string reference, ReadingType readingType,
                               std::string description)
: m_name{std::move(name)}
, m_reference{std::move(reference)}
, m_readingType{std::move(readingType)}
, m_description{std::move(description)}
{
}

SensorTemplate::SensorTemplate(std::string name, std::string reference, ReadingType::Name readingTypeName,
                               ReadingType::MeasurmentUnit unit, std::string description)

: m_name{std::move(name)}
, m_reference{std::move(reference)}
, m_readingType{ReadingType{readingTypeName, unit}}
, m_description{std::move(description)}
{
}

SensorTemplate::SensorTemplate(std::string name, std::string reference, std::string readingTypeName,
                               std::string unitSymbol, std::string description)
: m_name{std::move(name)}
, m_reference{std::move(reference)}
, m_readingType{ReadingType{std::move(readingTypeName), std::move(unitSymbol)}}
, m_description{std::move(description)}
{
}

const std::string& SensorTemplate::getName() const
{
    return m_name;
}

const std::string& SensorTemplate::getReference() const
{
    return m_reference;
}

const std::string& SensorTemplate::getUnitSymbol() const
{
    return m_readingType.getMeasurmentUnitSymbol();
}

const std::string& SensorTemplate::getDescription() const
{
    return m_description;
}

const std::string& SensorTemplate::getReadingTypeName() const
{
    return m_readingType.getName();
}

bool SensorTemplate::operator==(SensorTemplate& rhs) const
{
    if (m_name != rhs.m_name || m_reference != rhs.m_reference || m_description != rhs.m_description)
    {
        return false;
    }

    if (m_readingType != rhs.m_readingType)
    {
        return false;
    }

    return true;
}

bool SensorTemplate::operator!=(SensorTemplate& rhs) const
{
    return !(*this == rhs);
}
}    // namespace wolkabout

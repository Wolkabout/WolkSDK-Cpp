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

#include "ActuatorTemplate.h"

#include <utility>

namespace wolkabout
{
ActuatorTemplate::ActuatorTemplate(std::string name, std::string reference, DataType dataType, std::string description)
: m_name{std::move(name)}
, m_reference{std::move(reference)}
, m_readingType{std::move(dataType)}
, m_description{std::move(description)}
{
}

ActuatorTemplate::ActuatorTemplate(std::string name, std::string reference, ActuationReadingType readingType,
                                   std::string description)
: m_name{std::move(name)}
, m_reference{std::move(reference)}
, m_readingType{std::move(readingType)}
, m_description{std::move(description)}
{
}

ActuatorTemplate::ActuatorTemplate(std::string name, std::string reference, ActuationReadingType::Name readingTypeName,
                                   ActuationReadingType::MeasurmentUnit unit, std::string description)

: m_name{std::move(name)}
, m_reference{std::move(reference)}
, m_readingType{ActuationReadingType(readingTypeName, unit)}
, m_description{std::move(description)}
{
}

ActuatorTemplate::ActuatorTemplate(std::string name, std::string reference, std::string readingTypeName,
                                   std::string unitSymbol, std::string description)
: m_name{std::move(name)}
, m_reference{std::move(reference)}
, m_readingType{ActuationReadingType(std::move(readingTypeName), std::move(unitSymbol))}
, m_description{std::move(description)}
{
}

const std::string& ActuatorTemplate::getName() const
{
    return m_name;
}

const std::string& ActuatorTemplate::getReference() const
{
    return m_reference;
}

const std::string& ActuatorTemplate::getDescription() const
{
    return m_description;
}

const std::string& ActuatorTemplate::getUnitSymbol() const
{
    return m_readingType.getMeasurmentUnitSymbol();
}

const std::string& ActuatorTemplate::getReadingTypeName() const
{
    return m_readingType.getName();
}

bool ActuatorTemplate::operator==(const ActuatorTemplate& rhs) const
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

bool ActuatorTemplate::operator!=(const ActuatorTemplate& rhs) const
{
    return !(*this == rhs);
}
}    // namespace wolkabout

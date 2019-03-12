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

#include "model/SensorTemplate.h"

namespace wolkabout
{
SensorTemplate::SensorTemplate(std::string name, std::string reference, DataType dataType, std::string description,
                               double minimum, double maximum)
: m_name{std::move(name)}
, m_reference{std::move(reference)}
, m_readingType{std::move(dataType)}
, m_description{std::move(description)}
, m_minimum{minimum}
, m_maximum{maximum}
{
}

SensorTemplate::SensorTemplate(std::string name, std::string reference, ReadingType readingType,
                               std::string description, double minimum, double maximum)
: m_name{std::move(name)}
, m_reference{std::move(reference)}
, m_readingType{std::move(readingType)}
, m_description{std::move(description)}
, m_minimum{minimum}
, m_maximum{maximum}
{
}

SensorTemplate::SensorTemplate(std::string name, std::string reference, ReadingType::Name readingTypeName,
                               ReadingType::MeasurmentUnit unit, std::string description, double minimum,
                               double maximum)

: m_name{std::move(name)}
, m_reference{std::move(reference)}
, m_readingType{ReadingType(std::move(readingTypeName), std::move(unit))}
, m_description{std::move(description)}
, m_minimum{minimum}
, m_maximum{maximum}
{
}

SensorTemplate::SensorTemplate(std::string name, std::string reference, std::string readingTypeName,
                               std::string unitSymbol, std::string description, double minimum, double maximum)
: m_name{std::move(name)}
, m_reference{std::move(reference)}
, m_readingType{ReadingType(std::move(readingTypeName), std::move(unitSymbol))}
, m_description{std::move(description)}
, m_minimum(minimum)
, m_maximum(maximum)
{
}

SensorTemplate::SensorTemplate(std::string name, std::string reference, std::string readingTypeName,
                               std::string unitSymbol, DataType dataType, std::string description, double minimum,
                               double maximum)
: m_name{std::move(name)}
, m_reference{std::move(reference)}
, m_readingType{ReadingType(std::move(readingTypeName), std::move(unitSymbol), std::move(dataType))}
, m_description{std::move(description)}
, m_minimum(minimum)
, m_maximum(maximum)
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

DataType SensorTemplate::getDataType() const
{
    return m_readingType.getDataType();
}

double SensorTemplate::getMinimum() const
{
    return m_minimum;
}

double SensorTemplate::getMaximum() const
{
    return m_maximum;
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

    if (m_minimum != rhs.m_minimum || m_maximum != rhs.m_maximum)
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

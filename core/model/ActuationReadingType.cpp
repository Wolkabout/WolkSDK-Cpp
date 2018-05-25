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

#include "model/ActuationReadingType.h"

#include <stdexcept>

namespace wolkabout
{
const std::string ActuationReadingType::DEFAULT_DELIMITER = ",";

ActuationReadingType::ActuationReadingType(DataType dataType)
: m_precision{1}, m_labels{}, m_size{1}, m_delimiter{DEFAULT_DELIMITER}
{
    switch (dataType)
    {
    case DataType::NUMERIC:
    {
        m_name = toString(ActuationReadingType::Name::COUNT);
        m_unit = toString(ActuationReadingType::MeasurmentUnit::COUNT);
        m_dataType = DataType::NUMERIC;
        m_unitSymbol = symbolForUnit(ActuationReadingType::MeasurmentUnit::COUNT);

        break;
    }
    case DataType::BOOLEAN:
    {
        m_name = toString(ActuationReadingType::Name::SWITCH);
        m_unit = toString(ActuationReadingType::MeasurmentUnit::BOOLEAN);
        m_dataType = DataType::BOOLEAN;
        m_unitSymbol = symbolForUnit(ActuationReadingType::MeasurmentUnit::BOOLEAN);

        break;
    }
    case DataType::STRING:
    {
        m_name = toString(ActuationReadingType::Name::STRING);
        m_unit = toString(ActuationReadingType::MeasurmentUnit::TEXT);
        m_dataType = DataType::STRING;
        m_unitSymbol = symbolForUnit(ActuationReadingType::MeasurmentUnit::TEXT);

        break;
    }
    default:
    {
        throw std::logic_error(std::string("Undefined Data type: ") + std::to_string(static_cast<int>(dataType)));
    }
    }
}

ActuationReadingType::ActuationReadingType(ActuationReadingType::Name name, ActuationReadingType::MeasurmentUnit unit)
: m_precision{1}, m_labels{}, m_size{1}, m_delimiter{DEFAULT_DELIMITER}
{
    validate(name, unit);

    m_name = toString(name);
    m_unit = toString(unit);

    m_dataType = dataTypeForName(name);
    m_unitSymbol = symbolForUnit(unit);
}

ActuationReadingType::ActuationReadingType(std::string name, std::string unitSymbol, DataType dataType, int precision,
                                           std::vector<std::string> labels)
: m_name{std::move(name)}
, m_unitSymbol{std::move(unitSymbol)}
, m_dataType{std::move(dataType)}
, m_precision{precision}
, m_labels{std::move(labels)}
, m_size{m_labels.size() == 0 ? 1 : m_labels.size()}
, m_delimiter{DEFAULT_DELIMITER}
{
}

const std::string& ActuationReadingType::getName() const
{
    return m_name;
}

const std::string& ActuationReadingType::getMeasurmentUnit() const
{
    return m_unit;
}

const std::string& ActuationReadingType::getMeasurmentUnitSymbol() const
{
    return m_unitSymbol;
}

DataType ActuationReadingType::getDataType() const
{
    return m_dataType;
}

int ActuationReadingType::getPrecision() const
{
    return m_precision;
}

const std::vector<std::string>& ActuationReadingType::getLabels() const
{
    return m_labels;
}

size_t ActuationReadingType::getSize() const
{
    return m_size;
}

const std::string& ActuationReadingType::getDelimiter() const
{
    return m_delimiter;
}

bool ActuationReadingType::operator==(ActuationReadingType& rhs) const
{
    if (m_name != rhs.m_name || m_unit != rhs.m_unit || m_unitSymbol != rhs.m_unitSymbol ||
        m_dataType != rhs.m_dataType)
    {
        return false;
    }

    if (m_delimiter != rhs.m_delimiter)
    {
        return false;
    }

    if (m_labels.size() != rhs.m_labels.size())
    {
        return false;
    }

    for (unsigned long long int i = 0; i < m_labels.size(); ++i)
    {
        if (m_labels[i] != rhs.m_labels[i])
        {
            return false;
        }
    }

    return true;
}

bool ActuationReadingType::operator!=(ActuationReadingType& rhs) const
{
    return !(*this == rhs);
}

void ActuationReadingType::validate(ActuationReadingType::Name name, ActuationReadingType::MeasurmentUnit unit) const
{
    switch (name)
    {
    case ActuationReadingType::Name::COUNT:
    {
        switch (unit)
        {
        case MeasurmentUnit::COUNT:
        case MeasurmentUnit::STEPS:
        {
            break;
        }
        default:
        {
            throw std::logic_error(std::string("Invalid Measurment unit specified for ") + toString(name) + ": " +
                                   toString(unit));
        }
        }

        break;
    }
    case ActuationReadingType::Name::SWITCH:
    {
        switch (unit)
        {
        case MeasurmentUnit::BOOLEAN:
        {
            break;
        }
        default:
        {
            throw std::logic_error(std::string("Invalid Measurment unit specified for ") + toString(name) + ": " +
                                   toString(unit));
        }
        }

        break;
    }
    case ActuationReadingType::Name::STRING:
    {
        switch (unit)
        {
        case MeasurmentUnit::TEXT:
        {
            break;
        }
        default:
        {
            throw std::logic_error(std::string("Invalid Measurment unit specified for ") + toString(name) + ": " +
                                   toString(unit));
        }
        }

        break;
    }
    default:
    {
        throw std::logic_error(std::string("Undefined Actuation measurment unit: ") +
                               std::to_string(static_cast<int>(unit)));
    }
    }
}

DataType ActuationReadingType::dataTypeForName(ActuationReadingType::Name name) const
{
    switch (name)
    {
    case ActuationReadingType::Name::COUNT:
    {
        return DataType::NUMERIC;
    }
    case ActuationReadingType::Name::SWITCH:
    {
        return DataType::BOOLEAN;
    }
    case ActuationReadingType::Name::STRING:
    {
        return DataType::STRING;
    }
    default:
    {
        throw std::logic_error(std::string("Undefined Actuation reading type name: ") +
                               std::to_string(static_cast<int>(name)));
    }
    }
}

std::string ActuationReadingType::symbolForUnit(ActuationReadingType::MeasurmentUnit unit) const
{
    switch (unit)
    {
    case ActuationReadingType::MeasurmentUnit::COUNT:
    {
        return "count";
    }
    case ActuationReadingType::MeasurmentUnit::STEPS:
    {
        return "#";
    }
    case ActuationReadingType::MeasurmentUnit::BOOLEAN:
    {
        return "";
    }
    case ActuationReadingType::MeasurmentUnit::TEXT:
    {
        return "";
    }
    default:
    {
        throw std::logic_error(std::string("Undefined Actuation measurment unit: ") +
                               std::to_string(static_cast<int>(unit)));
    }
    }
}

std::string toString(ActuationReadingType::Name name)
{
    switch (name)
    {
    case ActuationReadingType::Name::COUNT:
    {
        return "COUNT(ACTUATOR)";
    }
    case ActuationReadingType::Name::SWITCH:
    {
        return "SWITCH(ACTUATOR)";
    }
    case ActuationReadingType::Name::STRING:
    {
        return "STRING(ACTUATOR)";
    }
    default:
    {
        throw std::logic_error(std::string("Undefined Actuation reading type name: ") +
                               std::to_string(static_cast<int>(name)));
    }
    }
}

std::string toString(ActuationReadingType::MeasurmentUnit unit)
{
    switch (unit)
    {
    case ActuationReadingType::MeasurmentUnit::COUNT:
    {
        return "COUNT(ACTUATOR)";
    }
    case ActuationReadingType::MeasurmentUnit::STEPS:
    {
        return "STEPS";
    }
    case ActuationReadingType::MeasurmentUnit::BOOLEAN:
    {
        return "BOOLEAN(ACTUATOR)";
    }
    case ActuationReadingType::MeasurmentUnit::TEXT:
    {
        return "TEXT(ACTUATOR)";
    }
    default:
    {
        throw std::logic_error(std::string("Undefined Actuation measurment unit: ") +
                               std::to_string(static_cast<int>(unit)));
    }
    }
}
}

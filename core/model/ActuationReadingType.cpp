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

#include "model/ActuationReadingType.h"

#include <stdexcept>
#include <utility>

namespace wolkabout
{
ActuationReadingType::ActuationReadingType(DataType dataType)
{
    switch (dataType)
    {
    case DataType::NUMERIC:
    {
        m_name = toString(ActuationReadingType::Name::COUNT);
        m_unit = toString(ActuationReadingType::MeasurmentUnit::COUNT);
        m_unitSymbol = symbolForUnit(ActuationReadingType::MeasurmentUnit::COUNT);

        break;
    }
    case DataType::BOOLEAN:
    {
        m_name = toString(ActuationReadingType::Name::SWITCH);
        m_unit = toString(ActuationReadingType::MeasurmentUnit::BOOLEAN);
        m_unitSymbol = symbolForUnit(ActuationReadingType::MeasurmentUnit::BOOLEAN);

        break;
    }
    case DataType::STRING:
    {
        m_name = toString(ActuationReadingType::Name::STRING);
        m_unit = toString(ActuationReadingType::MeasurmentUnit::TEXT);
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
{
    validate(name, unit);

    m_name = toString(name);
    m_unit = toString(unit);

    m_unitSymbol = symbolForUnit(unit);
}

ActuationReadingType::ActuationReadingType(std::string name, std::string unitSymbol)
: m_name{std::move(name)}, m_unitSymbol{std::move(unitSymbol)}
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

bool ActuationReadingType::operator==(ActuationReadingType& rhs) const
{
    if (m_name != rhs.m_name || m_unitSymbol != rhs.m_unitSymbol)
    {
        return false;
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
        return "RANGE (ACTUATOR)";
    }
    case ActuationReadingType::Name::SWITCH:
    {
        return "SWITCH(ACTUATOR)";
    }
    case ActuationReadingType::Name::STRING:
    {
        return "TEXT(ACTUATOR)";
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
        throw std::logic_error(std::string("Undefined Actuation measurement unit: ") +
                               std::to_string(static_cast<int>(unit)));
    }
    }
}
}    // namespace wolkabout

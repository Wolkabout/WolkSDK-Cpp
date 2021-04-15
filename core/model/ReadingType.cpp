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

#include "ReadingType.h"

#include <stdexcept>
#include <utility>

namespace wolkabout
{
ReadingType::ReadingType(DataType dataType)
{
    switch (dataType)
    {
    case DataType::NUMERIC:
    {
        m_name = toString(ReadingType::Name::GENERIC);
        m_unit = toString(ReadingType::MeasurmentUnit::NUMERIC);
        m_unitSymbol = symbolForUnit(ReadingType::MeasurmentUnit::NUMERIC);

        break;
    }
    case DataType::BOOLEAN:
    {
        m_name = toString(ReadingType::Name::GENERIC_BOOLEAN);
        m_unit = toString(ReadingType::MeasurmentUnit::BOOLEAN);
        m_unitSymbol = symbolForUnit(ReadingType::MeasurmentUnit::BOOLEAN);

        break;
    }
    case DataType::STRING:
    {
        m_name = toString(ReadingType::Name::GENERIC_TEXT);
        m_unit = toString(ReadingType::MeasurmentUnit::TEXT);
        m_unitSymbol = symbolForUnit(ReadingType::MeasurmentUnit::TEXT);

        break;
    }
    default:
    {
        throw std::logic_error(std::string("Undefined Data type: ") + std::to_string(static_cast<int>(dataType)));
    }
    }
}

ReadingType::ReadingType(ReadingType::Name name, ReadingType::MeasurmentUnit unit)
{
    validate(name, unit);

    m_name = toString(name);
    m_unit = toString(unit);

    m_unitSymbol = symbolForUnit(unit);
}

ReadingType::ReadingType(std::string name, std::string unitSymbol)
: m_name{std::move(name)}, m_unitSymbol{std::move(unitSymbol)}
{
}

const std::string& ReadingType::getName() const
{
    return m_name;
}

const std::string& ReadingType::getMeasurmentUnit() const
{
    return m_unit;
}

const std::string& ReadingType::getMeasurmentUnitSymbol() const
{
    return m_unitSymbol;
}

bool ReadingType::operator==(const ReadingType& rhs) const
{
    if (m_name != rhs.m_name || m_unitSymbol != rhs.m_unitSymbol)
    {
        return false;
    }

    return true;
}

bool ReadingType::operator!=(const ReadingType& rhs) const
{
    return !(*this == rhs);
}

void ReadingType::validate(ReadingType::Name name, ReadingType::MeasurmentUnit unit)
{
    switch (name)
    {
    case ReadingType::Name::GENERIC:
    {
        switch (unit)
        {
        case MeasurmentUnit::BIT:
        case MeasurmentUnit::PERCENT:
        case MeasurmentUnit::CO2_MOL:
        case MeasurmentUnit::X10C:
        case MeasurmentUnit::X100V:
        case MeasurmentUnit::X10PA:
        case MeasurmentUnit::NUMERIC:
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
    case ReadingType::Name::TEMPERATURE:
    {
        switch (unit)
        {
        case MeasurmentUnit::KELVIN:
        case MeasurmentUnit::CELSIUS:
        case MeasurmentUnit::FAHRENHEIT:
        case MeasurmentUnit::CELSIUS_X2:
        case MeasurmentUnit::CELSIUS_X10:
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
    case ReadingType::Name::PRESSURE:
    {
        switch (unit)
        {
        case MeasurmentUnit::PASCAL:
        case MeasurmentUnit::MILLIMETER_OF_MERCURY:
        case MeasurmentUnit::INCH_OF_MERCURY:
        case MeasurmentUnit::BAR:
        case MeasurmentUnit::ATMOSPHERE:
        case MeasurmentUnit::MILLIBAR:
        case MeasurmentUnit::PRESSURE_PERCENT:
        case MeasurmentUnit::MILLIBAR_X10:
        case MeasurmentUnit::MICROBAR:
        case MeasurmentUnit::KILO_PASCAL:
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
    case ReadingType::Name::HUMIDITY:
    {
        switch (unit)
        {
        case MeasurmentUnit::HUMIDITY_PERCENT:
        case MeasurmentUnit::HUMIDITY_PERCENT_X10:
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
    case ReadingType::Name::BATTERY_VOLTAGE:
    {
        switch (unit)
        {
        case MeasurmentUnit::VOLT:
        case MeasurmentUnit::MILLIVOLT:
        case MeasurmentUnit::CENTIVOLT:
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
    case ReadingType::Name::MOVEMENT:
    {
        switch (unit)
        {
        case MeasurmentUnit::MOVEMENT:
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
    case ReadingType::Name::LIGHT:
    {
        switch (unit)
        {
        case MeasurmentUnit::LIGHT_PERCENT:
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
    case ReadingType::Name::ACCELEROMETER:
    {
        switch (unit)
        {
        case MeasurmentUnit::METRES_PER_SQUARE_SECOND:
        case MeasurmentUnit::GRAVITY:
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
    case ReadingType::Name::GYROSCOPE:
    {
        switch (unit)
        {
        case MeasurmentUnit::GYROSCOPE:
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
    case ReadingType::Name::LOCATION:
    {
        switch (unit)
        {
        case MeasurmentUnit::LOCATION:
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
    case ReadingType::Name::HEART_RATE:
    {
        switch (unit)
        {
        case MeasurmentUnit::BEATS_PER_MINUTE:
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
    case ReadingType::Name::BATTERY_POWER:
    {
        switch (unit)
        {
        case MeasurmentUnit::BATTERY:
        case MeasurmentUnit::BATTERY_X1000:
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
    case ReadingType::Name::BREATHING_RATE:
    {
        switch (unit)
        {
        case MeasurmentUnit::BREATHS_PER_MINUTE:
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
    case ReadingType::Name::CALORIES:
    {
        switch (unit)
        {
        case MeasurmentUnit::CALORIES:
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
    case ReadingType::Name::ELECTRIC_CURRENT:
    {
        switch (unit)
        {
        case MeasurmentUnit::AMPERE:
        case MeasurmentUnit::MILLIAMPERE:
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
    case ReadingType::Name::POWER:
    {
        switch (unit)
        {
        case MeasurmentUnit::WATT:
        case MeasurmentUnit::HORSEPOWER:
        case MeasurmentUnit::MILLIWATT:
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
    case ReadingType::Name::FLOOR_POSITION:
    {
        switch (unit)
        {
        case MeasurmentUnit::METER:
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
    case ReadingType::Name::FLUID_VOLUME:
    {
        switch (unit)
        {
        case MeasurmentUnit::OUNCE_LIQUID_US:
        case MeasurmentUnit::GALLON_UK:
        case MeasurmentUnit::GALLON_DRY_US:
        case MeasurmentUnit::LITRE:
        case MeasurmentUnit::OUNCE_LIQUID_UK:
        case MeasurmentUnit::MILLILITRE:
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
    case ReadingType::Name::LENGHT:
    {
        switch (unit)
        {
        case MeasurmentUnit::METRE:
        case MeasurmentUnit::MILE:
        case MeasurmentUnit::POINT:
        case MeasurmentUnit::FOOT:
        case MeasurmentUnit::INCH:
        case MeasurmentUnit::PARSEC:
        case MeasurmentUnit::YARD:
        case MeasurmentUnit::MILLIMETER:
        case MeasurmentUnit::CENTIMETER:
        case MeasurmentUnit::KILOMETER:
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
    case ReadingType::Name::MASS:
    {
        switch (unit)
        {
        case MeasurmentUnit::KILOGRAM:
        case MeasurmentUnit::GALLON_LIQUID_US:
        case MeasurmentUnit::ATOMIC_MASS:
        case MeasurmentUnit::TON_UK:
        case MeasurmentUnit::METRIC_TON:
        case MeasurmentUnit::TON_US:
        case MeasurmentUnit::ELECTRON_MASS:
        case MeasurmentUnit::POUND:
        case MeasurmentUnit::OUNCE:
        case MeasurmentUnit::MILLIGRAM:
        case MeasurmentUnit::GRAM:
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
    case ReadingType::Name::SOUND_LEVEL:
    {
        switch (unit)
        {
        case MeasurmentUnit::DECIBEL:
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
    case ReadingType::Name::SPEED:
    {
        switch (unit)
        {
        case MeasurmentUnit::KNOT:
        case MeasurmentUnit::KILOMETERS_PER_HOUR:
        case MeasurmentUnit::MILES_PER_HOUR:
        case MeasurmentUnit::MACH:
        case MeasurmentUnit::SPEED_OF_LIGHT:
        case MeasurmentUnit::METER_PER_SECOND:
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
    case ReadingType::Name::TIME:
    {
        switch (unit)
        {
        case MeasurmentUnit::SECOND:
        case MeasurmentUnit::MINUTE:
        case MeasurmentUnit::HOUR:
        case MeasurmentUnit::MONTH:
        case MeasurmentUnit::DAY:
        case MeasurmentUnit::WEEK:
        case MeasurmentUnit::YEAR:
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
    case ReadingType::Name::MAGNETIC_FLUX_DENSITY:
    {
        switch (unit)
        {
        case MeasurmentUnit::TESLA:
        case MeasurmentUnit::GAUSS:
        case MeasurmentUnit::MICRO_TESLA:
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
    case ReadingType::Name::RADIATION:
    {
        switch (unit)
        {
        case MeasurmentUnit::SIEVERT:
        case MeasurmentUnit::BECQUEREL:
        case MeasurmentUnit::RUTHERFORD:
        case MeasurmentUnit::ROENTGEN:
        case MeasurmentUnit::RADIATION_DOSE_EFFECTIVE:
        case MeasurmentUnit::CURIE:
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
    case ReadingType::Name::FORCE:
    {
        switch (unit)
        {
        case MeasurmentUnit::NEWTON:
        case MeasurmentUnit::POUND_FORCE:
        case MeasurmentUnit::GRAVITY_FORCE:
        case MeasurmentUnit::KILOGRAM_FORCE:
        case MeasurmentUnit::DYNE:
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
    case ReadingType::Name::MEASURE:
    {
        switch (unit)
        {
        case MeasurmentUnit::SQUARE_METRE:
        case MeasurmentUnit::CUBIC_METRE:
        case MeasurmentUnit::BYTE:
        case MeasurmentUnit::GRADE:
        case MeasurmentUnit::HECTARE:
        case MeasurmentUnit::CUBIC_INCH:
        case MeasurmentUnit::REVOLUTION:
        case MeasurmentUnit::CENTIRADIAN:
        case MeasurmentUnit::RAD:
        case MeasurmentUnit::COMPUTER_POINT:
        case MeasurmentUnit::DEGREE_ANGLE:
        case MeasurmentUnit::SECOND_ANGLE:
        case MeasurmentUnit::MINUTE_ANGLE:
        case MeasurmentUnit::SPHERE:
        case MeasurmentUnit::ARE:
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
    case ReadingType::Name::ANGLE:
    {
        switch (unit)
        {
        case MeasurmentUnit::RADIAN:
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
    case ReadingType::Name::FREQUENCY:
    {
        switch (unit)
        {
        case MeasurmentUnit::HERTZ:
        case MeasurmentUnit::MEGA_HERTZ:
        case MeasurmentUnit::GIGA_HERTZ:
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
    case ReadingType::Name::MAGNETIC_FLUX:
    {
        switch (unit)
        {
        case MeasurmentUnit::WEBER:
        case MeasurmentUnit::MAXWELL:
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
    case ReadingType::Name::ELECTRIC_CAPACITY:
    {
        switch (unit)
        {
        case MeasurmentUnit::FARAD:
        case MeasurmentUnit::FARADAY:
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
    case ReadingType::Name::ELECTRIC_RESISTANCE:
    {
        switch (unit)
        {
        case MeasurmentUnit::OHM:
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
    case ReadingType::Name::ELECTRIC_CHARGE:
    {
        switch (unit)
        {
        case MeasurmentUnit::FRANKLIN:
        case MeasurmentUnit::ELECTRIC_CHARGE:
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
    case ReadingType::Name::ELECTRIC_MAGNETISM:
    {
        switch (unit)
        {
        case MeasurmentUnit::GILBERT:
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
    case ReadingType::Name::ELECTRIC_ENERGY:
    {
        switch (unit)
        {
        case MeasurmentUnit::JOULE:
        case MeasurmentUnit::ELECTRON_VOLT:
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
    case ReadingType::Name::ELECTRIC_INDUCTANCE:
    {
        switch (unit)
        {
        case MeasurmentUnit::HENRY:
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
    case ReadingType::Name::ELECTRIC_CONDUCTANCE:
    {
        switch (unit)
        {
        case MeasurmentUnit::SIEMENS:
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
    case ReadingType::Name::LUMINOUS_FLUX:
    {
        switch (unit)
        {
        case MeasurmentUnit::LUMEN:
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
    case ReadingType::Name::LUMINOUS_INTENSITY:
    {
        switch (unit)
        {
        case MeasurmentUnit::CANDELA:
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
    case ReadingType::Name::ILLUMINANCE:
    {
        switch (unit)
        {
        case MeasurmentUnit::LUX:
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
    case ReadingType::Name::GENERIC_TEXT:
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
    case ReadingType::Name::GENERIC_BOOLEAN:
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
    }
}

std::string ReadingType::symbolForUnit(ReadingType::MeasurmentUnit unit)
{
    switch (unit)
    {
    case ReadingType::MeasurmentUnit::BIT:
    {
        return "bit";
    }
    case ReadingType::MeasurmentUnit::PERCENT:
    {
        return "%";
    }
    case ReadingType::MeasurmentUnit::CO2_MOL:
    {
        return "mol";
    }
    case ReadingType::MeasurmentUnit::X10C:
    {
        return "X10℃";
    }
    case ReadingType::MeasurmentUnit::X100V:
    {
        return "X100V";
    }
    case ReadingType::MeasurmentUnit::NUMERIC:
    {
        return "";
    }
    case ReadingType::MeasurmentUnit::KELVIN:
    {
        return "K";
    }
    case ReadingType::MeasurmentUnit::CELSIUS:
    {
        return "℃";
    }
    case ReadingType::MeasurmentUnit::FAHRENHEIT:
    {
        return "°F";
    }
    case ReadingType::MeasurmentUnit::CELSIUS_X2:
    {
        return "X2C";
    }
    case ReadingType::MeasurmentUnit::CELSIUS_X10:
    {
        return "X10℃";
    }
    case ReadingType::MeasurmentUnit::PASCAL:
    {
        return "Pa";
    }
    case ReadingType::MeasurmentUnit::MILLIMETER_OF_MERCURY:
    {
        return "mmHg";
    }
    case ReadingType::MeasurmentUnit::INCH_OF_MERCURY:
    {
        return "inHg";
    }
    case ReadingType::MeasurmentUnit::BAR:
    {
        return "bar";
    }
    case ReadingType::MeasurmentUnit::ATMOSPHERE:
    {
        return "atm";
    }
    case ReadingType::MeasurmentUnit::MILLIBAR:
    {
        return "mb";
    }
    case ReadingType::MeasurmentUnit::PRESSURE_PERCENT:
    {
        return "%";
    }
    case ReadingType::MeasurmentUnit::MILLIBAR_X10:
    {
        return "X10mb";
    }
    case ReadingType::MeasurmentUnit::MICROBAR:
    {
        return "μbar";
    }
    case ReadingType::MeasurmentUnit::KILO_PASCAL:
    {
        return "kPa";
    }
    case ReadingType::MeasurmentUnit::HUMIDITY_PERCENT:
    {
        return "%";
    }
    case ReadingType::MeasurmentUnit::HUMIDITY_PERCENT_X10:
    {
        return "X10%";
    }
    case ReadingType::MeasurmentUnit::VOLT:
    {
        return "V";
    }
    case ReadingType::MeasurmentUnit::MILLIVOLT:
    {
        return "mV";
    }
    case ReadingType::MeasurmentUnit::CENTIVOLT:
    {
        return "cV";
    }
    case ReadingType::MeasurmentUnit::MOVEMENT:
    {
        return "";
    }
    case ReadingType::MeasurmentUnit::LIGHT_PERCENT:
    {
        return "%";
    }
    case ReadingType::MeasurmentUnit::METRES_PER_SQUARE_SECOND:
    {
        return "m/s²";
    }
    case ReadingType::MeasurmentUnit::GRAVITY:
    {
        return "g";
    }
    case ReadingType::MeasurmentUnit::GYROSCOPE:
    {
        return "deg/s";
    }
    case ReadingType::MeasurmentUnit::LOCATION:
    {
        return "";
    }
    case ReadingType::MeasurmentUnit::BEATS_PER_MINUTE:
    {
        return "bpm";
    }
    case ReadingType::MeasurmentUnit::BATTERY:
    {
        return "Wh";
    }
    case ReadingType::MeasurmentUnit::BATTERY_X1000:
    {
        return "kWh";
    }
    case ReadingType::MeasurmentUnit::BREATHS_PER_MINUTE:
    {
        return "bpm";
    }
    case ReadingType::MeasurmentUnit::CALORIES:
    {
        return "cal";
    }
    case ReadingType::MeasurmentUnit::AMPERE:
    {
        return "A";
    }
    case ReadingType::MeasurmentUnit::MILLIAMPERE:
    {
        return "mA";
    }
    case ReadingType::MeasurmentUnit::WATT:
    {
        return "W";
    }
    case ReadingType::MeasurmentUnit::HORSEPOWER:
    {
        return "hp";
    }
    case ReadingType::MeasurmentUnit::MILLIWATT:
    {
        return "mW";
    }
    case ReadingType::MeasurmentUnit::METER:
    {
        return "";
    }
    case ReadingType::MeasurmentUnit::OUNCE_LIQUID_US:
    {
        return "oz";
    }
    case ReadingType::MeasurmentUnit::GALLON_UK:
    {
        return "gallon_u";
    }
    case ReadingType::MeasurmentUnit::GALLON_DRY_US:
    {
        return "gallon_d";
    }
    case ReadingType::MeasurmentUnit::LITRE:
    {
        return "L";
    }
    case ReadingType::MeasurmentUnit::OUNCE_LIQUID_UK:
    {
        return "oz_uk";
    }
    case ReadingType::MeasurmentUnit::MILLILITRE:
    {
        return "mL";
    }
    case ReadingType::MeasurmentUnit::METRE:
    {
        return "m";
    }
    case ReadingType::MeasurmentUnit::MILE:
    {
        return "mi";
    }
    case ReadingType::MeasurmentUnit::POINT:
    {
        return "pt";
    }
    case ReadingType::MeasurmentUnit::FOOT:
    {
        return "ft";
    }
    case ReadingType::MeasurmentUnit::INCH:
    {
        return "in";
    }
    case ReadingType::MeasurmentUnit::PARSEC:
    {
        return "pc";
    }
    case ReadingType::MeasurmentUnit::YARD:
    {
        return "yd";
    }
    case ReadingType::MeasurmentUnit::MILLIMETER:
    {
        return "mm";
    }
    case ReadingType::MeasurmentUnit::CENTIMETER:
    {
        return "cm";
    }
    case ReadingType::MeasurmentUnit::KILOMETER:
    {
        return "km";
    }
    case ReadingType::MeasurmentUnit::KILOGRAM:
    {
        return "kg";
    }
    case ReadingType::MeasurmentUnit::GALLON_LIQUID_US:
    {
        return "gal";
    }
    case ReadingType::MeasurmentUnit::ATOMIC_MASS:
    {
        return "u";
    }
    case ReadingType::MeasurmentUnit::TON_UK:
    {
        return "ton_uk";
    }
    case ReadingType::MeasurmentUnit::METRIC_TON:
    {
        return "t";
    }
    case ReadingType::MeasurmentUnit::TON_US:
    {
        return "ton_us";
    }
    case ReadingType::MeasurmentUnit::ELECTRON_MASS:
    {
        return "me";
    }
    case ReadingType::MeasurmentUnit::POUND:
    {
        return "lb";
    }
    case ReadingType::MeasurmentUnit::OUNCE:
    {
        return "oz";
    }
    case ReadingType::MeasurmentUnit::MILLIGRAM:
    {
        return "mg";
    }
    case ReadingType::MeasurmentUnit::GRAM:
    {
        return "g";
    }
    case ReadingType::MeasurmentUnit::DECIBEL:
    {
        return "dB";
    }
    case ReadingType::MeasurmentUnit::KNOT:
    {
        return "kn";
    }
    case ReadingType::MeasurmentUnit::KILOMETERS_PER_HOUR:
    {
        return "km/h";
    }
    case ReadingType::MeasurmentUnit::MILES_PER_HOUR:
    {
        return "mph";
    }
    case ReadingType::MeasurmentUnit::MACH:
    {
        return "Mach";
    }
    case ReadingType::MeasurmentUnit::SPEED_OF_LIGHT:
    {
        return "c";
    }
    case ReadingType::MeasurmentUnit::METER_PER_SECOND:
    {
        return "m/s";
    }
    case ReadingType::MeasurmentUnit::SECOND:
    {
        return "s";
    }
    case ReadingType::MeasurmentUnit::MINUTE:
    {
        return "min";
    }
    case ReadingType::MeasurmentUnit::HOUR:
    {
        return "h";
    }
    case ReadingType::MeasurmentUnit::MONTH:
    {
        return "month";
    }
    case ReadingType::MeasurmentUnit::DAY:
    {
        return "day";
    }
    case ReadingType::MeasurmentUnit::WEEK:
    {
        return "week";
    }
    case ReadingType::MeasurmentUnit::YEAR:
    {
        return "year";
    }
    case ReadingType::MeasurmentUnit::TESLA:
    {
        return "T";
    }
    case ReadingType::MeasurmentUnit::GAUSS:
    {
        return "G";
    }
    case ReadingType::MeasurmentUnit::MICRO_TESLA:
    {
        return "μT";
    }
    case ReadingType::MeasurmentUnit::SIEVERT:
    {
        return "C";
    }
    case ReadingType::MeasurmentUnit::BECQUEREL:
    {
        return "Bq";
    }
    case ReadingType::MeasurmentUnit::RUTHERFORD:
    {
        return "Rd";
    }
    case ReadingType::MeasurmentUnit::ROENTGEN:
    {
        return "Roentgen";
    }
    case ReadingType::MeasurmentUnit::RADIATION_DOSE_EFFECTIVE:
    {
        return "rem";
    }
    case ReadingType::MeasurmentUnit::CURIE:
    {
        return "Ci";
    }
    case ReadingType::MeasurmentUnit::NEWTON:
    {
        return "N";
    }
    case ReadingType::MeasurmentUnit::POUND_FORCE:
    {
        return "lbf";
    }
    case ReadingType::MeasurmentUnit::GRAVITY_FORCE:
    {
        return "grav";
    }
    case ReadingType::MeasurmentUnit::KILOGRAM_FORCE:
    {
        return "kgf";
    }
    case ReadingType::MeasurmentUnit::DYNE:
    {
        return "dyn";
    }
    case ReadingType::MeasurmentUnit::SQUARE_METRE:
    {
        return "m²";
    }
    case ReadingType::MeasurmentUnit::CUBIC_METRE:
    {
        return "m³";
    }
    case ReadingType::MeasurmentUnit::BYTE:
    {
        return "byte";
    }
    case ReadingType::MeasurmentUnit::GRADE:
    {
        return "grade";
    }
    case ReadingType::MeasurmentUnit::HECTARE:
    {
        return "ha";
    }
    case ReadingType::MeasurmentUnit::CUBIC_INCH:
    {
        return "in³";
    }
    case ReadingType::MeasurmentUnit::REVOLUTION:
    {
        return "rev";
    }
    case ReadingType::MeasurmentUnit::CENTIRADIAN:
    {
        return "centirad";
    }
    case ReadingType::MeasurmentUnit::RAD:
    {
        return "rd";
    }
    case ReadingType::MeasurmentUnit::COMPUTER_POINT:
    {
        return "pixel";
    }
    case ReadingType::MeasurmentUnit::DEGREE_ANGLE:
    {
        return "°";
    }
    case ReadingType::MeasurmentUnit::SECOND_ANGLE:
    {
        return "″";
    }
    case ReadingType::MeasurmentUnit::MINUTE_ANGLE:
    {
        return "′";
    }
    case ReadingType::MeasurmentUnit::SPHERE:
    {
        return "sphere";
    }
    case ReadingType::MeasurmentUnit::ARE:
    {
        return "a";
    }
    case ReadingType::MeasurmentUnit::RADIAN:
    {
        return "rad";
    }
    case ReadingType::MeasurmentUnit::HERTZ:
    {
        return "Hz";
    }
    case ReadingType::MeasurmentUnit::MEGA_HERTZ:
    {
        return "MHz";
    }
    case ReadingType::MeasurmentUnit::GIGA_HERTZ:
    {
        return "GHz";
    }
    case ReadingType::MeasurmentUnit::WEBER:
    {
        return "Wb";
    }
    case ReadingType::MeasurmentUnit::MAXWELL:
    {
        return "Mx";
    }
    case ReadingType::MeasurmentUnit::FARAD:
    {
        return "F";
    }
    case ReadingType::MeasurmentUnit::FARADAY:
    {
        return "Fd";
    }
    case ReadingType::MeasurmentUnit::OHM:
    {
        return "Ω";
    }
    case ReadingType::MeasurmentUnit::FRANKLIN:
    {
        return "Fr";
    }
    case ReadingType::MeasurmentUnit::ELECTRIC_CHARGE:
    {
        return "e";
    }
    case ReadingType::MeasurmentUnit::GILBERT:
    {
        return "Gi";
    }
    case ReadingType::MeasurmentUnit::JOULE:
    {
        return "J";
    }
    case ReadingType::MeasurmentUnit::ELECTRON_VOLT:
    {
        return "eV";
    }
    case ReadingType::MeasurmentUnit::HENRY:
    {
        return "H";
    }
    case ReadingType::MeasurmentUnit::SIEMENS:
    {
        return "S";
    }
    case ReadingType::MeasurmentUnit::LUMEN:
    {
        return "lm";
    }
    case ReadingType::MeasurmentUnit::CANDELA:
    {
        return "cd";
    }
    case ReadingType::MeasurmentUnit::LUX:
    {
        return "lx";
    }
    case ReadingType::MeasurmentUnit::TEXT:
    {
        return "";
    }
    case ReadingType::MeasurmentUnit::BOOLEAN:
    {
        return "";
    }
    }

    return "";
}

std::string toString(ReadingType::Name name)
{
    switch (name)
    {
    case ReadingType::Name::GENERIC:
    {
        return "GENERIC";
    }
    case ReadingType::Name::TEMPERATURE:
    {
        return "TEMPERATURE";
    }
    case ReadingType::Name::PRESSURE:
    {
        return "PRESSURE";
    }
    case ReadingType::Name::HUMIDITY:
    {
        return "HUMIDITY";
    }
    case ReadingType::Name::BATTERY_VOLTAGE:
    {
        return "BATTERY_VOLTAGE";
    }
    case ReadingType::Name::MOVEMENT:
    {
        return "MOVEMENT";
    }
    case ReadingType::Name::LIGHT:
    {
        return "LIGHT";
    }
    case ReadingType::Name::ACCELEROMETER:
    {
        return "ACCELEROMETER";
    }
    case ReadingType::Name::GYROSCOPE:
    {
        return "GYROSCOPE";
    }
    case ReadingType::Name::LOCATION:
    {
        return "LOCATION";
    }
    case ReadingType::Name::HEART_RATE:
    {
        return "HEART_RATE";
    }
    case ReadingType::Name::BATTERY_POWER:
    {
        return "BATTERY_POWER";
    }
    case ReadingType::Name::BREATHING_RATE:
    {
        return "BREATHING_RATE";
    }
    case ReadingType::Name::CALORIES:
    {
        return "CALORIES";
    }
    case ReadingType::Name::ELECTRIC_CURRENT:
    {
        return "ELECTRIC_CURRENT";
    }
    case ReadingType::Name::POWER:
    {
        return "POWER";
    }
    case ReadingType::Name::FLOOR_POSITION:
    {
        return "FLOOR_POSITION";
    }
    case ReadingType::Name::FLUID_VOLUME:
    {
        return "FLUID_VOLUME";
    }
    case ReadingType::Name::LENGHT:
    {
        return "LENGHT";
    }
    case ReadingType::Name::MASS:
    {
        return "MASS";
    }
    case ReadingType::Name::SOUND_LEVEL:
    {
        return "SOUND_LEVEL";
    }
    case ReadingType::Name::SPEED:
    {
        return "SPEED";
    }
    case ReadingType::Name::TIME:
    {
        return "TIME";
    }
    case ReadingType::Name::MAGNETIC_FLUX_DENSITY:
    {
        return "MAGNETIC_FLUX_DENSITY";
    }
    case ReadingType::Name::RADIATION:
    {
        return "RADIATION";
    }
    case ReadingType::Name::FORCE:
    {
        return "FORCE";
    }
    case ReadingType::Name::MEASURE:
    {
        return "MEASURE";
    }
    case ReadingType::Name::ANGLE:
    {
        return "ANGLE";
    }
    case ReadingType::Name::FREQUENCY:
    {
        return "FREQUENCY";
    }
    case ReadingType::Name::MAGNETIC_FLUX:
    {
        return "MAGNETIC_FLUX";
    }
    case ReadingType::Name::ELECTRIC_CAPACITY:
    {
        return "ELECTRIC_CAPACITY";
    }
    case ReadingType::Name::ELECTRIC_RESISTANCE:
    {
        return "ELECTRIC_RESISTANCE";
    }
    case ReadingType::Name::ELECTRIC_CHARGE:
    {
        return "ELECTRIC_CHARGE";
    }
    case ReadingType::Name::ELECTRIC_MAGNETISM:
    {
        return "ELECTRIC_MAGNETISM";
    }
    case ReadingType::Name::ELECTRIC_ENERGY:
    {
        return "ELECTRIC_ENERGY";
    }
    case ReadingType::Name::ELECTRIC_INDUCTANCE:
    {
        return "ELECTRIC_INDUCTANCE";
    }
    case ReadingType::Name::ELECTRIC_CONDUCTANCE:
    {
        return "ELECTRIC_CONDUCTANCE";
    }
    case ReadingType::Name::LUMINOUS_FLUX:
    {
        return "LUMINOUS_FLUX";
    }
    case ReadingType::Name::LUMINOUS_INTENSITY:
    {
        return "LUMINOUS_INTENSITY";
    }
    case ReadingType::Name::ILLUMINANCE:
    {
        return "ILLUMINANCE";
    }
    case ReadingType::Name::GENERIC_TEXT:
    {
        return "GENERIC_TEXT";
    }
    case ReadingType::Name::GENERIC_BOOLEAN:
    {
        return "GENERIC_BOOLEAN";
    }
    }

    return "";
}

std::string toString(ReadingType::MeasurmentUnit unit)
{
    switch (unit)
    {
    case ReadingType::MeasurmentUnit::BIT:
    {
        return "BIT";
    }
    case ReadingType::MeasurmentUnit::PERCENT:
    {
        return "PERCENT";
    }
    case ReadingType::MeasurmentUnit::CO2_MOL:
    {
        return "CO2_MOL";
    }
    case ReadingType::MeasurmentUnit::X10C:
    {
        return "X10℃";
    }
    case ReadingType::MeasurmentUnit::X100V:
    {
        return "X100V";
    }
    case ReadingType::MeasurmentUnit::X10PA:
    {
        return "X10Pa";
    }
    case ReadingType::MeasurmentUnit::NUMERIC:
    {
        return "NUMERIC";
    }
    case ReadingType::MeasurmentUnit::KELVIN:
    {
        return "KELVIN";
    }
    case ReadingType::MeasurmentUnit::CELSIUS:
    {
        return "CELSIUS";
    }
    case ReadingType::MeasurmentUnit::FAHRENHEIT:
    {
        return "FAHRENHEIT";
    }
    case ReadingType::MeasurmentUnit::CELSIUS_X2:
    {
        return "CELSIUS_X2";
    }
    case ReadingType::MeasurmentUnit::CELSIUS_X10:
    {
        return "CELSIUS_X10";
    }
    case ReadingType::MeasurmentUnit::PASCAL:
    {
        return "PASCAL";
    }
    case ReadingType::MeasurmentUnit::MILLIMETER_OF_MERCURY:
    {
        return "MILLIMETER_OF_MERCURY";
    }
    case ReadingType::MeasurmentUnit::INCH_OF_MERCURY:
    {
        return "INCH_OF_MERCURY";
    }
    case ReadingType::MeasurmentUnit::BAR:
    {
        return "BAR";
    }
    case ReadingType::MeasurmentUnit::ATMOSPHERE:
    {
        return "ATMOSPHERE";
    }
    case ReadingType::MeasurmentUnit::MILLIBAR:
    {
        return "MILLIBAR";
    }
    case ReadingType::MeasurmentUnit::PRESSURE_PERCENT:
    {
        return "PRESSURE_PERCENT";
    }
    case ReadingType::MeasurmentUnit::MILLIBAR_X10:
    {
        return "MILLIBAR_X10";
    }
    case ReadingType::MeasurmentUnit::MICROBAR:
    {
        return "MICROBAR";
    }
    case ReadingType::MeasurmentUnit::KILO_PASCAL:
    {
        return "KILO_PASCAL";
    }
    case ReadingType::MeasurmentUnit::HUMIDITY_PERCENT:
    {
        return "HUMIDITY_PERCENT";
    }
    case ReadingType::MeasurmentUnit::HUMIDITY_PERCENT_X10:
    {
        return "HUMIDITY_PERCENT_X10";
    }
    case ReadingType::MeasurmentUnit::VOLT:
    {
        return "VOLT";
    }
    case ReadingType::MeasurmentUnit::MILLIVOLT:
    {
        return "MILLIVOLT";
    }
    case ReadingType::MeasurmentUnit::CENTIVOLT:
    {
        return "CENTIVOLT";
    }
    case ReadingType::MeasurmentUnit::MOVEMENT:
    {
        return "MOVEMENT";
    }
    case ReadingType::MeasurmentUnit::LIGHT_PERCENT:
    {
        return "LIGHT_PERCENT";
    }
    case ReadingType::MeasurmentUnit::METRES_PER_SQUARE_SECOND:
    {
        return "METRES_PER_SQUARE_SECOND";
    }
    case ReadingType::MeasurmentUnit::GRAVITY:
    {
        return "GRAVITY";
    }
    case ReadingType::MeasurmentUnit::GYROSCOPE:
    {
        return "GYROSCOPE";
    }
    case ReadingType::MeasurmentUnit::LOCATION:
    {
        return "LOCATION";
    }
    case ReadingType::MeasurmentUnit::BEATS_PER_MINUTE:
    {
        return "BEATS_PER_MINUTE";
    }
    case ReadingType::MeasurmentUnit::BATTERY:
    {
        return "BATTERY";
    }
    case ReadingType::MeasurmentUnit::BATTERY_X1000:
    {
        return "BATTERY_X1000";
    }
    case ReadingType::MeasurmentUnit::BREATHS_PER_MINUTE:
    {
        return "BREATHS_PER_MINUTE";
    }
    case ReadingType::MeasurmentUnit::CALORIES:
    {
        return "CALORIES";
    }
    case ReadingType::MeasurmentUnit::AMPERE:
    {
        return "AMPERE";
    }
    case ReadingType::MeasurmentUnit::MILLIAMPERE:
    {
        return "MILLIAMPERE";
    }
    case ReadingType::MeasurmentUnit::WATT:
    {
        return "WATT";
    }
    case ReadingType::MeasurmentUnit::HORSEPOWER:
    {
        return "HORSEPOWER";
    }
    case ReadingType::MeasurmentUnit::MILLIWATT:
    {
        return "MILLIWATT";
    }
    case ReadingType::MeasurmentUnit::METER:
    {
        return "METER";
    }
    case ReadingType::MeasurmentUnit::OUNCE_LIQUID_US:
    {
        return "OUNCE_LIQUID_US";
    }
    case ReadingType::MeasurmentUnit::GALLON_UK:
    {
        return "GALLON_UK";
    }
    case ReadingType::MeasurmentUnit::GALLON_DRY_US:
    {
        return "GALLON_DRY_US";
    }
    case ReadingType::MeasurmentUnit::LITRE:
    {
        return "LITRE";
    }
    case ReadingType::MeasurmentUnit::OUNCE_LIQUID_UK:
    {
        return "OUNCE_LIQUID_UK";
    }
    case ReadingType::MeasurmentUnit::MILLILITRE:
    {
        return "MILLILITRE";
    }
    case ReadingType::MeasurmentUnit::METRE:
    {
        return "METRE";
    }
    case ReadingType::MeasurmentUnit::MILE:
    {
        return "MILE";
    }
    case ReadingType::MeasurmentUnit::POINT:
    {
        return "POINT";
    }
    case ReadingType::MeasurmentUnit::INCH:
    {
        return "INCH";
    }
    case ReadingType::MeasurmentUnit::FOOT:
    {
        return "FOOT";
    }
    case ReadingType::MeasurmentUnit::YARD:
    {
        return "YARD";
    }
    case ReadingType::MeasurmentUnit::MILLIMETER:
    {
        return "MILLIMETER";
    }
    case ReadingType::MeasurmentUnit::CENTIMETER:
    {
        return "CENTIMETER";
    }
    case ReadingType::MeasurmentUnit::KILOMETER:
    {
        return "KILOMETER";
    }
    case ReadingType::MeasurmentUnit::PARSEC:
    {
        return "PARSEC";
    }
    case ReadingType::MeasurmentUnit::KILOGRAM:
    {
        return "KILOGRAM";
    }
    case ReadingType::MeasurmentUnit::GALLON_LIQUID_US:
    {
        return "GALLON_LIQUID_US";
    }
    case ReadingType::MeasurmentUnit::ATOMIC_MASS:
    {
        return "ATOMIC_MASS";
    }
    case ReadingType::MeasurmentUnit::TON_UK:
    {
        return "TON_UK";
    }
    case ReadingType::MeasurmentUnit::METRIC_TON:
    {
        return "METRIC_TON";
    }
    case ReadingType::MeasurmentUnit::TON_US:
    {
        return "TON_US";
    }
    case ReadingType::MeasurmentUnit::ELECTRON_MASS:
    {
        return "ELECTRON_MASS";
    }
    case ReadingType::MeasurmentUnit::POUND:
    {
        return "POUND";
    }
    case ReadingType::MeasurmentUnit::OUNCE:
    {
        return "OUNCE";
    }
    case ReadingType::MeasurmentUnit::MILLIGRAM:
    {
        return "MILLIGRAM";
    }
    case ReadingType::MeasurmentUnit::GRAM:
    {
        return "GRAM";
    }
    case ReadingType::MeasurmentUnit::DECIBEL:
    {
        return "DECIBEL";
    }
    case ReadingType::MeasurmentUnit::KNOT:
    {
        return "KNOT";
    }
    case ReadingType::MeasurmentUnit::KILOMETERS_PER_HOUR:
    {
        return "KILOMETERS_PER_HOUR";
    }
    case ReadingType::MeasurmentUnit::MILES_PER_HOUR:
    {
        return "MILES_PER_HOUR";
    }
    case ReadingType::MeasurmentUnit::MACH:
    {
        return "MACH";
    }
    case ReadingType::MeasurmentUnit::SPEED_OF_LIGHT:
    {
        return "SPEED_OF_LIGHT";
    }
    case ReadingType::MeasurmentUnit::METER_PER_SECOND:
    {
        return "METER_PER_SECOND";
    }
    case ReadingType::MeasurmentUnit::SECOND:
    {
        return "SECOND";
    }
    case ReadingType::MeasurmentUnit::MINUTE:
    {
        return "MINUTE";
    }
    case ReadingType::MeasurmentUnit::HOUR:
    {
        return "HOUR";
    }
    case ReadingType::MeasurmentUnit::MONTH:
    {
        return "MONTH";
    }
    case ReadingType::MeasurmentUnit::DAY:
    {
        return "DAY";
    }
    case ReadingType::MeasurmentUnit::WEEK:
    {
        return "WEEK";
    }
    case ReadingType::MeasurmentUnit::YEAR:
    {
        return "YEAR";
    }
    case ReadingType::MeasurmentUnit::TESLA:
    {
        return "TESLA";
    }
    case ReadingType::MeasurmentUnit::GAUSS:
    {
        return "GAUSS";
    }
    case ReadingType::MeasurmentUnit::MICRO_TESLA:
    {
        return "MICRO_TESLA";
    }
    case ReadingType::MeasurmentUnit::SIEVERT:
    {
        return "SIEVERT";
    }
    case ReadingType::MeasurmentUnit::BECQUEREL:
    {
        return "BECQUEREL";
    }
    case ReadingType::MeasurmentUnit::RUTHERFORD:
    {
        return "RUTHERFORD";
    }
    case ReadingType::MeasurmentUnit::ROENTGEN:
    {
        return "ROENTGEN";
    }
    case ReadingType::MeasurmentUnit::RADIATION_DOSE_EFFECTIVE:
    {
        return "RADIATION_DOSE_EFFECTIVE";
    }
    case ReadingType::MeasurmentUnit::CURIE:
    {
        return "CURIE";
    }
    case ReadingType::MeasurmentUnit::NEWTON:
    {
        return "NEWTON";
    }
    case ReadingType::MeasurmentUnit::POUND_FORCE:
    {
        return "POUND_FORCE";
    }
    case ReadingType::MeasurmentUnit::GRAVITY_FORCE:
    {
        return "GRAVITY";
    }
    case ReadingType::MeasurmentUnit::KILOGRAM_FORCE:
    {
        return "KILOGRAM_FORCE";
    }
    case ReadingType::MeasurmentUnit::DYNE:
    {
        return "DYNE";
    }
    case ReadingType::MeasurmentUnit::SQUARE_METRE:
    {
        return "SQUARE_METRE";
    }
    case ReadingType::MeasurmentUnit::CUBIC_METRE:
    {
        return "CUBIC_METRE";
    }
    case ReadingType::MeasurmentUnit::BYTE:
    {
        return "BYTE";
    }
    case ReadingType::MeasurmentUnit::GRADE:
    {
        return "GRADE";
    }
    case ReadingType::MeasurmentUnit::HECTARE:
    {
        return "HECTARE";
    }
    case ReadingType::MeasurmentUnit::CUBIC_INCH:
    {
        return "CUBIC_INCH";
    }
    case ReadingType::MeasurmentUnit::REVOLUTION:
    {
        return "REVOLUTION";
    }
    case ReadingType::MeasurmentUnit::CENTIRADIAN:
    {
        return "CENTIRADIAN";
    }
    case ReadingType::MeasurmentUnit::RAD:
    {
        return "RAD";
    }
    case ReadingType::MeasurmentUnit::COMPUTER_POINT:
    {
        return "COMPUTER_POINT";
    }
    case ReadingType::MeasurmentUnit::DEGREE_ANGLE:
    {
        return "DEGREE_ANGLE";
    }
    case ReadingType::MeasurmentUnit::SECOND_ANGLE:
    {
        return "SECOND_ANGLE";
    }
    case ReadingType::MeasurmentUnit::MINUTE_ANGLE:
    {
        return "MINUTE_ANGLE";
    }
    case ReadingType::MeasurmentUnit::SPHERE:
    {
        return "SPHERE";
    }
    case ReadingType::MeasurmentUnit::ARE:
    {
        return "ARE";
    }
    case ReadingType::MeasurmentUnit::RADIAN:
    {
        return "RADIAN";
    }
    case ReadingType::MeasurmentUnit::HERTZ:
    {
        return "HERTZ";
    }
    case ReadingType::MeasurmentUnit::MEGA_HERTZ:
    {
        return "MEGA_HERTZ";
    }
    case ReadingType::MeasurmentUnit::GIGA_HERTZ:
    {
        return "GIGA_HERTZ";
    }
    case ReadingType::MeasurmentUnit::WEBER:
    {
        return "WEBER";
    }
    case ReadingType::MeasurmentUnit::MAXWELL:
    {
        return "MAXWELL";
    }
    case ReadingType::MeasurmentUnit::FARAD:
    {
        return "FARAD";
    }
    case ReadingType::MeasurmentUnit::FARADAY:
    {
        return "FARADAY";
    }
    case ReadingType::MeasurmentUnit::OHM:
    {
        return "OHM";
    }
    case ReadingType::MeasurmentUnit::FRANKLIN:
    {
        return "FRANKLIN";
    }
    case ReadingType::MeasurmentUnit::ELECTRIC_CHARGE:
    {
        return "ELECTRIC_CHARGE";
    }
    case ReadingType::MeasurmentUnit::GILBERT:
    {
        return "GILBERT";
    }
    case ReadingType::MeasurmentUnit::JOULE:
    {
        return "JOULE";
    }
    case ReadingType::MeasurmentUnit::ELECTRON_VOLT:
    {
        return "ELECTRON_VOLT";
    }
    case ReadingType::MeasurmentUnit::HENRY:
    {
        return "HENRY";
    }
    case ReadingType::MeasurmentUnit::SIEMENS:
    {
        return "SIEMENS";
    }
    case ReadingType::MeasurmentUnit::LUMEN:
    {
        return "LUMEN";
    }
    case ReadingType::MeasurmentUnit::CANDELA:
    {
        return "CANDELA";
    }
    case ReadingType::MeasurmentUnit::LUX:
    {
        return "LUX";
    }
    case ReadingType::MeasurmentUnit::TEXT:
    {
        return "TEXT";
    }
    case ReadingType::MeasurmentUnit::BOOLEAN:
    {
        return "BOOLEAN";
    }
    }

    return "";
}
}    // namespace wolkabout

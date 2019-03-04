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
#ifndef READINGTYPE_H
#define READINGTYPE_H

#include "model/DataType.h"

#include <vector>

namespace wolkabout
{
class ReadingType
{
public:
    enum class Name
    {
        GENERIC = 0,
        TEMPERATURE,
        PRESSURE,
        HUMIDITY,
        BATTERY_VOLTAGE,
        MOVEMENT,
        LIGHT,
        ACCELEROMETER,
        GYROSCOPE,
        LOCATION,
        HEART_RATE,
        COUNT,
        BATTERY_POWER,
        BREATHING_RATE,
        CALORIES,
        ELECTRIC_CURRENT,
        POWER,
        FLOOR_POSITION,
        FLUID_VOLUME,
        LENGHT,
        MASS,
        SOUND_LEVEL,
        SPEED,
        STRING,
        SWITCH,
        TIME,
        MAGNETIC_FLUX_DENSITY,
        RADIATION,
        FORCE,
        MEASURE,
        ANGLE,
        FREQUENCY,
        MAGNETIC_FLUX,
        ELECTRIC_CAPACITY,
        ELECTRIC_RESISTANCE,
        ELECTRIC_CHARGE,
        ELECTRIC_MAGNETISM,
        ELECTRIC_ENERGY,
        ELECTRIC_INDUCTANCE,
        ELECTRIC_CONDUCTANCE,
        LUMINOUS_FLUX,
        LUMINOUS_INTENSITY,
        ILLUMINANCE,
        GENERIC_TEXT,
        GENERIC_BOOLEAN
    };

    enum class MeasurmentUnit
    {
        BIT = 0,
        PERCENT,
        CO2_MOL,
        X10C,
        X100V,
        X10Pa,
        NUMERIC,

        KELVIN,
        CELSIUS,
        RANKINE,
        FAHRENHEIT,
        CELSIUS_X2,
        CELSIUS_X10,

        PASCAL,
        MILLIMETER_OF_MERCURY,
        INCH_OF_MERCURY,
        BAR,
        ATMOSPHERE,
        MILLIBAR,
        PRESSURE_PERCENT,
        MILLIBAR_X10,
        MICROBAR,
        KILO_PASCAL,

        HUMIDITY_PERCENT,
        HUMIDITY_PERCENT_X10,

        VOLT,
        MILLIVOLT,
        CENTIVOLT,

        MOVEMENT,

        LIGHT_PERCENT,

        METRES_PER_SQUARE_SECOND,
        GRAVITY,

        GYROSCOPE,

        LOCATION,

        BEATS_PER_MINUTE,

        BATTERY,
        BATTERY_X1000,

        BREATHS_PER_MINUTE,

        CALORIES,

        AMPERE,
        MILLIAMPERE,

        WATT,
        HORSEPOWER,
        MILLIWATT,

        METER,

        OUNCE_LIQUID_US,
        GALLON_UK,
        GALLON_DRY_US,
        LITRE,
        OUNCE_LIQUID_UK,
        MILLILITRE,

        METRE,
        MILE,
        POINT,
        FOOT,
        INCH,
        PARSEC,
        YARD,
        MILLIMETER,
        CENTIMETER,
        KILOMETER,

        KILOGRAM,
        GALLON_LIQUID_US,
        ATOMIC_MASS,
        TON_UK,
        METRIC_TON,
        TON_US,
        ELECTRON_MASS,
        POUND,
        OUNCE,
        MILLIGRAM,
        GRAM,

        DECIBEL,

        KNOT,
        KILOMETERS_PER_HOUR,
        MILES_PER_HOUR,
        MACH,
        SPEED_OF_LIGHT,
        METER_PER_SECOND,

        SECOND,
        MINUTE,
        HOUR,
        MONTH,
        DAY,
        WEEK,
        YEAR,

        TESLA,
        GAUSS,
        MICRO_TESLA,

        SIEVERT,
        BECQUEREL,
        RUTHERFORD,
        ROENTGEN,
        RADIATION_DOSE_EFFECTIVE,
        CURIE,

        NEWTON,
        POUND_FORCE,
        KILOGRAM_FORCE,
        DYNE,

        SQUARE_METRE,
        CUBIC_METRE,
        BYTE,
        GRADE,
        HECTARE,
        CUBIC_INCH,
        REVOLUTION,
        CENTIRADIAN,
        RAD,
        COMPUTER_POINT,
        DEGREE_ANGLE,
        SECOND_ANGLE,
        MINUTE_ANGLE,
        SPHERE,
        ARE,

        RADIAN,

        HERTZ,
        MEGA_HERTZ,
        GIGA_HERTZ,

        WEBER,
        MAXWELL,

        FARAD,
        FARADAY,

        OHM,
        
        FRANKLIN,
        ELECTRIC_CHARGE,
        
        GILBERT,
        
        JOULE,
        ELECTRON_VOLT,
        
        HENRY,
        
        SIEMENS,

        LUMEN,

        CANDELA,

        LUX,

        TEXT,

        BOOLEAN

    };

    ReadingType() = default;
    ReadingType(DataType dataType);
    ReadingType(ReadingType::Name name, ReadingType::MeasurmentUnit unit);
    ReadingType(std::string name, std::string unitSymbol);

    const std::string& getName() const;

    const std::string& getMeasurmentUnit() const;

    const std::string& getMeasurmentUnitSymbol() const;

    DataType getDataType() const;

    bool operator==(ReadingType& rhs) const;
    bool operator!=(ReadingType& rhs) const;

private:
    std::string m_name;
    std::string m_unit;
    DataType m_dataType;
    std::string m_unitSymbol;


    void validate(ReadingType::Name name, ReadingType::MeasurmentUnit unit);
    DataType dataTypeForName(ReadingType::Name name);
    std::string symbolForUnit(ReadingType::MeasurmentUnit unit);
};

std::string toString(ReadingType::Name name);
std::string toString(ReadingType::MeasurmentUnit unit);
}    // namespace wolkabout

#endif    // READINGTYPE_H

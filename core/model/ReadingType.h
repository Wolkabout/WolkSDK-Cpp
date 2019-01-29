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

#include <string>
#include <vector>

namespace wolkabout
{
class ReadingType
{
public:
    enum class Name
    {
        GENERIC = 0,
        GENERIC_TEXT,
        GENERIC_BOOLEAN,
        TEMPERATURE,
        PRESSURE,
        HUMIDITY,
        BATTERY,
        MOVEMENT,
        LIGHT,
        ACCELEROMETER,
        GYROSCOPE,
        MAGNETOMETER,
        LOCATION,
        HEART_RATE,
        COUNT,
        BATTERY_POWER,
        BREATHING_RATE,
        CALORIES,
        ELECTRIC_CURRENT,
        ELECTRIC_POWER,
        ELECTRIC_VOLTAGE,
        FLOOR_POSITION,
        FLUID_VOLUME,
        LENGHT,
        LOAD_WEIGHT,
        SOUND_LEVEL,
        SPEED,
        STEPS,
        STRING,
        SWITCH,
        TIME,
        MAGNETIC_FLUX_DENSITY,
        RADIATION,
        ELECTRICITY,
        FORCE,
        ENERGY,
        MEASURE
    };

    enum class MeasurmentUnit
    {
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

        LUMEN,
        COULOMB,
        LUX,
        CANDELA,
        LAMBERT,
        LIGHT_PERCENT,

        METRES_PER_SQUARE_SECOND,
        GRAVITY,

        GYROSCOPE,

        MAXWELL,

        LOCATION,

        BEATS_PER_MINUTE,

        STEPS,
        COUNT,

        BATTERY,
        BATTERY_X1000,

        BREATHS_PER_MINUTE,

        CALORIES,

        AMPERE,
        MILLIAMPERE,

        WATT,
        HORSEPOWER,
        MILLIWATT,

        // MILLIVOLT,

        METER,

        OUNCE_LIQUID_US,
        GALLON_UK,
        GALLON_DRY_US,
        LITRE,
        OUNCE_LIQUID_UK,
        MILLILITRE,

        RADIAN,
        STERADIAN,
        METRE,
        MILE,
        LIGHT_YEAR,
        POINT,
        KNOT,
        FOOT,
        ANGSTROM,
        INCH,
        PARSEC,
        FOOT_SURVEY_US,
        ASTRONOMICAL_UNIT,
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

        KILOMETERS_PER_HOUR,
        MILES_PER_HOUR,
        MACH,
        SPEED_OF_LIGHT,
        METER_PER_SECOND,
        // KNOT,

        // STEPS,

        TEXT,
        NUMERIC,
        BOOLEAN,

        SECOND,
        MINUTE,
        YEAR_SIDEREAL,
        HOUR,
        MONTH,
        YEAR_CALENDAR,
        DAY_SIDEREAL,
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

        FARAD,
        HENRY,
        JOULE,
        SIEMENS,
        OHM,
        FRANKLIN,
        ELECTRON_VOLT,
        ELECTRIC_CHARGE,
        FARADAY,
        GILBERT,

        NEWTON,
        POUND_FORCE,
        // GRAVITY,
        KILOGRAM_FORCE,
        DYNE,

        GRAY,
        ENERGY,

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
        ARE
    };

    ReadingType() = default;
    ReadingType(DataType dataType);
    ReadingType(ReadingType::Name name, ReadingType::MeasurmentUnit unit);
    ReadingType(std::string name, std::string unitSymbol, DataType dataType, int precision,
                std::vector<std::string> labels);

    const std::string& getName() const;

    const std::string& getMeasurmentUnit() const;

    const std::string& getMeasurmentUnitSymbol() const;

    DataType getDataType() const;

    int getPrecision() const;

    const std::vector<std::string>& getLabels() const;

    size_t getSize() const;

    const std::string& getDelimiter() const;

    bool operator==(ReadingType& rhs) const;
    bool operator!=(ReadingType& rhs) const;

private:
    std::string m_name;
    std::string m_unit;
    std::string m_unitSymbol;

    DataType m_dataType;
    int m_precision;
    std::vector<std::string> m_labels;
    size_t m_size;

    std::string m_delimiter;

    void validate(ReadingType::Name name, ReadingType::MeasurmentUnit unit);
    DataType dataTypeForName(ReadingType::Name name);
    std::vector<std::string> labelsForName(ReadingType::Name name);
    std::string symbolForUnit(ReadingType::MeasurmentUnit unit);
    // ReadingType::Name nameForUnit(ReadingType::MeasurmentUnit unit);

    static const std::string DEFAULT_DELIMITER;
};

std::string toString(ReadingType::Name name);
std::string toString(ReadingType::MeasurmentUnit unit);
}    // namespace wolkabout

#endif    // READINGTYPE_H

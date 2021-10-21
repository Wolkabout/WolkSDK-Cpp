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

#ifndef ACTUATIONREADINGTYPE_H
#define ACTUATIONREADINGTYPE_H

#include "core/model/DataType.h"

#include <string>

namespace wolkabout
{
class ActuationReadingType
{
public:
    enum class Name
    {
        COUNT = 0,
        STRING,
        SWITCH
    };

    enum class MeasurmentUnit
    {
        BOOLEAN,
        STEPS,
        COUNT,
        TEXT
    };

    ActuationReadingType() = default;
    ActuationReadingType(DataType dataType);
    ActuationReadingType(ActuationReadingType::Name name, ActuationReadingType::MeasurmentUnit unit);
    ActuationReadingType(std::string name, std::string unitSymbol);

    const std::string& getName() const;

    const std::string& getMeasurmentUnit() const;

    const std::string& getMeasurmentUnitSymbol() const;

    bool operator==(const ActuationReadingType& rhs) const;
    bool operator!=(const ActuationReadingType& rhs) const;

private:
    std::string m_name;
    std::string m_unit;
    std::string m_unitSymbol;

    void validate(ActuationReadingType::Name name, ActuationReadingType::MeasurmentUnit unit) const;
    std::string symbolForUnit(ActuationReadingType::MeasurmentUnit unit) const;
};

std::string toString(ActuationReadingType::Name name);
std::string toString(ActuationReadingType::MeasurmentUnit unit);
}    // namespace wolkabout

#endif    // ACTUATIONREADINGTYPE_H

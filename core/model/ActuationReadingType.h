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

#ifndef ACTUATIONREADINGTYPE_H
#define ACTUATIONREADINGTYPE_H

#include "model/DataType.h"

#include <string>
#include <vector>

namespace wolkabout
{
class ActuationReadingType
{
public:
    enum class Name
    {
        COUNT = 0,
        SWITCH,
        STRING
    };

    enum class MeasurmentUnit
    {
        COUNT = 0,
        STEPS,
        BOOLEAN,
        TEXT
    };

    ActuationReadingType() = default;
    ActuationReadingType(DataType dataType);
    ActuationReadingType(ActuationReadingType::Name name, ActuationReadingType::MeasurmentUnit unit);
    ActuationReadingType(std::string name, std::string unitSymbol, DataType dataType, int precision,
                         std::vector<std::string> labels);

    const std::string& getName() const;

    const std::string& getMeasurmentUnit() const;

    const std::string& getMeasurmentUnitSymbol() const;

    DataType getDataType() const;

    int getPrecision() const;

    const std::vector<std::string>& getLabels() const;

    size_t getSize() const;

    const std::string& getDelimiter() const;

    bool operator==(ActuationReadingType& rhs) const;
    bool operator!=(ActuationReadingType& rhs) const;

private:
    std::string m_name;
    std::string m_unit;
    std::string m_unitSymbol;

    DataType m_dataType;
    int m_precision;
    std::vector<std::string> m_labels;
    size_t m_size;

    std::string m_delimiter;

    void validate(ActuationReadingType::Name name, ActuationReadingType::MeasurmentUnit unit) const;
    DataType dataTypeForName(ActuationReadingType::Name name) const;
    std::string symbolForUnit(ActuationReadingType::MeasurmentUnit unit) const;

    static const std::string DEFAULT_DELIMITER;
};

std::string toString(ActuationReadingType::Name name);
std::string toString(ActuationReadingType::MeasurmentUnit unit);
}    // namespace wolkabout

#endif    // ACTUATIONREADINGTYPE_H

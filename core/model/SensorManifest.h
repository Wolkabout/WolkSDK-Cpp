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

#ifndef SENSORMANIFEST_H
#define SENSORMANIFEST_H

#include "model/DataType.h"
#include "model/ReadingType.h"

#include <string>
#include <vector>

namespace wolkabout
{
class SensorManifest
{
public:
    SensorManifest() = default;

    SensorManifest(std::string name, std::string reference, DataType dataType, std::string description,
                   double minimum = 0, double maximum = 0);

    SensorManifest(std::string name, std::string reference, ReadingType readingType, std::string description,
                   double minimum = 0, double maximum = 0);

    SensorManifest(std::string name, std::string reference, ReadingType::Name readingTypeName,
                   ReadingType::MeasurmentUnit unit, std::string description, double minimum = 0, double maximum = 0);

    SensorManifest(std::string name, std::string reference, std::string readingTypeName, std::string unitSymbol,
                   DataType dataType, int precision, std::string description, std::vector<std::string> labels,
                   double minimum = 0, double maximum = 0);

    const std::string& getName() const;

    const std::string& getReference() const;

    const std::string& getReadingTypeName() const;

    const std::string& getUnitSymbol() const;

    const std::string& getDescription() const;

    DataType getDataType() const;

    int getPrecision() const;

    double getMinimum() const;

    double getMaximum() const;

    const std::string& getDelimiter() const;

    const std::vector<std::string>& getLabels() const;

    size_t getSize() const;

    bool operator==(SensorManifest& rhs) const;
    bool operator!=(SensorManifest& rhs) const;

private:
    std::string m_name;
    std::string m_reference;
    ReadingType m_readingType;
    std::string m_description;

    double m_minimum;
    double m_maximum;
};
}    // namespace wolkabout

#endif    // SENSORMANIFEST_H

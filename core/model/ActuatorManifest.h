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

#ifndef ACTUATORMANIFEST_H
#define ACTUATORMANIFEST_H

#include "model/ActuationReadingType.h"
#include "model/DataType.h"

#include <string>
#include <vector>

namespace wolkabout
{
class ActuatorManifest
{
public:
    ActuatorManifest() = default;

    ActuatorManifest(std::string name, std::string reference, DataType dataType, std::string description,
                     double minimum = 0, double maximum = 0);

    ActuatorManifest(std::string name, std::string reference, ActuationReadingType readingType, std::string description,
                     double minimum = 0, double maximum = 0);

    ActuatorManifest(std::string name, std::string reference, ActuationReadingType::Name readingTypeName,
                     ActuationReadingType::MeasurmentUnit unit, std::string description, double minimum = 0,
                     double maximum = 0);

    ActuatorManifest(std::string name, std::string reference, std::string readingTypeName, std::string unitSymbol,
                     DataType dataType, int precision, std::string description, std::vector<std::string> labels,
                     double minimum = 0, double maximum = 0);

    const std::string& getName() const;

    const std::string& getReference() const;

    const std::string& getDescription() const;

    const std::string& getUnitSymbol() const;

    const std::string& getReadingTypeName() const;

    DataType getDataType() const;

    int getPrecision() const;

    double getMinimum() const;

    double getMaximum() const;

    const std::string& getDelimiter() const;

    const std::vector<std::string>& getLabels() const;

    size_t getSize() const;

    bool operator==(ActuatorManifest& rhs) const;
    bool operator!=(ActuatorManifest& rhs) const;

private:
    std::string m_name;
    std::string m_reference;
    ActuationReadingType m_readingType;
    std::string m_description;

    double m_minimum;
    double m_maximum;
};
}    // namespace wolkabout

#endif    // ACTUATORMANIFEST_H

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

#ifndef CONFIGMANIFEST_H
#define CONFIGMANIFEST_H

#include "model/DataType.h"

#include <string>
#include <vector>

namespace wolkabout
{
class ConfigurationManifest
{
public:
    ConfigurationManifest() = default;
    ConfigurationManifest(std::string name, std::string reference, DataType dataType, std::string description,
                          std::string defaultValue, double minimum = 0, double maximum = 0);

    ConfigurationManifest(std::string name, std::string reference, DataType dataType, std::string description,
                          std::string defaultValue, std::vector<std::string> labels, double minimum = 0,
                          double maximum = 0);

    const std::string& getName() const;

    const std::string& getReference() const;

    DataType getDataType() const;

    const std::string& getDescription() const;

    const std::string& getDefaultValue() const;

    double getMinimum() const;

    double getMaximum() const;

    const std::vector<std::string>& getLabels() const;

    size_t getSize() const;

    const std::string& getDelimiter() const;

    bool operator==(ConfigurationManifest& rhs) const;
    bool operator!=(ConfigurationManifest& rhs) const;

private:
    std::string m_name;
    std::string m_reference;
    DataType m_dataType;
    std::string m_description;
    std::string m_defaultValue;

    double m_minimum;
    double m_maximum;

    std::vector<std::string> m_labels;
    size_t m_size;

    std::string m_delimiter;

    static const std::string DEFAULT_DELIMITER;
};
}    // namespace wolkabout

#endif    // CONFIGMANIFEST_H

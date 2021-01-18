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

#ifndef CONFIGURATIONTEMPLATE_H
#define CONFIGURATIONTEMPLATE_H

#include "model/DataType.h"
#include "model/WolkOptional.h"

#include <string>
#include <vector>

namespace wolkabout
{
class ConfigurationTemplate
{
public:
    ConfigurationTemplate() = default;

    ConfigurationTemplate(std::string name, std::string reference, DataType dataType, std::string description,
                          std::string defaultValue, std::vector<std::string> labels = {});

    const std::string& getName() const;

    const std::string& getReference() const;

    DataType getDataType() const;

    const std::string& getDescription() const;

    const std::string& getDefaultValue() const;

    const std::vector<std::string>& getLabels() const;

    size_t getSize() const;

    bool operator==(const ConfigurationTemplate& rhs) const;
    bool operator!=(const ConfigurationTemplate& rhs) const;

private:
    std::string m_name;
    std::string m_reference;
    DataType m_dataType;
    std::string m_description;
    std::string m_defaultValue;

    std::vector<std::string> m_labels;
    size_t m_size;
};
}    // namespace wolkabout

#endif    // CONFIGURATIONTEMPLATE_H

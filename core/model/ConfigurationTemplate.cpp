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

#include "ConfigurationTemplate.h"

#include <cmath>
#include <stdexcept>
#include <utility>

namespace wolkabout
{
ConfigurationTemplate::ConfigurationTemplate(std::string name, std::string reference, DataType dataType,
                                             std::string description, std::string defaultValue, double minimum,
                                             double maximum)
: ConfigurationTemplate(name, reference, dataType, description, defaultValue, {}, minimum, maximum)
{
}

ConfigurationTemplate::ConfigurationTemplate(std::string name, std::string reference, DataType dataType,
                                             std::string description, std::string defaultValue,
                                             std::vector<std::string> labels, double minimum, double maximum)
: m_name{std::move(name)}
, m_reference{std::move(reference)}
, m_dataType{dataType}
, m_description{std::move(description)}
, m_defaultValue{std::move(defaultValue)}
, m_minimum{minimum}
, m_maximum{maximum}
, m_labels{std::move(labels)}
, m_size{m_labels.size() == 0 ? 1 : m_labels.size()}
{
}

const std::string& ConfigurationTemplate::getName() const
{
    return m_name;
}

const std::string& ConfigurationTemplate::getReference() const
{
    return m_reference;
}

DataType ConfigurationTemplate::getDataType() const
{
    return m_dataType;
}

double ConfigurationTemplate::getMinimum() const
{
    return m_minimum;
}

double ConfigurationTemplate::getMaximum() const
{
    return m_maximum;
}

const std::string& ConfigurationTemplate::getDescription() const
{
    return m_description;
}

const std::string& ConfigurationTemplate::getDefaultValue() const
{
    return m_defaultValue;
}

size_t ConfigurationTemplate::getSize() const
{
    return m_size;
}

const std::vector<std::string>& ConfigurationTemplate::getLabels() const
{
    return m_labels;
}

const std::string& ConfigurationTemplate::getDelimiter() const
{
    return m_delimiter;
}

bool ConfigurationTemplate::operator==(ConfigurationTemplate& rhs) const
{
    if (m_name != rhs.m_name || m_reference != rhs.m_reference || m_description != rhs.m_description ||
        m_dataType != rhs.m_dataType)
    {
        return false;
    }

    if (m_minimum != rhs.m_minimum || m_maximum != rhs.m_maximum)
    {
        return false;
    }

    if (m_size != rhs.m_size || m_delimiter != rhs.m_delimiter)
    {
        return false;
    }

    if (m_labels.size() != rhs.m_labels.size())
    {
        return false;
    }

    for (size_t i = 0; i < m_labels.size(); ++i)
    {
        if (m_labels[i] != rhs.m_labels[i])
        {
            return false;
        }
    }

    return true;
}

bool ConfigurationTemplate::operator!=(ConfigurationTemplate& rhs) const
{
    return !(*this == rhs);
}

}    // namespace wolkabout

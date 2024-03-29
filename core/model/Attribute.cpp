/**
 * Copyright 2022 Wolkabout Technology s.r.o.
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

#include "core/model/Attribute.h"

#include <stdexcept>
#include <utility>

namespace wolkabout
{
Attribute::Attribute(std::string name, DataType dataType, std::string value)
: m_name(std::move(name)), m_dataType(dataType), m_value(std::move(value))
{
    if (m_dataType < DataType::STRING || m_dataType > DataType::VECTOR)
        throw std::runtime_error("Failed to create Attribute with invalid DataType value.");
}

const std::string& Attribute::getName() const
{
    return m_name;
}

DataType Attribute::getDataType() const
{
    return m_dataType;
}

const std::string& Attribute::getValue() const
{
    return m_value;
}
}    // namespace wolkabout

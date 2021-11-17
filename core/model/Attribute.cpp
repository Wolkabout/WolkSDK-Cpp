/*
 * Copyright 2021 Adriateh d.o.o.
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

namespace wolkabout
{
Attribute::Attribute(std::string name, DataType dataType, std::string value)
: m_name(std::move(name)), m_dataType(dataType), m_value(std::move(value))
{
}

const std::string& Attribute::getName() const
{
    return m_name;
}

void Attribute::setName(const std::string& name)
{
    m_name = name;
}

DataType Attribute::getDataType() const
{
    return m_dataType;
}

void Attribute::setDataType(DataType dataType)
{
    m_dataType = dataType;
}

std::string Attribute::getValue() const
{
    return m_value;
}

void Attribute::setValue(std::string value)
{
    m_value = std::move(value);
}
}    // namespace wolkabout

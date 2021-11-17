/**
 * Copyright 2021 WolkAbout Technology s.r.o.
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

#ifndef WOLKABOUTCORE_ATTRIBUTE_H
#define WOLKABOUTCORE_ATTRIBUTE_H

#include "core/Types.h"

#include <string>
#include <utility>

namespace wolkabout
{
class Attribute
{
public:
    Attribute(std::string name, DataType dataType, std::string value);

    const std::string& getName() const;

    void setName(const std::string& name);

    DataType getDataType() const;

    void setDataType(DataType dataType);

    std::string getValue() const;

    void setValue(std::string value);

private:
    std::string m_name;
    DataType m_dataType;
    std::string m_value;
};
}    // namespace wolkabout
#endif    // WOLKABOUTCORE_ATTRIBUTE_H

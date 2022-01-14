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

#ifndef WOLKABOUTCORE_ATTRIBUTE_H
#define WOLKABOUTCORE_ATTRIBUTE_H

#include "core/Types.h"

namespace wolkabout
{
/**
 * This class represents an attribute model that should be used to register/update attributes for devices.
 * This intakes all the necessary information to register an attribute for a device.
 */
class Attribute
{
public:
    /**
     * Default constructor for this model class.
     *
     * @param name The name of the attribute.
     * @param dataType The data type of the attribute.
     * @param value The value currently assigned to the attribute.
     */
    Attribute(std::string name, DataType dataType, std::string value);

    /**
     * Default getter for the name of the attribute.
     *
     * @return The name of the attribute as a string.
     */
    const std::string& getName() const;

    /**
     * Default getter for the data type of the attribute.
     *
     * @return The data type of the attribute as an enum value.
     */
    DataType getDataType() const;

    /**
     * Default getter for the current value of the attribute.
     *
     * @return The value of the attribute as a string.
     */
    const std::string& getValue() const;

private:
    std::string m_name;
    DataType m_dataType;
    std::string m_value;
};
}    // namespace wolkabout
#endif    // WOLKABOUTCORE_ATTRIBUTE_H

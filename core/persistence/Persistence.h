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

#ifndef PERSISTENCE_H
#define PERSISTENCE_H

#include "core/Types.h"
#include "core/model/Attribute.h"
#include "core/model/Feed.h"
#include "core/model/Reading.h"

#include <map>
#include <memory>
#include <string>
#include <vector>

namespace wolkabout
{
/**
 * @brief A storage designed for holding elements in persistent store prior to
 * publishing to WolkAbout IoT Platform.
 *
 * Multiple Readings can be stored under the same key.
 *
 * Implementation storing/retrieving strategy must be FIFO.
 */
class Persistence
{
public:
    /**
     * @brief Destructor
     */
    virtual ~Persistence() = default;

    /**
     * @brief Inserts the wolkabout::Reading
     *
     * @param key     with which wolkabout::Reading should be associated
     * @param reading to be inserted
     * @return {@code true} if successful, or {@code false} if
     * element can not be inserted
     */
    virtual bool putReading(const std::string& key, std::shared_ptr<Reading> reading) = 0;

    /**
     * @brief Retrieves, first {@code count} wolkabout::Readings of this
     * storage, associated with given {@code key} or returns empty {@code
     * std::vector<std::shared_ptr<Reading>>} if this storage is empty.
     *
     * @param key   of the wolkabout::Readings
     * @param count number of items to peek
     * @return {@code std::vector<std::shared_ptr<Reading>>} containing
     * {@code count} wolkabout::Readings starting from the head, or returns
     * less than {@code count} wolkabout::Readings if this storage does not
     * have requested number of elements
     */
    virtual std::vector<std::shared_ptr<Reading>> getReadings(const std::string& key, std::uint_fast64_t count) = 0;

    /**
     * @brief Removes first {@code count} wolkabout::Readings of this
     * storage, associated with given {@code key}.
     *
     * @param key   of the wolkabout::Readings
     * @param count number of items to remove
     */
    virtual void removeReadings(const std::string& key, std::uint_fast64_t count) = 0;

    /**
     * Returns {@code std::vector<std::string>>} of wolkabout::Readings keys
     * contained in this storage.
     *
     * @return {@code std::vector<std::string>} containing keys, or empty {@code
     * std::vector<std::string>>} if no wolkabout::Readings are present.
     */
    virtual std::vector<std::string> getReadingsKeys() = 0;

    /**
     * @brief Inserts the device attribute.
     *
     * @param attribute  to be inserted
     * @return {@code true} if successful, or {@code false} if
     * element can not be inserted
     */
    virtual bool putAttribute(std::shared_ptr<Attribute> attribute) = 0;

    /**
     * @brief Retrieves device attributes contained in this storage.
     *
     * @return Device attribute as {@code std::vector<std::shared_ptr<Attribute>>}, or {@code nullptr}
     * if this storage does not contain persisted device attributes
     */
    virtual std::vector<std::shared_ptr<Attribute>> getAttributes() = 0;

    /**
     * @brief Removes device attributes from this storage.
     */
    virtual void removeAttributes() = 0;

    virtual bool putParameter(Parameter parameter) = 0;

    virtual std::map<ParameterName, std::string> getParameters() = 0;

    virtual void removeParameters(ParameterName parameterName) = 0;

    /**
     * Returns {@code true} if this storage contains no wolkabout::Readings,
     * wolkabout::Attributes associated with any key.
     *
     * @return {@code true} if this storage contains no wolkabout::Readings,
     * wolkabout::Attributes associated with any key
     */
    virtual bool isEmpty() = 0;
};
}    // namespace wolkabout

#endif

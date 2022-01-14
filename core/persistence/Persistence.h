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
 * publishing to Wolkabout IoT Platform.
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
    virtual bool putReading(const std::string& key, const Reading& reading) = 0;

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
     * @param key of the wolkabout::Attribute
     * @param attribute  to be inserted
     * @return {@code true} if successful, or {@code false} if
     * element can not be inserted
     */
    virtual bool putAttribute(const std::string& key, std::shared_ptr<Attribute> attribute) = 0;

    /**
     * @brief Retrieves device attributes contained in this storage.
     *
     * @return Device attribute as {@code std::map<std::string, std::shared_ptr<Attribute>>}, or an empty map
     * if this storage does not contain persisted device attributes
     */
    virtual std::map<std::string, std::shared_ptr<Attribute>> getAttributes() = 0;

    /**
     * @brief Retrieves device attribute contained in this storage.
     *
     * @param key The key of the attribute which should be obtained.
     * @return Device attribute as {@code std::shared_ptr<Attribute>}, or a {@code nullptr} if an attribute with the key
     * couldn't be found.
     */
    virtual std::shared_ptr<Attribute> getAttributeUnderKey(const std::string& key) = 0;

    /**
     * @brief Removes device attributes from this storage.
     */
    virtual void removeAttributes() = 0;

    /**
     * @brief Removes device attributes from this storage.
     *
     * @param key Key filter that can be used to delete a specific attribute.
     */
    virtual void removeAttributes(const std::string& key) = 0;

    /**
     * Returns {@code std::vector<std::string>>} of wolkabout::Attribute keys contained in this storage.
     *
     * @return {@code std::vector<std::string>>} containing keys, or empty {@code std::vector<std::string>>} if no
     * wolkabout::Attributes are present.
     */
    virtual std::vector<std::string> getAttributeKeys() = 0;

    /**
     * @brief Inserts the device parameter.
     *
     * @param key of the wolkabout::Parameter
     * @param parameter to be inserted
     * @return {@code true} if successful, or {@code false} if element can not be inserted
     */
    virtual bool putParameter(const std::string& key, Parameter parameter) = 0;

    /**
     * @brief Retrieves device parameters contained in this storage.
     *
     * @return Device parameters as {@code std::map<std::string, wolkabout::Parameter>}, or an empty map if this storage
     * does not contain persisted device parameters.
     */
    virtual std::map<std::string, Parameter> getParameters() = 0;

    /**
     * @brief Retrieves device parameters contained in this storage.
     *
     * @param key The key of the parameter which should be obtained.
     * @return Device parameter as {@code Parameter}, or a {@code {}} if a parameter with the key couldn't be found.
     */
    virtual Parameter getParameterForKey(const std::string& key) = 0;

    /**
     * @brief Removes device parameters from the storage.
     */
    virtual void removeParameters() = 0;

    /**
     * @brief Removes device parameters from the storage.
     *
     * @param key Key filter that can be used to delete a specific parameter.
     */
    virtual void removeParameters(const std::string& key) = 0;

    /**
     * Returns {@code std::vector<std::string>} of wolkabout::Parameter keys contained in this storage.
     *
     * @return {@code std::vector<std::string>>} containing keys, or empty {@code std::vector<std::string>>] if no
     * wolkabout::Parameters are present.
     */
    virtual std::vector<std::string> getParameterKeys() = 0;

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

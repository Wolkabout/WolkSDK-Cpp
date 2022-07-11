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

#ifndef WOLKABOUTCORE_READING_H
#define WOLKABOUTCORE_READING_H

#include "core/Types.h"

#include <cmath>

namespace wolkabout
{
/**
 * This class represents a single Reading. A reading is a value for a feed at a certain time.
 * Reference is used to notify for which feed the reading is defined, the value(s) are the values that are active for
 * the set timestamp. The Reading class stores the values as a string, as string is the only way a value of any type can
 * be kept, and a vector of such values since multi-value feeds are also a thing.
 */
class Reading
{
public:
    /**
     * This is the default single value constructor for a reading.
     * This assumes your feed is using a single value string feed.
     *
     * @param reference The reference of the feed for this reading.
     * @param value The value of this reading.
     * @param rtcTimestamp The time at which this reading occurred. Optional.
     */
    Reading(std::string reference, std::string value, std::uint64_t rtcTimestamp = 0);

    /**
     * This is the default single value constructor for a reading.
     * This assumes your feed is using a single value numeric feed.
     *
     * @param reference The reference of the feed for this reading.
     * @param value The value of this reading.
     * @param rtcTimestamp The time at which this reading occurred. Optional.
     */
    Reading(std::string reference, std::uint64_t value, std::uint64_t rtcTimestamp = 0);

    /**
     * This is the default single value constructor for a reading.
     * This assumes your feed is using a single value numeric feed.
     *
     * @param reference The reference of the feed for this reading.
     * @param value The value of this reading.
     * @param rtcTimestamp The time at which this reading occurred. Optional.
     */
    Reading(std::string reference, std::int64_t value, std::uint64_t rtcTimestamp = 0);

    /**
     * This is the default single value constructor for a reading.
     * This assumes your feed is using a single value numeric feed.
     *
     * @param reference The reference of the feed for this reading.
     * @param value The value of this reading.
     * @param rtcTimestamp The time at which this reading occurred. Optional.
     */
    Reading(std::string reference, std::double_t value, std::uint64_t rtcTimestamp = 0);

    /**
     * This is the default single value constructor for a reading.
     * This assumes your feed is using a single value boolean feed.
     *
     * @param reference The reference of the feed for this reading.
     * @param value The value of this reading.
     * @param rtcTimestamp The time at which this reading occurred. Optional.
     */
    Reading(std::string reference, bool value, std::uint64_t rtcTimestamp = 0);

    /**
     * This is the reading constructor that intakes the Location.
     * Location is the only multi-value reading that is represented as a single value, so this class handles such
     * readings differently.
     *
     * @param reference The reference of the feed for this reading.
     * @param value The value of this reading.
     * @param rtcTimestamp The time at which this reading occurred. Optional.
     */
    Reading(std::string reference, Location location, std::uint64_t rtcTimestamp = 0);

    /**
     * This is the reading constructor that intakes multiple values.
     * This is for feeds that are defined as vectors.
     *
     * @param reference The reference of the feed for this reading.
     * @param values The list of numeric values for this feed.
     * @param rtcTimestamp The time at which this reading occurred. Optional.
     */
    Reading(std::string reference, std::vector<std::string> values, std::uint64_t rtcTimestamp = 0);

    /**
     * This is the reading constructor that intakes multiple numeric values that are unsigned integers.
     * This is for feeds that are defined as vectors.
     *
     * @param reference The reference of the feed for this reading.
     * @param values The list of numeric values for this feed.
     * @param rtcTimestamp The time at which this reading occurred. Optional.
     */
    Reading(std::string reference, const std::vector<std::uint64_t>& values, std::uint64_t rtcTimestamp = 0);

    /**
     * This is the reading constructor that intakes multiple numeric values that are integers.
     * This is for feeds that are defined as vectors.
     *
     * @param reference The reference of the feed for this reading.
     * @param values The list of numeric values for this feed.
     * @param rtcTimestamp The time at which this reading occurred. Optional.
     */
    Reading(std::string reference, const std::vector<std::int64_t>& values, std::uint64_t rtcTimestamp = 0);

    /**
     * This is the reading constructor that intakes multiple numeric values that are doubles.
     * This is for feeds that are defined as vectors.
     *
     * @param reference The reference of the feed for this reading.
     * @param values The list of numeric values for this feed.
     * @param rtcTimestamp The time at which this reading occurred. Optional.
     */
    Reading(std::string reference, const std::vector<std::double_t>& values, std::uint64_t rtcTimestamp = 0);

    /**
     * Default getter for the reference. The reference is for the feed this reading is regarding.
     *
     * @return The reference of the feed as a string.
     */
    const std::string& getReference() const;

    /**
     * Default getter for the information if the reading is carrying multiple values or not.
     *
     * @details If the reading is single value (when this getter returns false), then the next getters are potentially
     * available, and will not throw an exception when invoked:
     *  - getStringValue()
     *  - getUIntValue() - If the value can be parsed as an unsigned integer.
     *  - getIntValue() - If the value can be parsed as an integer.
     *  - getDoubleValue() - If the value can be parsed as a double.
     *  - getBooleanValue() - If the value can be parsed as a boolean ("true", "false")
     *  - getHexValue() - If the value can be parsed as a hexadecimal value
     *  - getLocation() - If the value can be parsed as a location. Location strings are "<LATITUDE>,<LONGITUDE>", where
     * <LATITUDE> is a double between -90 and 90, and <LONGITUDE> is a double between -180 and 180
     *          If the reading is multi value (when this getter returns true), then the next getter are potentially
     * available, and will not throw an exception when invoked:
     *  - getStringValues()
     *  - getUIntValues() - If each value is an unsigned integer.
     *  - getIntValues() - If each value is an integer.
     *  - getDoubleValues() - If each value is a double.
     *
     * @return Whether the reading is single or multi-valued. `true` = multi-value, `false` = single-value.
     */
    bool isMulti() const;

    /**
     * This is the default getter for the single value this reading holds as a string. This string may be interpreted
     * into other types, such as Numeric, Boolean, Hexadecimal, Location or Enum.
     *
     * @return The single value of the reading as a string.
     */
    const std::string& getStringValue() const;

    /**
     * This is a getter to ask if the single value this reading holds can be parsed into an unsigned integer.
     *
     * @return Whether the value can be parsed into an unsigned integer.
     */
    bool isUInt() const;

    /**
     * This is the default getter for the single value this reading holds as an unsigned integer.
     *
     * @throw std::invalid_argument If the value can not be read as an unsigned integer.
     * @throw std::out_of_range If the value is out of range for an unsigned integer.
     *
     * @return The single value of the reading as an unsigned integer.
     */
    std::uint64_t getUIntValue() const;

    /**
     * This is a getter to ask if the single value this reading holds can be parsed into an integer.
     *
     * @return Whether the value can be parsed into an integer.
     */
    bool isInt() const;

    /**
     * This is the default getter for the single value this reading holds as an integer.
     *
     * @throw std::invalid_argument If the value can not be read as an integer.
     * @throw std::out_of_range If the value is out of range for an integer.
     *
     * @return The single value of the reading as an integer.
     */
    std::int64_t getIntValue() const;

    /**
     * This is a getter to ask if the single value this reading holds can be parsed into a double.
     *
     * @return Whether the value can be parsed into a double.
     */
    bool isDouble() const;

    /**
     * This is the default getter for the single value this reading holds as a double.
     *
     * @throw std::invalid_argument If the value can not be read as a double.
     * @throw std::out_of_range If the value is out of range for a double.
     *
     * @return The single value of the reading as a double.
     */
    std::double_t getDoubleValue() const;

    /**
     * This is a getter to ask if the single value this reading holds can be parsed into a boolean.
     *
     * @return Whether the value can be parsed into a boolean.
     */
    bool isBoolean() const;

    /**
     * This is the default getter for the single value this reading holds as a boolean.
     *
     * @throw std::out_of_range If the value is neither true nor false.
     *
     * @return The single value of the reading as a boolean.
     */
    bool getBoolValue() const;

    /**
     * This is the default getter for the single value this reading holds as an integer, but interpreted as hex.
     *
     * @throw std::invalid_argument If the value is not a hexadecimal value.
     *
     * @return The single value of the reading read as hexadecimal.
     */
    std::uint64_t getHexValue() const;

    /**
     * This is the default getter for the location value this reading holds.
     *
     * @throw std::invalid_argument If the value does not follow the format: "<LATITUDE>,<LONGITUDE>" where <LATITUDE>
     * is a double between -90 and 90, and <LONGITUDE> is a double between -180 and 180.
     *
     * @return The location value of th reading.
     */
    Location getLocationValue() const;

    /**
     * This is the default getter for the vector of string values this reading holds.
     *
     * @return Vector containing raw string values.
     */
    std::vector<std::string> getStringValues() const;

    /**
     * This is the default getter for the vector of unsigned integer values this reading holds.
     *
     * @throw std::invalid_argument If any of the values inside the vector can not be parsed as an unsigned integer.
     * @throw std::out_of_range If any of the values inside the vector are not in range to be an unsigned integer.
     *
     * @return Vector containing multiple unsigned integer values.
     */
    std::vector<std::uint64_t> getUIntValues() const;

    /**
     * This is the default getter for the vector of integer values this reading holds.
     *
     * @throw std::invalid_argument If any of the values inside the vector can not be parsed as an integer.
     * @throw std::out_of_range If any of the values inside the vector are not in range to be an integer.
     *
     * @return Vector containing multiple integer values.
     */
    std::vector<std::int64_t> getIntValues() const;

    /**
     * This is the default getter for the vector of double values this reading holds.
     *
     * @throw std::invalid_argument If any of the values inside the vector can not be parsed as a double.
     * @throw std::out_of_range If any of the values inside the vector are not in range to be a double.
     *
     * @return Vector containing multiple double values.
     */
    std::vector<std::double_t> getDoubleValues() const;

    /**
     * This is the default getter for the timestamp. This is the time at which the reading was taken.
     *
     * @return The timestamp as the unix epoch time in milliseconds.
     */
    const std::uint64_t& getTimestamp() const;

    /**
     * This is the default setter for the timestamp.
     *
     * @param timestamp The timestamp as the unix epoch time in milliseconds.
     */
    void setTimestamp(std::uint64_t timestamp);

private:
    // Here is the collection of values regarding this reading
    std::string m_reference;
    std::vector<std::string> m_values;
    std::uint64_t m_timestamp;
};
}    // namespace wolkabout

#endif    // WOLKABOUTCORE_READING_H

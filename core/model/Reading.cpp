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

#include "core/model/Reading.h"

#include <algorithm>
#include <regex>
#include <sstream>
#include <stdexcept>
#include <utility>

namespace wolkabout
{
// This is the divider that is used to put the latitude and longitude of a location together.
const std::string LOCATION_DIVIDER = ",";

// This is a collection of regex's that can be used to quickly determine if a value can be parsed into any of the types.
const std::string UNSIGNED_REGEX = "\\d+";
const std::string INTEGER_REGEX = "-?\\d+";
const std::string DOUBLE_REGEX = "-?\\d+.\\d+";
const std::string BOOLEAN_REGEX = "(true|false)";

Reading::Reading(std::string reference, std::string value, std::uint64_t rtcTimestamp)
: m_reference(std::move(reference)), m_values({std::move(value)}), m_timestamp(rtcTimestamp)
{
}

Reading::Reading(std::string reference, std::uint64_t value, std::uint64_t rtcTimestamp)
: m_reference(std::move(reference)), m_values({std::to_string(value)}), m_timestamp(rtcTimestamp)
{
}

Reading::Reading(std::string reference, std::int64_t value, std::uint64_t rtcTimestamp)
: m_reference(std::move(reference)), m_values({std::to_string(value)}), m_timestamp(rtcTimestamp)
{
}

Reading::Reading(std::string reference, std::double_t value, std::uint64_t rtcTimestamp)
: m_reference(std::move(reference)), m_values({std::to_string(value)}), m_timestamp(rtcTimestamp)
{
}

Reading::Reading(std::string reference, bool value, std::uint64_t rtcTimestamp)
: m_reference(std::move(reference)), m_values({value ? "true" : "false"}), m_timestamp(rtcTimestamp)
{
}

Reading::Reading(std::string reference, Location location, std::uint64_t rtcTimestamp)
: m_reference(std::move(reference))
, m_values({std::to_string(location.latitude) + LOCATION_DIVIDER + std::to_string(location.longitude)})
, m_timestamp(rtcTimestamp)
{
}

Reading::Reading(std::string reference, std::vector<std::string> values, std::uint64_t rtcTimestamp)
: m_reference(std::move(reference)), m_values(std::move(values)), m_timestamp(rtcTimestamp)
{
}

Reading::Reading(std::string reference, const std::vector<std::uint64_t>& values, std::uint64_t rtcTimestamp)
: m_reference(std::move(reference)), m_timestamp(rtcTimestamp)
{
    for (const auto& value : values)
        m_values.emplace_back(std::to_string(value));
}

Reading::Reading(std::string reference, const std::vector<std::int64_t>& values, std::uint64_t rtcTimestamp)
: m_reference(std::move(reference)), m_timestamp(rtcTimestamp)
{
    for (const auto& value : values)
        m_values.emplace_back(std::to_string(value));
}

Reading::Reading(std::string reference, const std::vector<std::double_t>& values, std::uint64_t rtcTimestamp)
: m_reference(std::move(reference)), m_timestamp(rtcTimestamp)
{
    for (const auto& value : values)
        m_values.emplace_back(std::to_string(value));
}

const std::string& Reading::getReference() const
{
    return m_reference;
}

bool Reading::isMulti() const
{
    return m_values.size() > 1;
}

const std::string& Reading::getStringValue() const
{
    return m_values.front();
}

bool Reading::isUInt() const
{
    return std::regex_match(m_values.front(), std::regex(UNSIGNED_REGEX));
}

std::uint64_t Reading::getUIntValue() const
{
    return std::stoull(m_values.front());
}

bool Reading::isInt() const
{
    return std::regex_match(m_values.front(), std::regex(INTEGER_REGEX));
}

std::int64_t Reading::getIntValue() const
{
    return std::stoll(m_values.front());
}

bool Reading::isDouble() const
{
    return std::regex_match(m_values.front(), std::regex(DOUBLE_REGEX));
}

std::double_t Reading::getDoubleValue() const
{
    return std::stod(m_values.front());
}

bool Reading::isBoolean() const
{
    return std::regex_match(m_values.front(), std::regex(BOOLEAN_REGEX, std::regex_constants::icase));
}

bool Reading::getBoolValue() const
{
    // Make the string lower case
    auto value = m_values.front();
    std::transform(value.cbegin(), value.cend(), value.begin(), ::tolower);

    // Now check the value
    if (value == "true")
        return true;
    else if (value == "false")
        return false;
    else
        throw std::invalid_argument("Reading does not contain a valid bool value.");
}

std::uint64_t Reading::getHexValue() const
{
    // Set up the string stream
    auto input = std::istringstream(m_values.front());

    // Try to read with `std::hex`
    auto value = std::uint64_t{0};
    if (!(input >> std::hex >> value))
        throw std::invalid_argument("Reading does not contain a valid hex value.");

    // Return the proper value
    return value;
}

Location Reading::getLocationValue() const
{
    // Check that the string looks like how it should
    const auto& front = m_values.front();
    if (front.find(LOCATION_DIVIDER) == std::string::npos ||
        front.find(LOCATION_DIVIDER) != front.rfind(LOCATION_DIVIDER))
        throw std::invalid_argument("Reading does not contain a valid location value.");

    // Divide the two strings
    auto latitude = front.substr(0, front.find(LOCATION_DIVIDER));
    auto longitude = front.substr(front.find(LOCATION_DIVIDER) + 1);

    // Return the value
    return Location{std::stof(latitude), std::stof(longitude)};
}

std::vector<std::string> Reading::getStringValues() const
{
    return m_values;
}

std::vector<std::uint64_t> Reading::getUIntValues() const
{
    // Make place for the values
    auto values = std::vector<std::uint64_t>{};
    for (const auto& value : m_values)
        values.emplace_back(std::stoull(value));
    return values;
}

std::vector<std::int64_t> Reading::getIntValues() const
{
    // Make place for the values
    auto values = std::vector<std::int64_t>{};
    for (const auto& value : m_values)
        values.emplace_back(std::stoll(value));
    return values;
}

std::vector<std::double_t> Reading::getDoubleValues() const
{
    // Make place for the values
    auto values = std::vector<std::double_t>{};
    for (const auto& value : m_values)
        values.emplace_back(std::stod(value));
    return values;
}

const std::uint64_t& Reading::getTimestamp() const
{
    return m_timestamp;
}

void Reading::setTimestamp(std::uint64_t timestamp)
{
    m_timestamp = timestamp;
}
}    // namespace wolkabout

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

#ifndef STRINGUTILS_H
#define STRINGUTILS_H

#include "core/utilities/ByteUtils.h"

#include <string>
#include <vector>

namespace wolkabout
{
class StringUtils
{
public:
    StringUtils() = delete;

    static bool contains(const std::string& string, char c);

    static bool contains(const std::string& string, const std::string& substring);

    static std::vector<std::string> tokenize(const std::string& string, const std::string& delimiter);

    static bool startsWith(const std::string& string, const std::string& prefix);

    static bool endsWith(const std::string& string, const std::string& suffix);

    static void removeTrailingWhitespace(std::string& string);

    static std::string removePrefix(const std::string& string, const std::string& prefix);

    static std::string removeSufix(const std::string& string, const std::string& sufix);

    static std::string removeSubstring(const std::string& string, const std::string& substring);

    static bool isBase64(unsigned char c);

    static std::string base64Encode(const ByteArray& bytesToEncode);

    static std::string base64Decode(const std::string& encodedString);

    static bool mqttTopicMatch(const std::string& wildcardTopic, const std::string& topic);

    static std::string toUpperCase(const std::string& string);

    template <typename T> static std::string toString(const T& value);

    static std::vector<std::string> tokenize(std::string const& str, const char delim);

    static const std::string EMPTY_STRING;
    static const std::string BOOL_TRUE;
    static const std::string BOOL_FALSE;
};

template <typename T> std::string StringUtils::toString(const T& value)
{
    return std::to_string(value);
}

template <> inline std::string StringUtils::toString<bool>(const bool& value)
{
    return value ? BOOL_TRUE : BOOL_FALSE;
}

template <> inline std::string StringUtils::toString<char*>(char* const& value)
{
    return std::string{value};
}

template <> inline std::string StringUtils::toString<const char*>(const char* const& value)
{
    return std::string{value};
}
}    // namespace wolkabout

#endif

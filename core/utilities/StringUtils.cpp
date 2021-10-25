/*
 * Copyright 2019 WolkAbout Technology s.r.o.
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

#include "StringUtils.h"

#include <algorithm>
#include <cctype>

namespace wolkabout
{
static const std::string base64_chars = "ABCDEFGHIJKLMNOPQRSTUVWXYZ"
                                        "abcdefghijklmnopqrstuvwxyz"
                                        "0123456789+/";

const std::string StringUtils::EMPTY_STRING = "";
const std::string StringUtils::BOOL_TRUE = "true";
const std::string StringUtils::BOOL_FALSE = "false";

bool StringUtils::contains(const std::string& string, char c)
{
    return string.find(c) != std::string::npos;
}

bool StringUtils::contains(const std::string& string, const std::string& substring)
{
    return string.find(substring) != std::string::npos;
}

std::vector<std::string> StringUtils::tokenize(const std::string& string, const std::string& delimiter)
{
    std::vector<std::string> tokens;
    if (string.empty())
    {
        return tokens;
    }

    std::string::size_type size = string.size();
    std::string::size_type position = 0;
    std::string::size_type delimiterPosition = string.find(delimiter, position);

    while (std::string::npos != delimiterPosition && delimiterPosition <= size)
    {
        tokens.push_back(string.substr(position, delimiterPosition - position));
        position = delimiterPosition + 1;
        delimiterPosition = string.find(delimiter, position);
    }

    tokens.push_back(string.substr(position, std::string::npos));
    return tokens;
}

bool StringUtils::startsWith(const std::string& string, const std::string& prefix)
{
    return string.size() >= prefix.size() && 0 == string.compare(0, prefix.size(), prefix);
}

bool StringUtils::endsWith(const std::string& string, const std::string& suffix)
{
    return string.size() >= suffix.size() && 0 == string.compare(string.size() - suffix.size(), suffix.size(), suffix);
}

void StringUtils::removeTrailingWhitespace(std::string& string)
{
    while (!string.empty() && std::isspace(*string.rbegin()))
    {
        string.erase(string.length() - 1);
    }
}

std::string StringUtils::removePrefix(const std::string& string, const std::string& prefix)
{
    if (startsWith(string, prefix))
    {
        std::string ret = string;
        ret.erase(0, prefix.size());

        return ret;
    }

    return string;
}

std::string StringUtils::removeSufix(const std::string& string, const std::string& sufix)
{
    if (endsWith(string, sufix))
    {
        std::string ret = string;
        ret.erase(string.size() - sufix.size(), std::string::npos);

        return ret;
    }

    return string;
}

std::string StringUtils::removeSubstring(const std::string& string, const std::string& substring)
{
    auto pos = string.find(substring);
    if (pos != std::string::npos)
    {
        std::string ret = string;
        ret.erase(pos, substring.size());

        return ret;
    }

    return string;
}

bool StringUtils::isBase64(unsigned char c)
{
    return (std::isalnum(c) || (c == '+') || (c == '/'));
}

std::string StringUtils::base64Encode(const ByteArray& bytesToEncode)
{
    std::string ret;
    int i = 0;
    int j = 0;
    std::uint8_t char_array_3[3];
    std::uint8_t char_array_4[4];

    for (const auto& byte : bytesToEncode)
    {
        char_array_3[i++] = byte;
        if (i == 3)
        {
            char_array_4[0] = static_cast<std::uint8_t>((char_array_3[0] & 0xfcu) >> 2u);
            char_array_4[1] =
              static_cast<std::uint8_t>(((char_array_3[0] & 0x03u) << 4u) + ((char_array_3[1] & 0xf0u) >> 4u));
            char_array_4[2] =
              static_cast<std::uint8_t>(((char_array_3[1] & 0x0fu) << 2u) + ((char_array_3[2] & 0xc0u) >> 6u));
            char_array_4[3] = static_cast<std::uint8_t>(char_array_3[2] & 0x3fu);

            for (i = 0; i < 4; ++i)
            {
                ret += base64_chars[char_array_4[i]];
            }
            i = 0;
        }
    }

    if (i)
    {
        for (j = i; j < 3; ++j)
        {
            char_array_3[j] = '\0';
        }

        char_array_4[0] = static_cast<std::uint8_t>(char_array_3[0] & 0xfcu) >> 2u;
        char_array_4[1] =
          static_cast<std::uint8_t>(((char_array_3[0] & 0x03u) << 4u) + ((char_array_3[1] & 0xf0u) >> 4u));
        char_array_4[2] =
          static_cast<std::uint8_t>(((char_array_3[1] & 0x0fu) << 2u) + ((char_array_3[2] & 0xc0u) >> 6u));
        char_array_4[3] = static_cast<std::uint8_t>(char_array_3[2] & 0x3fu);

        for (j = 0; j < i + 1; ++j)
        {
            ret += base64_chars[char_array_4[j]];
        }

        while ((i++ < 3))
        {
            ret += '=';
        }
    }

    return ret;
}

std::string StringUtils::base64Decode(const std::string& encodedString)
{
    std::string ret;

    int in_len = encodedString.size();
    int i = 0;
    int j = 0;
    int in_ = 0;
    unsigned char char_array_4[4], char_array_3[3];

    while (in_len-- && (encodedString[in_] != '=') && isBase64(encodedString[in_]))
    {
        char_array_4[i++] = encodedString[in_];
        in_++;
        if (i == 4)
        {
            for (i = 0; i < 4; i++)
                char_array_4[i] = base64_chars.find(char_array_4[i]);

            char_array_3[0] = (char_array_4[0] << 2) + ((char_array_4[1] & 0x30) >> 4);
            char_array_3[1] = ((char_array_4[1] & 0xf) << 4) + ((char_array_4[2] & 0x3c) >> 2);
            char_array_3[2] = ((char_array_4[2] & 0x3) << 6) + char_array_4[3];

            for (i = 0; (i < 3); i++)
                ret += char_array_3[i];
            i = 0;
        }
    }

    if (i)
    {
        for (j = i; j < 4; j++)
            char_array_4[j] = 0;

        for (j = 0; j < 4; j++)
            char_array_4[j] = base64_chars.find(char_array_4[j]);

        char_array_3[0] = (char_array_4[0] << 2) + ((char_array_4[1] & 0x30) >> 4);
        char_array_3[1] = ((char_array_4[1] & 0xf) << 4) + ((char_array_4[2] & 0x3c) >> 2);
        char_array_3[2] = ((char_array_4[2] & 0x3) << 6) + char_array_4[3];

        for (j = 0; (j < i - 1); j++)
            ret += char_array_3[j];
    }

    return ret;
}

bool StringUtils::mqttTopicMatch(const std::string& wildcardTopic, const std::string& topic)
{
    const auto wildcardTopicTokens = tokenize(wildcardTopic, "/");
    const auto topicTokens = tokenize(topic, "/");

    for (size_t i = 0; i < wildcardTopicTokens.size(); ++i)
    {
        if (wildcardTopicTokens[i] == "+")
        {
            if (topicTokens.size() <= i)
            {
                return false;
            }
        }
        else if (wildcardTopicTokens[i] == "#")
        {
            return topicTokens.size() >= i;
        }
        else
        {
            if (topicTokens.size() <= i)
            {
                return false;
            }
            else if (wildcardTopicTokens[i] != topicTokens[i])
            {
                return false;
            }
        }
    }

    return true;
}

std::string StringUtils::toUpperCase(const std::string& string)
{
    std::string transformed = string;
    std::transform(transformed.begin(), transformed.end(), transformed.begin(), ::toupper);

    return transformed;
}
std::vector<std::string> StringUtils::tokenize(const std::string& str, const char delimiter)
{
    size_t start;
    size_t end = 0;
    std::vector<std::string> out{};

    while ((start = str.find_first_not_of(delimiter, end)) != std::string::npos)
    {
        end = str.find(delimiter, start);
        out.push_back(str.substr(start, end - start));
    }

    return out;
}
}    // namespace wolkabout
